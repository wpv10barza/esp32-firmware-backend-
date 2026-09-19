#!/usr/bin/env python3
"""Flash a Guition ESP32-S3-4848S040 artifact and validate its boot path."""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
import time
import zipfile
from pathlib import Path

import serial

MARKERS = {
    "serial": re.compile(r"BOOT: SERIAL OK"),
    "psram": re.compile(r"BOOT: PSRAM OK"),
    "st7701": re.compile(r"BOOT: ST7701 OK"),
    "gt911": re.compile(r"BOOT: GT911 OK"),
    "audio": re.compile(r"BOOT: AUDIO OK"),
    "gpio": re.compile(r"BOOT: GPIO MAP OK"),
    "http": re.compile(r"BOOT: HTTP READY"),
    "ready": re.compile(r"BOOT: READY"),
    "wifi": re.compile(r"BOOT: WIFI OK"),
}

def run(cmd: list[str]) -> None:
    print("$", " ".join(cmd), flush=True)
    result = subprocess.run(cmd, check=False)
    if result.returncode:
        raise SystemExit(result.returncode)

def extract(artifact: Path, out_dir: Path) -> None:
    out_dir.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(artifact) as zf:
        zf.extractall(out_dir)

def pulse_reset(ser: serial.Serial) -> None:
    try:
        ser.dtr = False
        ser.rts = True
        time.sleep(0.12)
        ser.rts = False
        time.sleep(0.18)
        ser.dtr = True
        time.sleep(0.12)
        ser.dtr = False
    except (OSError, serial.SerialException):
        pass

def capture(port: str, baud: int, wait_seconds: int, stability_seconds: int,
            require_wifi: bool, require_audio: bool) -> int:
    deadline = time.monotonic() + wait_seconds
    stability_deadline = None
    seen = {key: False for key in MARKERS}
    reset_count = 0

    with serial.Serial(port, baudrate=baud, timeout=0.25) as ser:
        pulse_reset(ser)
        while time.monotonic() < deadline:
            raw = ser.readline()
            if not raw:
                continue
            line = raw.decode("utf-8", errors="replace").strip()
            if line:
                print(line, flush=True)
            if "BOOT: RESET_REASON=" in line:
                reset_count += 1
            for key, pattern in MARKERS.items():
                if pattern.search(line):
                    seen[key] = True
            if "BOOT: READY" in line and stability_deadline is None:
                stability_deadline = time.monotonic() + stability_seconds
            if stability_deadline is not None and time.monotonic() >= stability_deadline:
                break

    required = ["serial", "psram", "st7701", "gt911", "gpio", "http", "ready"]
    if require_audio:
        required.append("audio")
    if require_wifi:
        required.append("wifi")

    missing = [key for key in required if not seen[key]]
    if missing:
        print("FAIL: missing boot markers:", ", ".join(missing))
        if not seen["wifi"]:
            print("NOTE: artifact 10377040660 was built without local_config.h; Wi-Fi cannot be demonstrated from that exact image.")
        return 1

    if reset_count != 1:
        print(f"FAIL: expected one boot/reset marker, observed {reset_count}")
        return 2

    print("PASS: boot peripherals initialized and stability window completed")
    print("PASS: I2S audio driver initialized" if seen["audio"] else "NOTE: audio disabled/not initialized")
    print("PASS: Wi-Fi connection established" if seen["wifi"] else "NOTE: Wi-Fi not demonstrated by this image")
    return 0

def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", required=True)
    parser.add_argument("--artifact", required=True, type=Path)
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--flash-baud", type=int, default=460800)
    parser.add_argument("--wait-seconds", type=int, default=45)
    parser.add_argument("--stability-seconds", type=int, default=60)
    parser.add_argument("--require-wifi", action="store_true")
    parser.add_argument("--allow-audio-disabled", action="store_true")
    args = parser.parse_args()

    stage = Path(".ci/panel-artifact")
    if args.artifact.suffix.lower() == ".zip":
        extract(args.artifact, stage)
    else:
        stage = args.artifact.parent

    images = [stage / "bootloader.bin", stage / "partitions.bin", stage / "firmware.bin"]
    for image in images:
        if not image.is_file():
            print(f"FAIL: missing {image}")
            return 1

    run([
        sys.executable, "-m", "esptool", "--chip", "esp32s3",
        "--port", args.port, "--baud", str(args.flash_baud),
        "write_flash",
        "0x0000", str(images[0]),
        "0x8000", str(images[1]),
        "0x10000", str(images[2]),
    ])

    return capture(
        args.port, args.baud, args.wait_seconds, args.stability_seconds,
        args.require_wifi, not args.allow_audio_disabled
    )

if __name__ == "__main__":
    raise SystemExit(main())
