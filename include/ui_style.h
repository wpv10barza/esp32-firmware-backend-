#pragma once

#include <cstdint>

namespace ui_style {

// Palette ported from alaltitov/Guition-ESP32-S3-4848S040
// branch 2026.8.2, src/common/colors.yaml.
constexpr uint32_t color_slate_blue_gray = 0x343645;
constexpr uint32_t color_steel_blue = 0x606682;
constexpr uint32_t color_misty_blue = 0x9BA2BC;
constexpr uint32_t color_black = 0x0D0D0D;
constexpr uint32_t color_dark_gray = 0x333333;
constexpr uint32_t color_gray = 0x666666;
constexpr uint32_t color_light_gray = 0x999999;
constexpr uint32_t color_white = 0xF2F0EB;
constexpr uint32_t color_red = 0xFF0000;
constexpr uint32_t color_crimson = 0xF5075C;
constexpr uint32_t color_light_blue = 0x2FC0FF;
constexpr uint32_t color_blue = 0x4C9FFF;
constexpr uint32_t color_yellow = 0xE7C12C;
constexpr uint32_t color_light_yellow = 0xF0D45C;
constexpr uint32_t color_amber = 0xF4A900;
constexpr uint32_t color_mint = 0x39D19C;
constexpr uint32_t color_light_mint = 0x66DCB3;
constexpr uint32_t color_light_green = 0x00FF00;
constexpr uint32_t color_green = 0x5CA848;
constexpr uint32_t color_orange = 0xF07C40;
constexpr uint32_t color_deep_orange = 0xFF6600;
constexpr uint32_t color_violet = 0x926BC7;
constexpr uint32_t color_dark_blue = 0x4867AA;
constexpr uint32_t color_deep_purple = 0x543D72;

// LVGL reference typography uses Nunito-SemiBold at 16/18/20/30/36/48...
// Arduino-GFX keeps its resident bitmap font here; these sizes preserve the
// same semantic hierarchy without bundling a second font engine.
constexpr uint8_t title_text_size = 2;
constexpr uint8_t body_text_size = 1;
constexpr uint8_t editor_text_size = 2;
constexpr uint8_t button_text_size = 2;
constexpr uint8_t compact_key_text_size = 1;
constexpr uint8_t key_text_size = 2;

constexpr uint8_t red(uint32_t rgb) {
  return static_cast<uint8_t>((rgb >> 16) & 0xFFU);
}

constexpr uint8_t green(uint32_t rgb) {
  return static_cast<uint8_t>((rgb >> 8) & 0xFFU);
}

constexpr uint8_t blue(uint32_t rgb) {
  return static_cast<uint8_t>(rgb & 0xFFU);
}

}  // namespace ui_style
