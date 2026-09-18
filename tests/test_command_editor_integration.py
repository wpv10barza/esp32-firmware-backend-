from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
APP = (ROOT / "include" / "app_config.h").read_text(encoding="utf-8")

def require(text, needle):
    assert needle in text, f"missing: {needle}"

require(MAIN, '#include "command_buffer.h"')
require(MAIN, '#include "command_text_viewport.h"')
require(MAIN, '#include "virtual_keyboard.h"')
require(MAIN, "CommandBuffer<kCommandCapacity> commandBuffer;")
require(MAIN, "commandBuffer.set(app_config::defaultCommand);")
require(MAIN, "send3CCommand(commandBuffer.c_str());")
require(MAIN, "virtual_keyboard::hitTest")
require(MAIN, "command_text_viewport::compute")
assert "send3CCommand(app_config::defaultCommand);" not in MAIN

require(APP, "DEFAULT_3C_COMMAND_VALUE")
print("runtime command editor regression: PASS")
