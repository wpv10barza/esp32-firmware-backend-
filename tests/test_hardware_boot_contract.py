from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
PIO = (ROOT / "platformio.ini").read_text(encoding="utf-8")

def require(text: str, needle: str, label: str) -> None:
    if needle not in text:
        raise AssertionError(f"missing {label}: {needle}")

for needle, label in [
    ("BOOT: SERIAL OK", "serial boot marker"),
    ("BOOT: RESET_REASON=", "reset reason marker"),
    ("BOOT: PSRAM OK", "PSRAM marker"),
    ("BOOT: ST7701 OK", "ST7701 marker"),
    ("probeTouchController()", "GT911 probe"),
    ("BOOT: GT911 OK", "GT911 marker"),
    ("BOOT: AUDIO OK", "audio marker"),
    ("BOOT: GPIO MAP OK", "GPIO marker"),
    ("BOOT: HTTP READY", "HTTP marker"),
    ("BOOT: READY", "ready marker"),
    ('web.on("/api/diagnostics", HTTP_GET', "diagnostics endpoint"),
    ("STABILITY: uptime_ms=", "stability telemetry"),
]:
    require(MAIN, needle, label)

require(MAIN, "heap_caps_get_free_size(MALLOC_CAP_SPIRAM)", "PSRAM telemetry")
require(MAIN, "esp_reset_reason()", "reset telemetry")
require(PIO, "-D ARDUINO_USB_CDC_ON_BOOT=1", "USB CDC boot logging")

print("Hardware boot contract: PASS")
