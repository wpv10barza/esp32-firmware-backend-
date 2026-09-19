#pragma once

#include <cstdint>

namespace guition_theme {

// Visual reference: alaltitov/Guition-ESP32-S3-4848S040 @ 2026.8.2
// Source palette from src/common/colors.yaml.
struct Rgb {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

constexpr Rgb slateBlueGray{0x34, 0x36, 0x45};  // #343645
constexpr Rgb steelBlue{0x60, 0x66, 0x82};      // #606682
constexpr Rgb mistyBlue{0x9B, 0xA2, 0xBC};      // #9BA2BC
constexpr Rgb white{0xF2, 0xF0, 0xEB};          // #F2F0EB
constexpr Rgb skyBlue{0x3F, 0xA7, 0xF3};        // #3FA7F3
constexpr Rgb lightBlue{0x2F, 0xC0, 0xFF};      // #2FC0FF
constexpr Rgb mint{0x39, 0xD1, 0x9C};           // #39D19C
constexpr Rgb green{0x5C, 0xA8, 0x48};          // #5CA848
constexpr Rgb amber{0xF4, 0xA9, 0x00};          // #F4A900
constexpr Rgb orange{0xF0, 0x7C, 0x40};         // #F07C40
constexpr Rgb crimson{0xF5, 0x07, 0x5C};        // #F5075C
constexpr Rgb darkBlue{0x48, 0x67, 0xAA};       // #4867AA

constexpr int panelRadius = 20;
constexpr int controlRadius = 16;

}  // namespace guition_theme
