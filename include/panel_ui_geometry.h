#pragma once

#include <cstdint>

namespace panel_ui_geometry {

constexpr int kScreenWidth = 480;
constexpr int kScreenHeight = 480;

struct Rect {
  int16_t left;
  int16_t top;
  int16_t right;
  int16_t bottom;

  constexpr Rect(int16_t l, int16_t t, int16_t r, int16_t b)
      : left(l), top(t), right(r), bottom(b) {}

  constexpr int width() const { return right - left; }
  constexpr int height() const { return bottom - top; }

  constexpr bool contains(int x, int y) const {
    return x >= left && x < right && y >= top && y < bottom;
  }
};

constexpr Rect kProbeWslButton{20, 370, 230, 452};
constexpr Rect kSend3CButton{250, 370, 460, 452};

constexpr int kActionGap = kSend3CButton.left - kProbeWslButton.right;
constexpr int kFooterHeight = kScreenHeight - kSend3CButton.bottom;

// Classic 6px GFX font at textSize=2 has a 12px character cell.
// These are conservative bounds for the two fixed labels.
constexpr int kTextSize2CharCell = 12;
constexpr int kProbeWslLabelWidth = 10 * kTextSize2CharCell;
constexpr int kSend3CLabelWidth = 9 * kTextSize2CharCell;

static_assert(kScreenWidth == 480 && kScreenHeight == 480, "panel must remain 480x480");
static_assert(kProbeWslButton.left >= 0 && kProbeWslButton.top >= 0 &&
              kProbeWslButton.right <= kScreenWidth &&
              kProbeWslButton.bottom <= kScreenHeight,
              "PROBAR WSL must remain inside panel");
static_assert(kSend3CButton.left >= 0 && kSend3CButton.top >= 0 &&
              kSend3CButton.right <= kScreenWidth &&
              kSend3CButton.bottom <= kScreenHeight,
              "ENVIAR 3C must remain inside panel");
static_assert(kProbeWslButton.right <= kSend3CButton.left,
              "action buttons must not overlap");
static_assert(kActionGap >= 16, "action gap must remain touch-safe");
static_assert(kFooterHeight >= 20, "keep a visible bottom footer");
static_assert(kProbeWslLabelWidth < kProbeWslButton.width(), "PROBAR WSL label must fit");
static_assert(kSend3CLabelWidth < kSend3CButton.width(), "ENVIAR 3C label must fit");

enum class Route : uint8_t { None, ProbeWsl, Send3C };

constexpr Route route(bool touched, int x, int y) {
  if (!touched) return Route::None;
  if (kProbeWslButton.contains(x, y)) return Route::ProbeWsl;
  if (kSend3CButton.contains(x, y)) return Route::Send3C;
  return Route::None;
}

}  // namespace panel_ui_geometry
