from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MAIN = (ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
THEME = (ROOT / "include" / "guition_theme.h").read_text(encoding="utf-8")

assert "alaltitov/Guition-ESP32-S3-4848S040 @ 2026.8.2" in THEME
assert "slateBlueGray" in MAIN
assert "skyBlue" in MAIN
assert "mistyBlue" in MAIN
assert "panelRadius" in MAIN
assert "controlRadius" in MAIN
assert "#343645" in THEME
assert "#606682" in THEME
assert "#9ba2bc" in THEME.lower()
assert "#f2f0eb" in THEME.lower()
assert "#3fa7f3" in THEME.lower()
assert "#39d19c" in THEME.lower()
assert "#f4a900" in THEME.lower()
assert "#f07c40" in THEME.lower()
assert "#f5075c" in THEME.lower()
assert "#4867aa" in THEME.lower()

print("Guition 480x480 visual theme contract: PASS")
