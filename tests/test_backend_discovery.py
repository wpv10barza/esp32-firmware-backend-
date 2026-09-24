from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
CONFIG = (ROOT / "include" / "app_config.h").read_text(encoding="utf-8")
WORKFLOW = (ROOT / ".github" / "workflows" / "firmware-and-pages.yml").read_text(
    encoding="utf-8"
)


def require(text: str, needle: str, label: str) -> None:
    if needle not in text:
        raise AssertionError(f"missing {label}: {needle}")


# No fixed backend IP/loopback endpoint.
if "BACKEND_IP" in MAIN:
    raise AssertionError("BACKEND_IP must not exist in firmware")
if "assistantBaseUrl" in MAIN:
    raise AssertionError("runtime endpoint must not use assistantBaseUrl")

for source_name, source in [("main.cpp", MAIN), ("app_config.h", CONFIG)]:
    private_ips = re.findall(r"192\.168\.\d+\.\d+", source)
    if private_ips:
        raise AssertionError(f"fixed private IP remains in {source}: {private_ips}")


# mDNS discovery contract.
require(MAIN, "#include <ESPmDNS.h>", "ESPmDNS include")
require(MAIN, "#include <Preferences.h>", "Preferences include")
require(MAIN, 'constexpr char kBackendMdnsService[] = "3c";', "mDNS service")
require(MAIN, 'constexpr char kBackendMdnsProtocol[] = "tcp";', "mDNS protocol")
require(
    MAIN,
    "MDNS.queryService(kBackendMdnsService, kBackendMdnsProtocol)",
    "mDNS service query",
)
require(MAIN, "MDNS.queryHost(queryHost, 2000)", "mDNS hostname resolution")
require(MAIN, "MDNS.port(index)", "discovered backend port")
require(MAIN, "void stopMdns()", "mDNS lifecycle cleanup")

if "MDNS.address(index)" in MAIN:
    raise AssertionError(
        "MDNS.address(index) is incompatible with the CI ESPmDNS 2.0.0 package"
    )


# NVS namespace must be creatable on first boot.
require(
    MAIN,
    'constexpr char kBackendPrefsNamespace[] = "backend";',
    "NVS namespace",
)
require(
    MAIN,
    "backendPrefs.begin(kBackendPrefsNamespace, false)",
    "NVS read/write open",
)
if "backendPrefs.begin(kBackendPrefsNamespace, true)" in MAIN:
    raise AssertionError("backend NVS namespace must not be opened read-only")
require(MAIN, "backendPrefs.end()", "NVS close")
require(MAIN, "NVS cache write incomplete", "NVS write verification")


# Cached endpoint remains first choice.
setup_pos = MAIN.index("void setup()")
setup = MAIN[setup_pos:]
load_pos = setup.index("loadBackendEndpointFromNvs();")
wifi_pos = setup.index("connectWifi();")
if load_pos >= wifi_pos:
    raise AssertionError("NVS endpoint must load before Wi-Fi connection")


health_pos = MAIN.index("bool checkBackendHealth()")
health_end = MAIN.index("int send3CCommand", health_pos)
health = MAIN[health_pos:health_end]

require(health, "if (WiFi.status() != WL_CONNECTED)", "health Wi-Fi guard")
require(health, "if (!backendEndpoint.valid())", "cache-first health decision")
require(health, "discoverBackendEndpoint();", "initial discovery")
require(health, "const BackendEndpoint failedEndpoint = backendEndpoint;", "failed endpoint snapshot")
require(health, "rediscovering _%s._%s", "rediscovery log")
post_failure = health.split("if (checkBackendHealthOnce()) return true;", 1)[1]
require(post_failure, "if (!discoverBackendEndpoint()) return false;", "post-failure rediscovery")
require(health, "return checkBackendHealthOnce();", "post-rediscovery health retry")


# Runtime endpoint is used by command and polling paths.
require(MAIN, 'endpoint("/api/device/v1/commands")', "command endpoint")
require(
    MAIN,
    'endpoint("/api/device/v1/commands/" + lastCommandId)',
    "poll endpoint",
)

# CI must run this contract.
require(
    WORKFLOW,
    "python tests/test_backend_discovery.py",
    "GitHub Actions backend discovery test",
)

print("Backend discovery contract: PASS")
print("- mDNS _3c._tcp -> hostname resolution -> IP:port")
print("- no fixed private IP")
print("- NVS cache opens read/write and verifies writes")
print("- health is gated by active Wi-Fi")
print("- failed health triggers rediscovery and one retry")
