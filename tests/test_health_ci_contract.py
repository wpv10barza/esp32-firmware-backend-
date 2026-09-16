from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
WORKFLOW = (ROOT / ".github" / "workflows" / "firmware-and-pages.yml").read_text(encoding="utf-8")
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
require(WORKFLOW, "validation:compiled_not_physically_flashed", "non-physical artifact marker")
require(WORKFLOW, "backend-health-smoke", "remote backend smoke-test job")
require(WORKFLOW, "physical-e2e-panel", "physical E2E job")
require(WORKFLOW, "self-hosted", "self-hosted hardware runner")
require(WORKFLOW, "panel_4848s040", "physical panel runner label")
require(WORKFLOW, "run_physical_e2e", "manual physical E2E switch")

# Physical workflow evidence must verify both ESP32 boot/display init and backend health.
require(WORKFLOW, "DISPLAY: display->begin() OK", "display boot evidence")
require(WORKFLOW, "Wi-Fi listo:", "Wi-Fi evidence")
require(WORKFLOW, "GET health -> 200", "backend health evidence")
require(WORKFLOW, "curl", "device-local HTTP health evidence")

# Documentation must distinguish remote CI evidence from the physical test.
require(README, "CI demuestra contrato, compilación y, cuando se configura, un smoke test HTTP del backend.", "README CI scope")
require(README, "La validación física E2E solo aparece en GitHub Actions cuando existe un runner self-hosted con el panel conectado por USB.", "README physical scope")
require(SITE, "CI remoto", "site remote CI label")
require(SITE, "runner self-hosted", "site physical runner scope")

print("Health/CI contract: PASS")
print("- Firmware calls GET /api/device/v1/health and requires HTTP 200")
print("- hosted CI does not claim physical USB/panel validation")
print("- optional backend HTTP smoke test is explicit")
print("- physical E2E is isolated to a self-hosted panel runner")
