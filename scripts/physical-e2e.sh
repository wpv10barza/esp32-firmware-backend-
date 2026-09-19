#!/usr/bin/env bash
set -euo pipefail

PORT="${1:-}"
LOG="${2:-esp32-physical-e2e.log}"
WORKSPACE="${GITHUB_WORKSPACE:-.}"
BACKUP="/tmp/local_config.h.esp32-e2e.$$"
STUB_PID=""
STUB_LOG="/tmp/esp32-e2e-stub.log"

cleanup() {
  set +e
  if [[ -n "$STUB_PID" ]]; then
    kill "$STUB_PID" 2>/dev/null || true
    wait "$STUB_PID" 2>/dev/null || true
  fi
  if [[ -f "$BACKUP" ]]; then
    cp "$BACKUP" include/local_config.h
    echo "PHYSICAL E2E: restored original local_config.h"
    pio run -e panel_4848s040 -t upload --upload-port "$PORT" >/tmp/esp32-e2e-restore.log 2>&1 || {
      echo "PHYSICAL E2E: WARNING could not restore original firmware"
      cat /tmp/esp32-e2e-restore.log
    }
    rm -f "$BACKUP"
  fi
}
trap cleanup EXIT INT TERM

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

RUNNER_IP="$(ip route get 1.1.1.1 | awk '{for (i=1; i<=NF; i++) if ($i=="src") {print $(i+1); exit}}')"
if [[ -z "$RUNNER_IP" ]]; then
  echo "PHYSICAL E2E: FAIL could not determine self-hosted runner LAN IP" >&2
  exit 1
fi

echo "PHYSICAL E2E: using $PORT"
echo "PHYSICAL E2E: runner LAN IP $RUNNER_IP"
rm -f "$LOG" "$STUB_LOG"

capture_serial() {
  local output="$1"
  local seconds="$2"
  (
    timeout --signal=TERM "${seconds}s" pio device monitor --port "$PORT" --baud 115200 || true
  ) >"$output" 2>&1 &
  echo $!
}

wait_for_device_health() {
  local ip="$1"
  local expected_backend="$2"
  local last=""
  for _ in $(seq 1 25); do
    last="$(curl --silent --show-error --max-time 3 "http://${ip}/health" 2>&1 || true)"
    if [[ -n "$last" ]]; then
      python3 - "$last" "$expected_backend" <<'PY'
import json
import sys

payload = json.loads(sys.argv[1])
expected_backend = sys.argv[2] == "true"
if payload.get("ok") is True and payload.get("board") == "ESP32-4848S040" and payload.get("wifi") is True and payload.get("backend") is expected_backend:
    raise SystemExit(0)
raise SystemExit(1)
PY
      if [[ $? -eq 0 ]]; then
        echo "$last"
        return 0
      fi
    fi
    sleep 1
  done
  echo "$last"
  return 1
}

wait_for_serial() {
  local file="$1"
  local pattern="$2"
  local timeout_s="$3"
  for _ in $(seq 1 "$timeout_s"); do
    if grep -Fq "$pattern" "$file"; then
      return 0
    fi
    sleep 1
  done
  return 1
}

echo "PHYSICAL E2E: initial firmware upload"
cp include/local_config.h "$BACKUP"
pio run -e panel_4848s040 -t upload --upload-port "$PORT"

BOOT_PID="$(capture_serial /tmp/esp32-e2e-boot.log 40)"
sleep 35
kill "$BOOT_PID" 2>/dev/null || true
wait "$BOOT_PID" 2>/dev/null || true

for pattern in   'ESP32-4848S040 3C'   'DISPLAY: display->begin() OK'   'Wi-Fi listo:'   'GET health -> 200'   'PANEL STATE -> WSL DISPONIBLE'; do
  if ! grep -Fq "$pattern" /tmp/esp32-e2e-boot.log; then
    echo "PHYSICAL E2E: FAIL missing serial evidence: $pattern" >&2
    cat /tmp/esp32-e2e-boot.log
    exit 1
  fi
done

DEVICE_IP="$(grep -Eo 'Wi-Fi listo: http://[0-9.]+/' /tmp/esp32-e2e-boot.log | tail -n1 | sed -E 's#.*http://([0-9.]+)/#\1#')"
if [[ -z "$DEVICE_IP" ]]; then
  echo "PHYSICAL E2E: FAIL could not extract device IP from boot serial log" >&2
  cat /tmp/esp32-e2e-boot.log
  exit 1
fi

HEALTH_BODY="$(wait_for_device_health "$DEVICE_IP" true)" || {
  echo "PHYSICAL E2E: FAIL healthy device /health" >&2
  cat /tmp/esp32-e2e-boot.log
  exit 1
}
echo "PHYSICAL E2E: initial device /health PASS: $HEALTH_BODY"

cat > /tmp/esp32-e2e-stub.py <<'PY'
import json
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

state = {
    "health_error": False,
    "post_count": 0,
}

COMMAND_ID = "physical-e2e-failed-001"


class Handler(BaseHTTPRequestHandler):
    def _send(self, code, payload):
        body = json.dumps(payload).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        if self.path == "/api/device/v1/health":
            if state["health_error"]:
                self._send(503, {"ok": False, "error": "CI induced health failure"})
            else:
                self._send(200, {
                    "ok": True,
                    "protocol_version": "1.0",
                    "accepts_commands": True,
                    "requires_human_confirmation": True,
                    "supports_status_polling": True,
                })
            return

        if self.path == f"/api/device/v1/commands/{COMMAND_ID}":
            self._send(200, {
                "command_id": COMMAND_ID,
                "status": "failed",
                "result": "CI induced poll failure",
            })
            return

        self._send(404, {"error": "not found"})

    def do_POST(self):
        if self.path == "/control/health-error":
            state["health_error"] = True
            self._send(200, {"ok": True})
            return

        if self.path == "/control/health-ok":
            state["health_error"] = False
            self._send(200, {"ok": True})
            return

        if self.path == "/api/device/v1/commands":
            state["post_count"] += 1
            if state["post_count"] == 1:
                self._send(500, {
                    "error": "CI induced POST failure",
                })
                return
            self._send(202, {
                "command_id": COMMAND_ID,
                "status": "pending_confirmation",
                "requires_human_confirmation": True,
                "status_path": f"/api/device/v1/commands/{COMMAND_ID}",
            })
            return

        self._send(404, {"error": "not found"})

    def log_message(self, format, *args):
        print(format % args, flush=True)


ThreadingHTTPServer(("0.0.0.0", 3033), Handler).serve_forever()
PY

python3 /tmp/esp32-e2e-stub.py >"$STUB_LOG" 2>&1 &
STUB_PID=$!
sleep 1
curl --fail --silent --show-error --max-time 5 "http://${RUNNER_IP}:3033/api/device/v1/health" >/dev/null

cat >> include/local_config.h <<EOF
#undef ASSISTANT_BASE_URL_VALUE
#define ASSISTANT_BASE_URL_VALUE "http://${RUNNER_IP}:3033"
EOF

echo "PHYSICAL E2E: uploading temporary failure-test firmware"
pio run -e panel_4848s040 -t upload --upload-port "$PORT"

TEST_PID="$(capture_serial /tmp/esp32-e2e-error.log 70)"
sleep 15

CONTROL_CODE="$(curl --silent --show-error --max-time 5 -o /tmp/control-health-error.json -w '%{http_code}' -X POST "http://${RUNNER_IP}:3033/control/health-error")"
if [[ "$CONTROL_CODE" != "200" ]]; then
  echo "PHYSICAL E2E: FAIL could not switch stub to health-error mode" >&2
  cat "$STUB_LOG"
  kill "$TEST_PID" 2>/dev/null || true
  exit 1
fi

HEALTH_ERROR_CODE="$(curl --silent --show-error --max-time 8 -o /tmp/device-health-error.json -w '%{http_code}' -X POST "http://${DEVICE_IP}/api/backend-health")"
if [[ "$HEALTH_ERROR_CODE" != "502" ]]; then
  echo "PHYSICAL E2E: FAIL device health error path expected 502, got $HEALTH_ERROR_CODE" >&2
  kill "$TEST_PID" 2>/dev/null || true
  cat /tmp/esp32-e2e-error.log
  exit 1
fi
wait_for_serial /tmp/esp32-e2e-error.log 'PANEL STATE -> ERROR | Health HTTP 503' 10 || {
  echo "PHYSICAL E2E: FAIL missing physical Health -> ERROR transition" >&2
  cat /tmp/esp32-e2e-error.log
  exit 1
}

curl --fail --silent --show-error --max-time 5 -X POST "http://${RUNNER_IP}:3033/control/health-ok" >/dev/null
curl --fail --silent --show-error --max-time 8 -X POST "http://${DEVICE_IP}/api/backend-health" >/dev/null

FIRST_POST_CODE="$(curl --silent --show-error --max-time 8 -o /tmp/device-post-error.json -w '%{http_code}'   -X POST "http://${DEVICE_IP}/api/3c"   -H 'Content-Type: application/x-www-form-urlencoded'   --data-urlencode 'text=Cambia la tarea J10 a mensual')"
if [[ "$FIRST_POST_CODE" != "502" ]]; then
  echo "PHYSICAL E2E: FAIL first device POST error expected local 502, got $FIRST_POST_CODE" >&2
  cat /tmp/esp32-e2e-error.log
  exit 1
fi
wait_for_serial /tmp/esp32-e2e-error.log 'PANEL STATE -> ERROR | POST HTTP 500' 10 || {
  echo "PHYSICAL E2E: FAIL missing physical POST -> ERROR transition" >&2
  cat /tmp/esp32-e2e-error.log
  exit 1
}

SECOND_POST_CODE="$(curl --silent --show-error --max-time 8 -o /tmp/device-post-pending.json -w '%{http_code}'   -X POST "http://${DEVICE_IP}/api/3c"   -H 'Content-Type: application/x-www-form-urlencoded'   --data-urlencode 'text=Cambia la tarea J10 a mensual')"
if [[ "$SECOND_POST_CODE" != "202" ]]; then
  echo "PHYSICAL E2E: FAIL second device POST expected 202, got $SECOND_POST_CODE" >&2
  cat /tmp/esp32-e2e-error.log
  exit 1
fi

wait_for_serial /tmp/esp32-e2e-error.log 'PANEL STATE -> PENDIENTE' 10 || {
  echo "PHYSICAL E2E: FAIL missing physical PENDING transition" >&2
  cat /tmp/esp32-e2e-error.log
  exit 1
}

wait_for_serial /tmp/esp32-e2e-error.log 'GET command status -> 200 status=failed' 10 || {
  echo "PHYSICAL E2E: FAIL missing physical failed polling evidence" >&2
  cat /tmp/esp32-e2e-error.log
  exit 1
}

wait_for_serial /tmp/esp32-e2e-error.log 'PANEL STATE -> ERROR | CI induced poll failure' 10 || {
  echo "PHYSICAL E2E: FAIL missing physical polling failed -> ERROR transition" >&2
  cat /tmp/esp32-e2e-error.log
  exit 1
}

kill "$TEST_PID" 2>/dev/null || true
wait "$TEST_PID" 2>/dev/null || true

cat /tmp/esp32-e2e-boot.log /tmp/esp32-e2e-error.log >"$LOG"

mkdir -p "$WORKSPACE"
cp "$LOG" "$WORKSPACE/esp32-physical-e2e.log"

echo "PHYSICAL E2E: PASS"
echo "- USB flash and physical boot"
echo "- ST7701 display initialization"
echo "- Wi-Fi and backend health -> WSL DISPONIBLE"
echo "- physical Health 503 -> ERROR"
echo "- physical POST 500 -> ERROR"
echo "- physical POST 202 -> PENDIENTE"
echo "- physical polling status=failed -> ERROR"
echo "- final firmware/configuration is restored by cleanup"
echo "- touch/optical legibility is not inferred from serial; it still requires direct physical observation"
