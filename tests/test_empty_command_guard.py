from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
VALIDATION = (ROOT / "include" / "command_validation.h").read_text(encoding="utf-8")
THEME = (ROOT / "include" / "guition_theme.h").read_text(encoding="utf-8")

def require(text, needle):
    assert needle in text, f"missing: {needle}"

require(MAIN, '#include "command_validation.h"')
require(MAIN, '#include "guition_theme.h"')
require(MAIN, "command_validation::isBlank(command.c_str())")
require(MAIN, "command_validation::kEmptyCommandMessage")
require(MAIN, '[BLOCKED] empty 3C command rejected before HTTP POST')
require(MAIN, 'web.on("/api/3c", HTTP_POST')
require(MAIN, 'const String rawCommand = web.arg("text");')
require(MAIN, 'const int responseCode = code == 400 ? 400')
require(MAIN, 'theme565(guition_theme::slateBlueGray)')
require(MAIN, 'theme565(guition_theme::skyBlue)')
require(MAIN, 'theme565(guition_theme::mistyBlue)')
require(MAIN, 'theme565(guition_theme::mint)')
require(MAIN, 'guition_theme::panelRadius')
require(MAIN, 'guition_theme::controlRadius')

# The guard must occur before Wi-Fi/HTTP setup.
guard = MAIN.index("if (command_validation::isBlank(command.c_str()))")
wifi = MAIN.index("if (WiFi.status() != WL_CONNECTED)", guard)
http = MAIN.index("HTTPClient http;", guard)
assert guard < wifi < http

# Empty web input is validated before overwriting the runtime buffer.
web = MAIN.index('web.on("/api/3c", HTTP_POST')
raw = MAIN.index('const String rawCommand = web.arg("text");', web)
assert 'app_config::commandBuffer = web.arg("text");' not in MAIN[web:web + 1200]
assert raw < MAIN.index("send3CCommand(rawCommand)", raw)

for color in ("#343645", "#606682", "#9ba2bc", "#f2f0eb", "#3f a7 f3".replace(" ", ""), "#39d19c", "#f4a900", "#f07c40", "#f5075c", "#4867aa"):
    require(THEME.lower(), color)

require(VALIDATION, "inline bool isBlank(const char* command)")
require(VALIDATION, 'kEmptyCommandMessage = "COMANDO VACIO"')

print("empty-command guard + Guition theme contract: PASS")
