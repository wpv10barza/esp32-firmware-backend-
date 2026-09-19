#pragma once

#include <stdint.h>

namespace panel_theme {

// Palette ported from the Guition ESP32-S3-4848S040 reference:
// color_black, color_slate_blue_gray, color_steel_blue, color_misty_blue,
// color_white, color_mint, color_green, color_amber and color_red.
struct Rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

constexpr Rgb pageBackground{0x0D, 0x0D, 0x0D};   // #0d0d0d
constexpr Rgb surface{0x34, 0x36, 0x45};          // #343645
constexpr Rgb steelBlue{0x60, 0x66, 0x82};        // #606682
constexpr Rgb mistyBlue{0x9B, 0xA2, 0xBC};        // #9ba2bc
constexpr Rgb primaryWhite{0xF2, 0xF0, 0xEB};     // #f2f0eb
constexpr Rgb blue{0x4C, 0x9F, 0xFF};             // #4c9fff
constexpr Rgb mint{0x39, 0xD1, 0x9C};             // #39d19c
constexpr Rgb green{0x5C, 0xA8, 0x48};            // #5ca848
constexpr Rgb amber{0xF4, 0xA9, 0x00};            // #f4a900
constexpr Rgb red{0xFF, 0x00, 0x00};              // #ff0000
constexpr Rgb orange{0xF0, 0x7C, 0x40};           // #f07c40

constexpr int cardX = 20;
constexpr int cardY = 74;
constexpr int cardWidth = 440;
constexpr int cardHeight = 262;
constexpr int cardRadius = 20;

constexpr int actionY = 370;
constexpr int actionHeight = 82;
constexpr int actionGap = 20;
constexpr int actionWidth = 210;

constexpr int editorFieldX = 8;
constexpr int editorFieldY = 42;
constexpr int editorFieldWidth = 464;
constexpr int editorFieldHeight = 72;
constexpr int editorFieldRadius = 20;

constexpr int smallRadius = 7;
constexpr int buttonRadius = 16;

}  // namespace panel_theme
