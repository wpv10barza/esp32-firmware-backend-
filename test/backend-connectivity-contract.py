from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
config = (ROOT / "include" / "app_config.h").read_text(encoding="utf-8")
source = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")

base_match = re.search(r'#define ASSISTANT_BASE_URL_VALUE\s+"([^"]+)"', config)
assert base_match, "ASSISTANT_BASE_URL_VALUE missing"
base_url = base_match.group(1)
assert re.fullmatch(r"http://(?:\d{1,3}\.){3}\d{1,3}:3000", base_url), base_url
assert "127.0.0.1" not in base_url and "localhost" not in base_url.lower(), base_url
assert re.search(r'app\.listen\(PORT,\s*[\'\"]0\.0\.0\.0[\'\"]', source), "backend must listen on 0.0.0.0"
for path in ("/api/device/v1/health", "/api/device/v1/commands", "/api/device/v1/commands/"):
    assert path in source, f"missing endpoint path: {path}"
print(f"PASS: ESP32 backend URL={base_url}")
print("PASS: backend listen=0.0.0.0:3000")
print("PASS: device API paths present")
print("NOTE: CI cannot validate the user's physical LAN IP or firewall.")
