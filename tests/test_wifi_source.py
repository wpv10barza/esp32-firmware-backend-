from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
APP = (ROOT / "include" / "app_config.h").read_text(encoding="utf-8")
PLATFORMIO = (ROOT / "platformio.ini").read_text(encoding="utf-8")


def require(text: str, needle: str, label: str) -> None:
    if needle not in text:
        raise AssertionError(f"missing {label}: {needle}")


# Credentials remain external to tracked firmware.
require(APP, '#include "local_config.h"', "local Wi-Fi config include")
require(APP, "WIFI_SSID_VALUE", "Wi-Fi SSID macro")
require(APP, "WIFI_PASSWORD_VALUE", "Wi-Fi password macro")

# STA setup remains intact.
require(MAIN, "WiFi.persistent(false);", "non-persistent Wi-Fi configuration")
require(MAIN, "WiFi.setAutoReconnect(true);", "automatic reconnect setting")
require(MAIN, "WiFi.mode(WIFI_STA);", "station mode")
require(MAIN, "WiFi.setHostname(app_config::deviceId);", "device hostname")

# Recovery is scan-driven and centralized.
require(MAIN, "WiFi.scanNetworks(false, true)", "recovery scan")
require(MAIN, "WiFi.scanDelete();", "scan cleanup")
require(MAIN, "bool configuredWifiVisible()", "SSID visibility helper")
require(MAIN, "void beginWifiConnection(", "single Wi-Fi begin path")
require(MAIN, "void serviceWifi()", "centralized Wi-Fi recovery")
require(MAIN, "WL_NO_SSID_AVAIL", "missing-AP handling")
require(MAIN, "WL_CONNECTION_LOST", "connection-loss handling")
require(MAIN, 'updatePanel(\n      PanelState::Offline,\n      "Esperando Wi-Fi")', "offline recovery state")

# Never use the old reconnect/disconnect hammering strategy.
if "WiFi.reconnect(" in MAIN:
    raise AssertionError("WiFi.reconnect() must not be used by firmware recovery")
if "WiFi.disconnect(" in MAIN:
    raise AssertionError("WiFi.disconnect() must not be used by firmware recovery")

# Recovery timing is explicit and bounded.
require(APP, "wifiRetryMs = 15000UL", "15 s retry window")
require(APP, "wifiScanMs = 20000UL", "20 s scan window")
require(APP, "healthCheckMs = 30000UL", "30 s backend health interval")

# Runtime diagnostics.
require(MAIN, "wifiStatusLabel", "Wi-Fi status diagnostic")
require(MAIN, "WiFi.gatewayIP().toString()", "gateway diagnostic")
require(MAIN, "WiFi.RSSI()", "RSSI diagnostic")
require(MAIN, "Wi-Fi listo:", "Wi-Fi acquisition diagnostic")

# Preserve the known-working display initialization.
require(MAIN, "st7701_type8_init_operations", "ST7701 type8 init")
if "tl040wvs03_init_operations" in MAIN:
    raise AssertionError("obsolete TL040WVS03 init sequence remains")

# Expected board environment.
require(PLATFORMIO, "[env:panel_4848s040]", "panel_4848s040 environment")

print("Wi-Fi source contract: PASS")
print("- scanned SSID recovery is present")
print("- NO_SSID_AVAIL no longer calls WiFi.reconnect()")
print("- recovery is centralized in serviceWifi()")
print("- retry/scan timing is explicit")
print("- display and board contracts remain intact")
