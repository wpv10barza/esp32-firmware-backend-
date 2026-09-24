#pragma once

#include <cstdint>

namespace guition_monitor_theme {

struct Rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

constexpr Rgb kBackground{0x34, 0x36, 0x45};   // color_slate_blue_gray
constexpr Rgb kSurface{0x0D, 0x0D, 0x0D};      // color_black
constexpr Rgb kSteelBlue{0x60, 0x66, 0x82};    // color_steel_blue
constexpr Rgb kMistyBlue{0x9B, 0xA2, 0xBC};    // color_misty_blue
constexpr Rgb kWhite{0xF2, 0xF0, 0xEB};        // color_white
constexpr Rgb kSkyBlue{0x3F, 0xA7, 0xF3};      // color_sky_blue
constexpr Rgb kLightBlue{0x2F, 0xC0, 0xFF};
constexpr Rgb kMint{0x39, 0xD1, 0x9C};         // color_mint
constexpr Rgb kLightMint{0x66, 0xDC, 0xB3};
constexpr Rgb kYellow{0xE7, 0xC1, 0x2C};
constexpr Rgb kAmber{0xF4, 0xA9, 0x00};
constexpr Rgb kOrange{0xF0, 0x7C, 0x40};
constexpr Rgb kCrimson{0xF5, 0x07, 0x5C};
constexpr Rgb kDarkBlue{0x48, 0x67, 0xAA};

}  // namespace guition_monitor_theme
