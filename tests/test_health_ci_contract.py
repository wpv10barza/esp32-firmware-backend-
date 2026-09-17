from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
WORKFLOW = (ROOT / ".github" / "workflows" / "firmware-and-pages.yml").read_text(encoding="utf-8")
PHYSICAL = (ROOT / "scripts" / "physical-e2e.sh").read_text(encoding="utf-8")
README = (ROOT / "README.md").read_text(encoding="utf-8")
SITE = (ROOT / "site" / "index.html").read_text(encoding="utf-8")


def require(text: str, needle: str, label: str) -> None:
    if needle not in text:
        raise AssertionError(f"missing {label}: {needle}")


# Firmware-to-backend health contract must stay explicit and use GET.
require(MAIN, 'endpoint("/api/device/v1/health")', "device health endpoint")
require(MAIN, "const int code = http.GET();", "GET health request")
require(MAIN, "backendAvailable = code == 200;", "HTTP 200 health gate")
require(MAIN, 'Serial.printf("GET health -> %d %s\\n", code, lastBackendMessage.c_str());', "health diagnostic")

# Hosted CI must never claim that USB/panel hardware was physically tested.
require(WORKFLOW, '"validation":"compiled_not_physically_flashed"', "non-physical artifact marker")
require(WORKFLOW, "backend-health-smoke:", "remote backend smoke-test job")
require(WORKFLOW, "physical-e2e-panel:", "physical E2E job")
require(WORKFLOW, "self-hosted", "self-hosted hardware runner")
require(WORKFLOW, "panel-4848s040", "physical panel runner label")
require(WORKFLOW, "run_physical_e2e", "manual physical E2E switch")

# Physical workflow evidence must verify USB flash plus boot, network and device HTTP health.
require(PHYSICAL, "DISPLAY: display->begin() OK", "display boot evidence")
require(PHYSICAL, "Wi-Fi listo:", "Wi-Fi evidence")
require(PHYSICAL, "GET health -> 200", "backend health evidence")
require(PHYSICAL, "curl", "device-local HTTP health evidence")
require(PHYSICAL, "wifi=true", "ESP32 health Wi-Fi evidence")
require(PHYSICAL, "backend=true", "ESP32 health backend evidence")

# Documentation must distinguish remote CI evidence from physical hardware evidence.
for needle, label in [
    ("CI remoto", "README remote CI section"),
    ("backend-health-smoke", "README backend smoke job"),
    ("BACKEND_HEALTH_URL", "README backend health secret"),
    ("validación física E2E", "README physical E2E section"),
    ("self-hosted", "README self-hosted scope"),
    ("conectado por USB", "README USB requirement"),
]:
    require(README, needle, label)

require(SITE, "CI remoto", "site remote CI label")
require(SITE, "runner self-hosted", "site physical runner scope")

print("Health/CI contract: PASS")
print("- Firmware calls GET /api/device/v1/health and requires HTTP 200")
print("- hosted CI does not claim physical USB/panel validation")
print("- optional backend HTTP smoke test is explicit")
print("- physical E2E is isolated to a self-hosted panel runner")
