from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
STYLE = (ROOT / "include" / "ui_style.h").read_text(encoding="utf-8")


def require(text: str, needle: str) -> None:
    if needle not in text:
        raise AssertionError(f"missing required UI contract: {needle}")


# Guition visual tokens copied from src/common/colors.yaml.
for token in [
    "0x34, 0x36, 0x45",
    "0x60, 0x66, 0x82",
    "0x9B, 0xA2, 0xBC",
    "0xF2, 0xF0, 0xEB",
    "0x3F, 0xA7, 0xF3",
    "0x5C, 0xA8, 0x48",
    "0xE7, 0xC1, 0x2C",
    "0xF5, 0x07, 0x5C",
]:
    require(STYLE, token)

require(MAIN, '#include "ui_style.h"')
# The Guition reference uses a black page background and slate-blue-gray surfaces.
require(STYLE, "kBackground = rgb565(0x0D, 0x0D, 0x0D)")
require(STYLE, "kSurface = rgb565(0x34, 0x36, 0x45)")
require(MAIN, "void clearScreenForMode()")
require(MAIN, "void drawEditorText()")
require(MAIN, "void drawEditorStatic()")
require(MAIN, "void drawEditorKeyboard()")
require(MAIN, "void enterCommandEditor()")
require(MAIN, "void exitCommandEditor()")

# Incremental typing redraw must stay inside the editor field; it cannot clear
# the complete 480x480 framebuffer.
start = MAIN.index("void drawEditorText()")
end = MAIN.index("void drawEditorKeyboard()", start)
editor_text = MAIN[start:end]
if "fillScreen(" in editor_text:
    raise AssertionError("drawEditorText() must never call fillScreen()")

# Mode entry is the only place where editor mode may clear the full screen.
require(MAIN, "clearScreenForMode();")
require(MAIN, "drawEditorStatic();")
require(MAIN, "drawEditorText();")

# Character and cursor operations use partial field redraws.
for needle in [
    "commandBuffer.insert(key.definition.label);",
    "commandBuffer.backspace();",
    "commandBuffer.moveLeft();",
    "commandBuffer.deleteForward();",
    "drawEditorText();",
]:
    require(MAIN, needle)

# Background must be stable while editing; panel state updates are deferred
# visually until the editor is closed so network polling cannot flicker typing.
require(MAIN, "if (!commandEditorOpen && (changed || detailChanged)) drawPanel();")

require(MAIN, "display->fillScreen(ui_style::kBackground);")

# Old per-state full-screen backgrounds must not survive the style migration.
for legacy in [
    "color565(8, 18, 30)",
    "color565(10, 18, 38)",
    "color565(18, 22, 30)",
]:
    if legacy in MAIN:
        raise AssertionError(f"legacy full-screen color remains: {legacy}")

print("Guition UI style + partial redraw contract: PASS")
print("- palette tokens present")
print("- editor typing avoids full-screen fill")
print("- mode transitions own full-screen clears")
print("- state polling cannot redraw over active typing")
