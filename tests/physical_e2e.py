#!/usr/bin/env python3
"""Run a real-device E2E check from a self-hosted GitHub Actions runner.

This test is intentionally impossible to satisfy on ubuntu-latest: it requires
an ESP32-S3-4848S040 connected to the runner over USB and reachable on Wi-Fi.
It records timestamps, device /health, device->backend health, and serial logs.
It never invents APPLIED/REJECTED/ERROR states; terminal command state must be
observed in the real serial stream when command mode is explicitly enabled.
"""
from __future__ import annotations

import json
import os
import pathlib
import re
import sys
import threading
import time
import urllib.error
import urllib.request
from datetime import datetime, timezone

try:
    import serial
except ImportError as exc:
    raise SystemExit("pyserial is required") from exc

PORT = os.environ.get("ESP32_SERIAL_PORT")
DEVICE_IP = os.environ.get("ESP32_IP")
OUT = pathlib.Path(os.environ.get("EVIDENCE_DIR", "physical-e2e-evidence"))
RUN_COMMAND = os.environ.get("RUN_COMMAND_E2E", "false").lower() == "true"
COMMAND_TEXT = os.environ.get("COMMAND_TEXT", "E2E physical validation")
WAIT_SECONDS = int(os.environ.get("PHYSICAL_E2E_TIMEOUT", "90"))
BAUD = int(os.environ.get("ESP32_BAUD", "115200"))

if not PORT or not DEVICE_IP:
    raise SystemExit("ESP32_SERIAL_PORT and ESP32_IP are required")

OUT.mkdir(parents=True, exist_ok=True)
serial_lines: list[str] = []
stop = threading.Event()

def now() -> str:
    return datetime.now(timezone.utc).isoformat()

def reader(ser: serial.Serial) -> None:
    while not stop.is_set():
        try:
            raw = ser.readline()
        except Exception:
            break
        if raw:
            serial_lines.append(f"{now()} {raw.decode('utf-8', errors='replace').rstrip()}")

def request(path: str, method: str = "GET", data: bytes | None = None) -> tuple[int, str]:
    req = urllib.request.Request(f"http://{DEVICE_IP}{path}", method=method, data=data)
    req.add_header("Content-Type", "application/x-www-form-urlencoded")
    try:
        with urllib.request.urlopen(req, timeout=10) as response:
            return response.status, response.read().decode("utf-8", errors="replace")
    except urllib.error.HTTPError as exc:
        return exc.code, exc.read().decode("utf-8", errors="replace")

ser = serial.Serial(PORT, BAUD, timeout=1)
thread = threading.Thread(target=reader, args=(ser,), daemon=True)
thread.start()
time.sleep(2)

records = []
def record(name: str, status: int, body: str) -> None:
    records.append({"timestamp": now(), "name": name, "http_status": status, "body": body})

# Device-local endpoint proves the physical firmware is serving requests.
status, body = request("/health")
record("device_health", status, body)
if status != 200:
    stop.set(); ser.close()
    raise SystemExit(f"device /health failed: HTTP {status}: {body}")

# This endpoint is implemented by the ESP32 and performs GET /api/device/v1/health.
status, body = request("/api/backend-health", method="POST")
record("device_to_backend_health", status, body)
if status != 200:
    stop.set(); ser.close()
    raise SystemExit(f"ESP32 -> backend health failed: HTTP {status}: {body}")

if RUN_COMMAND:
    payload = urllib.parse.urlencode({"text": COMMAND_TEXT}).encode()
    status, body = request("/api/3c", method="POST", data=payload)
    record("command_post_via_device", status, body)
    if status not in (202, 200):
        stop.set(); ser.close()
        raise SystemExit(f"ESP32 command POST failed: HTTP {status}: {body}")

    deadline = time.monotonic() + WAIT_SECONDS
    terminal = re.compile(r"PANEL STATE -> (APLICADO|RECHAZADO|ERROR)\b")
    observed = None
    while time.monotonic() < deadline:
        time.sleep(1)
        joined = "\n".join(serial_lines)
        match = terminal.search(joined)
        if match:
            observed = match.group(1)
            break
    if observed is None:
        stop.set(); ser.close()
        pathlib.Path(OUT / "serial.log").write_text("\n".join(serial_lines), encoding="utf-8")
        raise SystemExit("No terminal command state was observed on the physical serial stream")
    record("command_terminal_state_observed", 0, observed)

stop.set()
time.sleep(1)
ser.close()

pathlib.Path(OUT / "serial.log").write_text("\n".join(serial_lines), encoding="utf-8")
pathlib.Path(OUT / "http-evidence.json").write_text(json.dumps(records, indent=2), encoding="utf-8")
manifest = {
    "validation": "PHYSICAL_DEVICE_E2E",
    "timestamp_utc": now(),
    "device_ip": DEVICE_IP,
    "serial_port": PORT,
    "command_e2e_requested": RUN_COMMAND,
    "terminal_command_state": next((r["body"] for r in records if r["name"] == "command_terminal_state_observed"), None),
    "serial_log_lines": len(serial_lines),
    "claim_boundary": "This evidence is valid only when produced by this self-hosted runner with the physical ESP32 connected over USB.",
}
pathlib.Path(OUT / "manifest.json").write_text(json.dumps(manifest, indent=2), encoding="utf-8")
print(json.dumps(manifest, indent=2))
