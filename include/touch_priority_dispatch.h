#pragma once

namespace touch_priority_dispatch {

enum class Route { None, ProbeWsl, Send3C };

struct Rect {
  int left;
  int top;
  int right;
  int bottom;
};

constexpr int kScreenWidth = 480;
constexpr int kScreenHeight = 480;
constexpr Rect kProbeWslButton{20, 370, 230, 452};
constexpr Rect kSend3CButton{250, 370, 460, 452};

constexpr bool contains(const Rect& rect, int x, int y) {
  return x >= rect.left && x < rect.right && y >= rect.top && y < rect.bottom;
}

constexpr Route route(bool touched, int x, int y, bool editorActive) {
  if (!touched || editorActive) return Route::None;
  if (contains(kProbeWslButton, x, y)) return Route::ProbeWsl;
  if (contains(kSend3CButton, x, y)) return Route::Send3C;
  return Route::None;
}

static_assert(kProbeWslButton.right <= kScreenWidth && kSend3CButton.right <= kScreenWidth);
static_assert(kProbeWslButton.bottom <= kScreenHeight && kSend3CButton.bottom <= kScreenHeight);
static_assert(kProbeWslButton.right <= kSend3CButton.left);
}
