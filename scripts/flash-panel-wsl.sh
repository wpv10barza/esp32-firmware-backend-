#!/usr/bin/env bash
set -Eeuo pipefail

cd "$(dirname "$0")/.."
ENVIRONMENT="panel_4848s040"
BAUD=115200
PORT=""

usage() {
  cat <<'EOF'
Uso:
  ./scripts/flash-panel-wsl.sh
  ./scripts/flash-panel-wsl.sh --port /dev/ttyACM0

El script valida PlatformIO, compila, detecta un único /dev/ttyACM* o
/dev/ttyUSB*, comprueba permisos y luego ejecuta la carga y el monitor.
EOF
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --port|-p)
      [[ $# -ge 2 ]] || { echo "ERROR: --port requiere una ruta." >&2; exit 64; }
      PORT="$2"
      shift 2
      ;;
    --help|-h)
      usage
      exit 0
      ;;
    *)
      echo "ERROR: argumento desconocido: $1" >&2
      usage >&2
      exit 64
      ;;
  esac
done

if [[ ! -f include/local_config.h ]]; then
  echo "ERROR: falta include/local_config.h"
  echo "       Use include/local_config.example.h como base."
  exit 2
fi

if ! command -v pio >/dev/null 2>&1; then
  echo "ERROR: PlatformIO no está disponible en PATH."
  echo "       Pruebe: python3 -m pip install platformio==6.1.18"
  exit 127
fi

if ! pio run -e "$ENVIRONMENT"; then
  echo "ERROR: la compilación de $ENVIRONMENT falló."
  exit 10
fi

detect_port() {
  local ports=()
  mapfile -t ports < <(find /dev -maxdepth 1 -type c \( -name 'ttyACM*' -o -name 'ttyUSB*' \) -print 2>/dev/null | sort)
  if ((${#ports[@]} == 1)); then
    printf '%s\n' "${ports[0]}"
    return 0
  fi
  if ((${#ports[@]} > 1)); then
    echo "ERROR: WSL muestra varios puertos serie:" >&2
    printf '  %s\n' "${ports[@]}" >&2
    echo "       Seleccione uno con --port." >&2
  else
    echo "ERROR: WSL no muestra ningún /dev/ttyACM* ni /dev/ttyUSB*." >&2
  fi
  return 1
}

if [[ -z "$PORT" ]]; then
  PORT="$(detect_port)" || {
    echo
    echo "Compruebe:"
    echo "  pio device list"
    echo "  lsusb"
    echo "  ls -l /dev/ttyACM* /dev/ttyUSB*"
    echo
    echo "Si Windows ve el ESP32 pero WSL no, adjunte el USB a WSL 2 con usbipd."
    exit 20
  }
fi

if [[ ! -e "$PORT" ]]; then
  echo "ERROR: el puerto no existe: $PORT"
  exit 21
fi

if [[ ! -r "$PORT" || ! -w "$PORT" ]]; then
  echo "ERROR: sin permisos de lectura/escritura: $PORT"
  ls -l "$PORT" || true
  echo "       Revise: id; groups"
  exit 22
fi

echo "Puerto seleccionado: $PORT"
echo "Cargando firmware..."

if ! pio run -e "$ENVIRONMENT" -t upload --upload-port "$PORT"; then
  echo
  echo "ERROR: PlatformIO no pudo cargar $ENVIRONMENT en $PORT."
  echo "       Reintente con el ESP32 en modo BOOT si el hardware lo requiere."
  echo "       Diagnóstico: pio device list"
  exit 30
fi

echo "Carga correcta. Monitor serie $BAUD baudios:"
exec pio device monitor --port "$PORT" --baud "$BAUD"
