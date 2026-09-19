#!/usr/bin/env bash
set -euo pipefail

PORT="${1:-}"
LOG="${2:-esp32-physical-e2e.log}"
WORKSPACE="${GITHUB_WORKSPACE:-.}"

if [[ ! -f include/local_config.h ]]; then
  echo "PHYSICAL E2E: FAIL include/local_config.h is required on the self-hosted runner" >&2
  exit 1
fi

if [[ -z "$PORT" ]]; then
  PORT="$(pio device list 2>/dev/null | grep -oE '/dev/tty(ACM|USB)[0-9]+' | head -n1 || true)"
fi

if [[ -z "$PORT" || ! -e "$PORT" ]]; then
  echo "PHYSICAL E2E: FAIL no ESP32 serial device detected" >&2
  pio device list || true
  exit 1
fi

echo "PHYSICAL E2E: using $PORT"
rm -f "$LOG"

pio run -e panel_4848s040 -t upload --upload-port "$PORT"

echo "PHYSICAL E2E: collecting serial boot/state evidence"
(
  timeout --signal=TERM 80s pio device monitor --port "$PORT" --baud 115200 || true
) >"$LOG" 2>&1 &
MONITOR_PID=!

sleep 75
kill "$MONITOR_PID" 2>/dev/null || true
wait "$MONITOR_PID" 2>/dev/null || true

for pattern in   'ESP32-4848S040 3C'   'DISPLAY: display->begin() OK'   'Wi-Fi listo:'   'GET health -> 200'   'PANEL STATE -> WSL DISPONIBLE'; do
  if ! grep -Fq "$pattern" "$LOG"; then
    echo "PHYSICAL E2E: FAIL missing serial evidence: $pattern" >&2
    cat "$LOG"
    exit 1
  fi
done

DEVICE_IP="$(grep -Eo 'Wi-Fi listo: http://[0-9.]+/' "$LOG" | tail -n1 | sed -E 's#.*http://([0-9.]+)/#\1#')"
if [[ -z "$DEVICE_IP" ]]; then
  echo "PHYSICAL E2E: FAIL could not extract device IP from serial log" >&2
  cat "$LOG"
  exit 1
fi

HEALTH_BODY="$(curl --fail --silent --show-error --max-time 10 "http://${DEVICE_IP}/health")"
python3 - "$HEALTH_BODY" <<'PY'
import json
import sys

payload = json.loads(sys.argv[1])
expected = {
    "ok": True,
    "board": "ESP32-4848S040",
    "wifi": True,
    "backend": True,
}
for key, value in expected.items():
    if payload.get(key) != value:
        raise SystemExit(
            f"PHYSICAL E2E: FAIL /health field {key!r} "
            f"expected {value!r}, got {payload.get(key)!r}"
        )
print("PHYSICAL E2E: device /health PASS (ok=true, board=ESP32-4848S040, wifi=true, backend=true)")
PY

mkdir -p "$WORKSPACE"
cp "$LOG" "$WORKSPACE/esp32-physical-e2e.log"
echo "PHYSICAL E2E: PASS (USB flash + boot + ST7701 + Wi-Fi + backend health + device HTTP health)"
echo "PHYSICAL E2E: optical legibility, touch-zone routing, and induced backend ERROR require the physical validation procedure/camera evidence"
