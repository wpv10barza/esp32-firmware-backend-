#pragma once

#include <cstdint>

namespace ui_style {

constexpr uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue) {
  return static_cast<uint16_t>(
      ((static_cast<uint16_t>(red) & 0xF8U) << 8) |
      ((static_cast<uint16_t>(green) & 0xFCU) << 3) |
      (static_cast<uint16_t>(blue) >> 3));
}

// Ported visual tokens from alaltitov/Guition-ESP32-S3-4848S040.
// Source palette: slate blue-gray, steel blue, misty blue, warm white and sky blue.
constexpr uint16_t kBackground = rgb565(0x34, 0x36, 0x45);      // #343645
constexpr uint16_t kSurface = rgb565(0x60, 0x66, 0x82);          // #606682
constexpr uint16_t kSecondaryText = rgb565(0x9B, 0xA2, 0xBC);    // #9BA2BC
constexpr uint16_t kPrimaryText = rgb565(0xF2, 0xF0, 0xEB);      // #F2F0EB
constexpr uint16_t kAccent = rgb565(0x3F, 0xA7, 0xF3);           // #3FA7F3
constexpr uint16_t kSuccess = rgb565(0x5C, 0xA8, 0x48);          // #5CA848
constexpr uint16_t kWarning = rgb565(0xE7, 0xC1, 0x2C);          // #E7C12C
constexpr uint16_t kDanger = rgb565(0xF5, 0x07, 0x5C);           // #F5075C

// Arduino_GFX classic font scaling used as the closest portable equivalent
// while preserving the Guition visual hierarchy (large title/status, compact detail).
constexpr uint8_t kTitleTextSize = 3;
constexpr uint8_t kStatusTextSize = 3;
constexpr uint8_t kCommandTextSize = 2;
constexpr uint8_t kDetailTextSize = 2;
constexpr uint8_t kButtonTextSize = 2;
constexpr uint8_t kKeyboardTextSize = 2;
constexpr uint8_t kSmallKeyboardTextSize = 1;

constexpr int kFieldX = 8;
constexpr int kFieldY = 42;
constexpr int kFieldWidth = 464;
constexpr int kFieldHeight = 72;
constexpr int kFieldInnerX = 12;
constexpr int kFieldInnerY = 48;
constexpr int kFieldInnerWidth = 456;
constexpr int kFieldInnerHeight = 60;

constexpr int kCursorY = 57;
constexpr int kCursorHeight = 28;

constexpr int kKeyboardY = 216;
constexpr int kEditorControlY = 172;
constexpr int kEditorControlHeight = 36;

}  // namespace ui_style
