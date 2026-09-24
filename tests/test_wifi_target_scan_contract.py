from pathlib import Path

source = Path("src/main.cpp").read_text(encoding="utf-8")

required = [
    'void wifiEventHandler(WiFiEvent_t event, WiFiEventInfo_t info)',
    'info.wifi_sta_disconnected.reason',
    'void logConfiguredWifiScan()',
    'WiFi.scanNetworks()',
    'WiFi.SSID(index)',
    'WiFi.RSSI(index)',
    'ssid == app_config::wifiSsid',
    'WiFi.scanDelete()',
    'logConfiguredWifiScan();',
    'WiFi.begin(app_config::wifiSsid, app_config::wifiPassword);',
]

missing = [item for item in required if item not in source]
if missing:
    raise SystemExit(
        "Wi-Fi diagnostic contract failed; missing: " + ", ".join(missing)
    )

if 'WiFi.begin("Nokia C3"' in source or '#define WIFI_SSID_VALUE "Nokia C3"' in source:
    raise SystemExit("Production SSID must remain a local credential, not hard-coded in firmware.")

print("Wi-Fi target scan and disconnect diagnostics: PASS")
