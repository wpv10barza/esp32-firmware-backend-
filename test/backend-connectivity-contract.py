from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
config = (ROOT / "include" / "app_config.h").read_text(encoding="utf-8")
source = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")

url = re.search(r'#define ASSISTANT_BASE_URL_VALUE\s+"([^"]+)"', config).group(1)
assert re.fullmatch(r"http://(?:\d{1,3}\.){3}\d{1,3}:3000", url)
assert not re.search(r"localhost|127\.0\.0\.1", url, re.I)
assert re.search(r'app\.listen\(PORT,\s*[\'\"]0\.0\.0\.0[\'\"]', source)
for path in ("/api/device/v1/health", "/api/device/v1/commands", "/api/device/v1/commands/"):
    assert path in source
print(f"PASS URL={url}")
print("PASS listen=0.0.0.0:3000")
print("PASS device endpoints present")
print("NOTE physical LAN IP/firewall still require local validation")
