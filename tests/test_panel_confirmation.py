from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCE = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
CONFIG = (ROOT / "include" / "app_config.h").read_text(encoding="utf-8")


def test_validation_states_are_rendered():
    for token in ("Pending", "Applied", "Rejected", "Error"):
        assert f"PanelState::{token}" in SOURCE
    for label in ("PENDIENTE", "APLICADO", "RECHAZADO", "ERROR"):
        assert label in SOURCE


def test_panel_confirmation_gate_exists():
    assert "PanelState::AwaitingConfirmation" in SOURCE
    assert "String preparedCommand;" in SOURCE
    assert '"CONFIRMAR"' in SOURCE
    assert '"CANCELAR"' in SOURCE
    assert "void prepare3CCommand" in SOURCE
    assert "void cancelPreparedCommand" in SOURCE
    assert "int postPreparedCommand" in SOURCE


def test_send_button_only_prepares_command():
    touch = SOURCE.split("void handleTouch()", 1)[1]
    assert "prepare3CCommand(app_config::defaultCommand);" in touch
    assert "postPreparedCommand();" in touch
    assert touch.index("prepare3CCommand(app_config::defaultCommand);") < touch.index("postPreparedCommand();")


def test_web_prepare_does_not_post_directly():
    route = SOURCE.split('web.on("/api/3c", HTTP_POST, [] {', 1)[1]
    route = route.split('web.onNotFound', 1)[0]
    assert "prepare3CCommand(text);" in route
    assert "postPreparedCommand();" not in route
    assert "confirmation_required" in route


def test_required_api_contract_is_unchanged():
    assert '/api/device/v1/health' in SOURCE
    assert '/api/device/v1/commands' in SOURCE
    assert '/api/device/v1/commands/" + lastCommandId' in SOURCE
    assert "commandPollMs = 2500UL" in CONFIG


def test_hardware_contract_is_preserved():
    assert 'constexpr uint8_t kTouchAddress = 0x5D' in SOURCE
    assert 'kScreenWidth = 480' in SOURCE
    assert 'kScreenHeight = 480' in SOURCE
    assert 'st7701_type8_init_operations' in SOURCE
    assert 'Arduino_RGB_Display' in SOURCE


def test_touch_debounce_is_present():
    assert "kTouchDebounceMs" in SOURCE
    assert "lastTouchAction" in SOURCE
    assert "millis() - lastTouchAction >= kTouchDebounceMs" in SOURCE
