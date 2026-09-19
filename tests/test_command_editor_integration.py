from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
APP = (ROOT / "include" / "app_config.h").read_text(encoding="utf-8")

def require(text, needle):
    assert needle in text, f"missing: {needle}"

require(APP, "static String commandBuffer = DEFAULT_3C_COMMAND_VALUE;")

# The input subsystem must be real runtime components, not only a rendered keyboard.
for include in [
    '#include "command_buffer.h"',
    '#include "command_text_viewport.h"',
    '#include "virtual_keyboard.h"',
]:
    require(MAIN, include)

require(MAIN, "CommandBuffer<kCommandCapacity> commandBuffer;")
require(MAIN, "commandBuffer.set(app_config::commandBuffer.c_str());")
require(MAIN, 'commandEditorOpen = true;')
require(MAIN, "virtual_keyboard::hitTest")
require(MAIN, "command_text_viewport::compute")
require(MAIN, "placeCursorFromTouch(sample.x)")

# Active-editor touch must route to editing operations.
for operation in [
    "commandBuffer.insert(key.definition.label);",
    "commandBuffer.backspace();",
    "commandBuffer.insert(' ');",
    "commandBuffer.deleteForward();",
    "commandBuffer.moveLeft();",
    "commandBuffer.moveRight();",
]:
    require(MAIN, operation)

# ENTER commits the runtime buffer into the established request path.
require(MAIN, "case KeyKind::Enter:")
require(MAIN, "app_config::commandBuffer = commandBuffer.c_str();")
require(MAIN, "send3CCommand(app_config::commandBuffer);")

# The default command may seed the editor, but ENVIAR 3C must never bypass the buffer.
assert "send3CCommand(app_config::defaultCommand);" not in MAIN

# The editor keeps its actual text inside the horizontal viewport and exposes
# both alpha and numeric/symbol keyboard modes.
require(MAIN, "keyboardMode = virtual_keyboard::KeyboardMode::Alpha;")
require(MAIN, "virtual_keyboard::KeyboardMode::NumericSymbols")
require(MAIN, 'sample.y >= 216')
require(MAIN, 'sample.y >= 42 && sample.y < 114')

print("runtime command editor regression: PASS")
print("- editable field + horizontal cursor viewport present")
print("- touch routes to keyboard/control editing primitives")
print("- runtime CommandBuffer feeds the existing ENVIAR 3C request path")
