#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WINDOWS_BUSID=""
DO_FLASH=false
DO_MONITOR=false
RECONFIGURE=false
BAUD=115200
PORT=""
USB_TOPOLOGY="unknown"
STABLE_ID=""

STATE_DIR="${XDG_CONFIG_HOME:-$HOME/.config}/pcu-4848"
CACHE_DIR="${XDG_CACHE_HOME:-$HOME/.cache}/pcu-4848"
CONFIG_FILE="$STATE_DIR/local_config.h"
PIO_VENV="$STATE_DIR/platformio-venv"
PLATFORMIO_VERSION="6.2.0"

usage() {
  cat <<'EOF'
PCU 4848 - Ubuntu/WSL utility for ESP32-S3-4848S040

Uso:
  bash pcu-4848.sh                       # detectar USB + puerto + IP
  bash pcu-4848.sh --flash               # configurar (si hace falta), compilar y flashear
  bash pcu-4848.sh --monitor             # monitor serie
  bash pcu-4848.sh --flash --monitor     # flashear y dejar monitor abierto
  bash pcu-4848.sh --reconfigure --flash # volver a pedir Wi-Fi/token

La configuracion privada se guarda con permisos 600 en ~/.config/pcu-4848/.
Nunca se escribe en GitHub ni en el paquete distribuido.
EOF
}

while (($#)); do
  case "$1" in
    --windows-busid)
      [[ $# -ge 2 ]] || { echo "ERROR: --windows-busid requiere valor" >&2; exit 64; }
      WINDOWS_BUSID="$2"; shift 2 ;;
    --flash) DO_FLASH=true; shift ;;
    --monitor) DO_MONITOR=true; shift ;;
    --reconfigure) RECONFIGURE=true; shift ;;
    --help|-h) usage; exit 0 ;;
    *) echo "ERROR: argumento desconocido: $1" >&2; usage >&2; exit 64 ;;
  esac
done

mkdir -p "$STATE_DIR" "$CACHE_DIR"
chmod 700 "$STATE_DIR" "$CACHE_DIR" 2>/dev/null || true

list_ports() {
  local found=()
  if [[ -d /dev/serial/by-id ]]; then
    while IFS= read -r p; do found+=("$p"); done < <(find /dev/serial/by-id -maxdepth 1 -type l -print 2>/dev/null | sort)
  fi
  if ((${#found[@]} == 0)); then
    while IFS= read -r p; do found+=("$p"); done < <(find /dev -maxdepth 1 \( -name 'ttyACM*' -o -name 'ttyUSB*' \) -print 2>/dev/null | sort)
  fi
  printf '%s\n' "${found[@]}"
}

looks_like_esp32() {
  local p="$1" props=""
  if command -v udevadm >/dev/null 2>&1; then
    props="$(udevadm info --query=property --name "$p" 2>/dev/null || true)"
    grep -Eqi 'ID_VENDOR_ID=303a|ID_VENDOR=.*Espressif|ID_MODEL=.*(ESP32|JTAG|Serial)' <<<"$props" && return 0
  fi
  [[ "$p" =~ [Ee]spressif|ESP32|JTAG ]] && return 0
  return 1
}

select_port() {
  local ports=()
  mapfile -t ports < <(list_ports)
  if ((${#ports[@]} == 0)); then
    echo "ERROR: Ubuntu/WSL no ve ningun /dev/ttyACM* ni /dev/ttyUSB*." >&2
    echo "Si usa WSL, confirme que PCU Windows haya adjuntado el BUSID con usbipd." >&2
    return 1
  fi
  if ((${#ports[@]} == 1)); then
    PORT="${ports[0]}"
    return 0
  fi

  local esp=()
  local p
  for p in "${ports[@]}"; do
    if looks_like_esp32 "$p"; then esp+=("$p"); fi
  done
  if ((${#esp[@]} == 1)); then
    PORT="${esp[0]}"
    return 0
  fi

  echo "Se detectaron varios puertos serie:" >&2
  local i
  for i in "${!ports[@]}"; do printf '  [%d] %s\n' "$((i+1))" "${ports[$i]}" >&2; done
  if [[ -t 0 ]]; then
    local choice
    read -r -p "Seleccione el ESP32: " choice
    [[ "$choice" =~ ^[0-9]+$ ]] || return 1
    ((choice >= 1 && choice <= ${#ports[@]})) || return 1
    PORT="${ports[$((choice-1))]}"
    return 0
  fi
  echo "ERROR: seleccion ambigua; no se elegira un dispositivo automaticamente." >&2
  return 1
}

usb_topology_for_port() {
  local real tty sys current base
  real="$(readlink -f "$1" 2>/dev/null || printf '%s' "$1")"
  tty="$(basename "$real")"
  sys="/sys/class/tty/$tty/device"
  [[ -e "$sys" ]] || { printf 'unknown'; return 0; }
  current="$(readlink -f "$sys")"
  while [[ "$current" != / && -n "$current" ]]; do
    base="$(basename "$current")"
    if [[ "$base" =~ ^[0-9]+-[0-9]+(\.[0-9]+)*$ ]]; then
      printf '%s' "$base"
      return 0
    fi
    current="$(dirname "$current")"
  done
  printf 'unknown'
}

refresh_device_identity() {
  select_port
  local real
  real="$(readlink -f "$PORT" 2>/dev/null || printf '%s' "$PORT")"
  if [[ "$PORT" == /dev/serial/by-id/* ]]; then STABLE_ID="$PORT"; else STABLE_ID=""; fi
  PORT="$real"
  USB_TOPOLOGY="$(usb_topology_for_port "$PORT")"
}

health_ip() {
  local host='esp32-panel-3c.local' body='' ip=''
  if command -v curl >/dev/null 2>&1; then
    body="$(curl -fsS --max-time 2 "http://$host/health" 2>/dev/null || true)"
    ip="$(sed -n 's/.*"ip":"\([^"]*\)".*/\1/p' <<<"$body" | head -n1)"
    [[ -n "$ip" ]] && { printf '%s' "$ip"; return 0; }
  fi
  if command -v getent >/dev/null 2>&1; then
    ip="$(getent ahostsv4 "$host" 2>/dev/null | awk 'NR==1{print $1}')"
    [[ -n "$ip" ]] && { printf '%s' "$ip"; return 0; }
  fi
  return 1
}

serial_ip() {
  local seconds="${1:-4}" ip=''
  [[ -r "$PORT" ]] || return 1
  command -v timeout >/dev/null 2>&1 || return 1
  stty -F "$PORT" "$BAUD" raw -echo -ixon -ixoff 2>/dev/null || true
  ip="$(timeout "$seconds" awk '
    /Wi-Fi listo: http:\/\/[0-9.]+\// {
      if (match($0,/http:\/\/[0-9.]+\//)) {
        s=substr($0,RSTART+7,RLENGTH-8); print s; exit
      }
    }
  ' "$PORT" 2>/dev/null || true)"
  [[ -n "$ip" ]] && { printf '%s' "$ip"; return 0; }
  return 1
}

show_status() {
  local ip=''
  echo
  echo "============================================================"
  echo " PCU 4848 - ESTADO"
  echo "============================================================"
  [[ -n "$WINDOWS_BUSID" ]] && echo "BUSID Windows/usbipd : $WINDOWS_BUSID"
  echo "Topologia USB Linux : $USB_TOPOLOGY"
  echo "Puerto serie actual : $PORT"
  [[ -n "$STABLE_ID" ]] && echo "ID serie estable    : $STABLE_ID"
  if ip="$(health_ip)"; then
    echo "IP ESP32            : $ip"
    echo "Panel HTTP          : http://$ip/"
  elif ip="$(serial_ip 3)"; then
    echo "IP ESP32            : $ip (detectada por serie)"
    echo "Panel HTTP          : http://$ip/"
  else
    echo "IP ESP32            : aun no detectada"
    echo "mDNS panel          : esp32-panel-3c.local"
  fi
  echo "Backend mDNS        : 3c-backend.local / _3c._tcp"
}

c_escape() {
  local s="$1"
  s="${s//\\/\\\\}"
  s="${s//\"/\\\"}"
  printf '%s' "$s"
}

configure_private_values() {
  if [[ "$RECONFIGURE" == false && -s "$CONFIG_FILE" ]]; then
    return 0
  fi
  [[ -t 0 ]] || { echo "ERROR: falta configuracion privada y no hay terminal interactiva." >&2; return 1; }
  local ssid password token device audio brightness
  echo
  echo "Configuracion privada local (NO se sube a GitHub):"
  read -r -p "Wi-Fi SSID (2.4 GHz): " ssid
  read -r -s -p "Wi-Fi password: " password; echo
  read -r -s -p "Token API 3C (Enter si su backend no lo exige): " token; echo
  read -r -p "Device ID [panel-4848s040-3c-01]: " device
  device="${device:-panel-4848s040-3c-01}"
  read -r -p "Audio habilitado 1/0 [1]: " audio
  audio="${audio:-1}"
  read -r -p "Brillo 0-255 [180]: " brightness
  brightness="${brightness:-180}"
  [[ -n "$ssid" ]] || { echo "ERROR: SSID vacio." >&2; return 1; }
  [[ "$audio" == 0 || "$audio" == 1 ]] || { echo "ERROR: audio debe ser 0 o 1." >&2; return 1; }
  [[ "$brightness" =~ ^[0-9]+$ ]] && ((brightness >= 0 && brightness <= 255)) || { echo "ERROR: brillo invalido." >&2; return 1; }

  umask 077
  cat >"$CONFIG_FILE" <<EOF
#pragma once
#define WIFI_SSID_VALUE "$(c_escape "$ssid")"
#define WIFI_PASSWORD_VALUE "$(c_escape "$password")"
#define ASSISTANT_BASE_URL_VALUE ""
#define ESP32_API_TOKEN_VALUE "$(c_escape "$token")"
#define DEVICE_ID_VALUE "$(c_escape "$device")"
#define DEFAULT_3C_COMMAND_VALUE "Cambia la tarea J10 a mensual"
#define PANEL_AUDIO_ENABLED_VALUE $audio
#define PANEL_BRIGHTNESS_VALUE $brightness
EOF
  chmod 600 "$CONFIG_FILE"
  echo "Configuracion guardada en $CONFIG_FILE (modo 600)."
}

source_bundle() {
  if [[ -f "$ROOT/source/platformio.ini" ]]; then
    printf '%s' "$ROOT/source"
  elif [[ -f "$ROOT/../platformio.ini" && -d "$ROOT/../src" && -d "$ROOT/../include" ]]; then
    (cd "$ROOT/.." && pwd)
  else
    return 1
  fi
}

source_sha() {
  if [[ -s "$ROOT/source-sha.txt" ]]; then
    head -n1 "$ROOT/source-sha.txt" | tr -d '\r\n'
  elif command -v git >/dev/null 2>&1 && git -C "$ROOT/.." rev-parse HEAD >/dev/null 2>&1; then
    git -C "$ROOT/.." rev-parse HEAD
  else
    printf 'unknown'
  fi
}

prepare_workdir() {
  local bundle sha tag work
  bundle="$(source_bundle)" || { echo "ERROR: el paquete no contiene source/ ni se ejecuta desde el repositorio." >&2; return 1; }
  sha="$(source_sha)"
  tag="${sha:0:12}"
  [[ "$tag" == unknown || -z "$tag" ]] && tag='bundle'
  work="$CACHE_DIR/source-$tag"
  rm -rf "$work"
  mkdir -p "$work/include" "$work/src"
  cp "$bundle/platformio.ini" "$work/platformio.ini"
  cp "$bundle"/include/*.h "$work/include/"
  rm -f "$work/include/local_config.h"
  cp "$bundle"/src/main.cpp "$work/src/main.cpp"
  cp "$CONFIG_FILE" "$work/include/local_config.h"
  chmod 600 "$work/include/local_config.h"
  printf '%s' "$work"
}

ensure_platformio() {
  if command -v pio >/dev/null 2>&1; then
    command -v pio
    return 0
  fi
  if [[ -x "$PIO_VENV/bin/pio" ]]; then
    printf '%s' "$PIO_VENV/bin/pio"
    return 0
  fi
  command -v python3 >/dev/null 2>&1 || { echo "ERROR: Python 3 no esta disponible en Ubuntu/WSL." >&2; return 1; }
  if ! python3 -m venv --help >/dev/null 2>&1; then
    if command -v sudo >/dev/null 2>&1 && command -v apt-get >/dev/null 2>&1; then
      echo "Instalando python3-venv para preparar PlatformIO..."
      sudo apt-get update
      sudo apt-get install -y python3-venv
    else
      echo "ERROR: python3-venv no esta disponible." >&2
      return 1
    fi
  fi
  python3 -m venv "$PIO_VENV"
  "$PIO_VENV/bin/python" -m pip install --upgrade pip
  "$PIO_VENV/bin/python" -m pip install "platformio==$PLATFORMIO_VERSION"
  printf '%s' "$PIO_VENV/bin/pio"
}

save_personalized_build() {
  local work="$1" sha dest
  sha="$(source_sha)"
  dest="$STATE_DIR/last-build"
  rm -rf "$dest"; mkdir -p "$dest"
  for f in firmware.bin bootloader.bin partitions.bin firmware.elf; do
    [[ -f "$work/.pio/build/panel_4848s040/$f" ]] && cp "$work/.pio/build/panel_4848s040/$f" "$dest/$f"
  done
  (cd "$dest" && sha256sum * > SHA256SUMS.txt)
  printf '%s\n' "$sha" > "$dest/source-sha.txt"
  chmod -R go-rwx "$dest" 2>/dev/null || true
  echo "Build personalizado guardado localmente en $dest"
}

flash_panel() {
  configure_private_values
  local work pio_cmd
  work="$(prepare_workdir)"
  pio_cmd="$(ensure_platformio)"
  echo
  echo "============================================================"
  echo " COMPILAR + FLASHEAR SHA VALIDADO"
  echo "============================================================"
  echo "Source SHA : $(source_sha)"
  echo "USB        : ${WINDOWS_BUSID:-$USB_TOPOLOGY}"
  echo "Puerto     : $PORT"
  "$pio_cmd" run --project-dir "$work" -e panel_4848s040 -t upload --upload-port "$PORT"
  save_personalized_build "$work"
  sleep 2
  refresh_device_identity || true
  local ip=''
  echo "Buscando IP despues del reinicio..."
  if ip="$(serial_ip 20)"; then
    echo "IP ESP32: $ip"
    echo "Panel: http://$ip/"
  else
    local i
    for i in {1..10}; do
      if ip="$(health_ip)"; then
        echo "IP ESP32: $ip"
        echo "Panel: http://$ip/"
        break
      fi
      sleep 2
    done
    [[ -n "$ip" ]] || echo "IP aun no visible; abra monitor para ver el estado Wi-Fi."
  fi
}

open_monitor() {
  local pio_cmd=''
  if pio_cmd="$(ensure_platformio 2>/dev/null)"; then
    exec "$pio_cmd" device monitor --port "$PORT" --baud "$BAUD"
  fi
  stty -F "$PORT" "$BAUD" raw -echo -ixon -ixoff
  exec cat "$PORT"
}

refresh_device_identity
show_status

if [[ "$DO_FLASH" == true ]]; then
  flash_panel
  show_status || true
fi
if [[ "$DO_MONITOR" == true ]]; then
  open_monitor
fi
