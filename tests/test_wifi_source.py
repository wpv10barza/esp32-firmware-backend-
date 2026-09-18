from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
APP = (ROOT / "include" / "app_config.h").read_text(encoding="utf-8")
PLATFORMIO = (ROOT / "platformio.ini").read_text(encoding="utf-8")


def require(text: str, needle: str, label: str) -> None:
    if needle not in text:
        raise AssertionError(f"missing {label}: {needle}")


# Wi-Fi must come from ignored local_config.h, never from tracked firmware.
require(APP, '#include "local_config.h"', "local Wi-Fi config include")
require(APP, "WIFI_SSID_VALUE", "Wi-Fi SSID macro")
require(APP, "WIFI_PASSWORD_VALUE", "Wi-Fi password macro")
require(APP, "ASSISTANT_BASE_URL_VALUE", "backend URL macro")

# Firmware must use the configured credentials in STA mode.
require(MAIN, "WiFi.persistent(false);", "non-persistent Wi-Fi configuration")
require(MAIN, "WiFi.setAutoReconnect(true);", "automatic reconnect")
require(MAIN, "WiFi.mode(WIFI_STA);", "station mode")
require(MAIN, "WiFi.begin(app_config::wifiSsid, app_config::wifiPassword);", "configured Wi-Fi credentials")
require(MAIN, "WiFi.reconnect();", "Wi-Fi reconnect path")
require(MAIN, "updatePanel(PanelState::Busy, \"Reconectando Wi-Fi\");", "reconnect state")
require(MAIN, "WiFi.status() == WL_CONNECTED", "real Wi-Fi state check")

# Missing credentials must fail safely instead of calling WiFi.begin() with an
# empty SSID. This guards the exact local_config.h failure mode seen in WSL.
require(MAIN, "if (!strlen(app_config::wifiSsid))", "missing-SSID startup guard")
require(MAIN, "Configure include/local_config.h antes de usar Wi-Fi.", "missing local Wi-Fi configuration diagnostic")

# Configure station mode before beginning the connection, and keep the
# reconnect path non-destructive.
configure_pos = MAIN.find("void configureWifi()")
connect_pos = MAIN.find("void connectWifi()")
begin_pos = MAIN.find("WiFi.begin(app_config::wifiSsid, app_config::wifiPassword);")
if configure_pos < 0 or connect_pos < 0 or begin_pos < 0:
    raise AssertionError("Wi-Fi startup functions could not be located")
if configure_pos > connect_pos or connect_pos > begin_pos:
    raise AssertionError("Wi-Fi startup order must be configureWifi() -> connectWifi() -> WiFi.begin()")

# Reconnect logic must not repeatedly reset the station with WiFi.disconnect().
if "WiFi.disconnect();" in MAIN:
    raise AssertionError("reconnect path must use WiFi.reconnect(), not repeated WiFi.disconnect()+WiFi.begin()")

# Diagnostics must expose real connection status without printing credentials.
require(MAIN, "wifiStatusLabel", "Wi-Fi status diagnostic")
require(MAIN, "WiFi.gatewayIP().toString()", "gateway diagnostic")
require(MAIN, "WiFi.RSSI()", "RSSI diagnostic")
require(MAIN, "GET health ->", "backend health diagnostic")
require(MAIN, "Wi-Fi listo:", "Wi-Fi acquisition diagnostic")

# Preserve the known-working ST7701 initialization.
require(MAIN, "st7701_type8_init_operations", "ST7701 type8 init sequence")
if "tl040wvs03_init_operations" in MAIN:
    raise AssertionError("obsolete TL040WVS03 init sequence is still referenced")

# The tracked default must never point to loopback for the ESP32-to-host bridge.
if 'ASSISTANT_BASE_URL_VALUE "http://127.0.0.1:' in APP:
    raise AssertionError("ESP32 backend default must not use loopback")

# The firmware must keep using the expected board environment.
require(PLATFORMIO, "[env:panel_4848s040]", "panel_4848s040 environment")

# No real credentials are allowed in tracked firmware source.
if "WIFI_PASSWORD_VALUE \"" in MAIN:
    raise AssertionError("Wi-Fi password must not be hard-coded in main.cpp")
if "ESP32_API_TOKEN_VALUE \"" in MAIN:
    raise AssertionError("ESP32 API token must not be hard-coded in main.cpp")

print("Wi-Fi source contract: PASS")
print("- credentials sourced from ignored local_config.h")
print("- STA mode + auto-reconnect + non-destructive reconnect present")
print("- empty-SSID startup fails safely before WiFi.begin()")
print("- diagnostic exposes status/gateway/RSSI without credentials")
print("- backend URL is not hard-coded to loopback")
print("- ST7701 type8 init preserved")
