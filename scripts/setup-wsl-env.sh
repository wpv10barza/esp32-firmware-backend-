#!/usr/bin/env bash
set -Eeuo pipefail

cd "$(dirname "$0")/.." || exit 1

echo
echo "============================================================"
echo " PREPARAR WSL - VENV + PLATFORMIO"
echo "============================================================"

if ! command -v python3 >/dev/null 2>&1; then
  echo "[ERROR] python3 no está instalado."
  echo "Instale Python 3 antes de continuar."
  exit 1
fi

if ! python3 -m venv --help >/dev/null 2>&1; then
  echo "[INFO] python3-venv no está disponible."
  if command -v sudo >/dev/null 2>&1 && command -v apt-get >/dev/null 2>&1; then
    echo "[INFO] Instalando python3-venv..."
    sudo apt-get update
    sudo apt-get install -y python3-venv
  else
    echo "[ERROR] No se puede instalar python3-venv automáticamente."
    echo "Instale el paquete python3-venv y vuelva a ejecutar."
    exit 2
  fi
fi

if [[ ! -d ".venv" ]]; then
  echo
  echo "[1] CREAR .venv"
  python3 -m venv .venv
else
  echo
  echo "[1] .venv YA EXISTE"
fi

echo
echo "[2] ACTIVAR .venv"
source .venv/bin/activate

echo
echo "[3] ACTUALIZAR PIP"
python -m pip install --upgrade pip

echo
echo "[4] INSTALAR PLATFORMIO"
python -m pip install --upgrade platformio==6.2.0

echo
echo "[5] VERIFICAR"
echo "Python:"
python --version
echo
echo "PlatformIO:"
pio --version
echo
echo "Python activo:"
command -v python
echo
echo "pio activo:"
command -v pio
echo
echo "Puertos serie visibles:"
ls -l /dev/ttyACM* /dev/ttyUSB* 2>/dev/null || true
echo
echo "Dispositivos PlatformIO:"
pio device list || true
echo
echo "============================================================"
echo " WSL + VENV + PLATFORMIO LISTOS"
echo "============================================================"
echo
echo "Para flash:"
echo "  source .venv/bin/activate"
echo "  ./scripts/flash-panel.sh"
echo