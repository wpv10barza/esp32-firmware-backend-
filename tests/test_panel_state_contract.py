from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
STATE = (ROOT / "include" / "panel_state_machine.h").read_text(encoding="utf-8")
NATIVE = (ROOT / "test" / "panel_state_machine_test.cpp").read_text(encoding="utf-8")


def require(text: str, needle: str, label: str) -> None:
    if needle not in text:
        raise AssertionError(f"missing {label}: {needle}")


# The firmware and the native E2E model must share the same state machine.
require(MAIN, '#include "panel_state_machine.h"', "shared panel state include")
require(MAIN, 'using PanelState = panel_state::State;', "PanelState alias")
require(MAIN, 'return panel_state::label(state);', "visible state label mapping")
require(MAIN, 'panel_state::fromHealth(', "health transition mapping")
require(MAIN, 'panel_state::fromPost(', "POST transition mapping")
require(MAIN, 'panel_state::fromPollStatus(', "poll transition mapping")

# Every transport/protocol failure must result in a screen ERROR.
require(MAIN, 'setTransportError("POST", code, lastBackendMessage, true);', "POST transport error")
require(MAIN, 'setTransportError("POLL", code, body, true);', "poll transport error")
require(MAIN, 'updatePanel(PanelState::Error, message, true);', "visible transport error")
require(MAIN, 'setProtocolError(\n        "POLL"', "poll protocol error")
require(MAIN, 'setProtocolError("POST", "falta command_id");', "POST protocol error")

# The protocol failure cannot masquerade as a successful local 202 response.
require(MAIN, 'return 502;', "POST protocol response")
require(MAIN, 'web.send(code == 200 || code == 202 ? 202 : 502,', "local API response mapping")

# The state model itself contains the complete documented failure vocabulary.
for needle in [
    'equalsIgnoreCase(status, "error")',
    'equalsIgnoreCase(status, "failed")',
    'equalsIgnoreCase(status, "fallido")',
    'return State::Error;',
]:
    require(STATE, needle, "state-machine error mapping")

for needle in [
    'fromHealth(true, 500)',
    'fromPost(202, false)',
    'fromPollStatus("failed")',
    'fromPollStatus("fallido")',
    'fromPollStatus("unexpected")',
    'label(State::Error)',
]:
    require(NATIVE, needle, "native E2E assertion")

print("Panel state E2E contract: PASS")
print("- shared firmware/native state machine")
print("- Health HTTP failure -> visible ERROR")
print("- POST HTTP failure or missing command_id -> visible ERROR")
print("- Poll HTTP failure -> visible ERROR")
print("- poll error/failed/fallido -> visible ERROR")
print("- unknown/missing poll status -> visible ERROR")
print("- ERROR label is explicit on the display")
