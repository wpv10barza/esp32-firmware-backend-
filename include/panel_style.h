#pragma once

#include <cstdint>

namespace panel_style {

// Visual language aligned to Guition-ESP32-S3-4848S040 src/common/colors.yaml.
// RGB values are kept as data so the renderer remains independent of LVGL.

struct Rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

constexpr Rgb background{0x34, 0x36, 0x45};   // color_slate_blue_gray
constexpr Rgb title{0x3F, 0xA7, 0xF3};        // color_sky_blue
constexpr Rgb secondary{0x9B, 0xA2, 0xBC};   // color_misty_blue
constexpr Rgb text{0xF2, 0xF0, 0xEB};        // color_white
constexpr Rgb fieldBorder{0x60, 0x66, 0x82}; // color_steel_blue
constexpr Rgb keyFill{0x60, 0x66, 0x82};
constexpr Rgb keyBorder{0x9B, 0xA2, 0xBC};
constexpr Rgb accent{0x39, 0xD1, 0x9C};      // color_mint
constexpr Rgb enterFill{0x39, 0xD1, 0x9C};

constexpr uint8_t titleTextSize = 2;
constexpr uint8_t keyTextSize = 2;
constexpr uint8_t secondaryTextSize = 1;
constexpr uint8_t keyRadius = 7;
constexpr uint8_t buttonRadius = 16;
constexpr Rgb buttonBorder = keyBorder;

}  // namespace panel_style
