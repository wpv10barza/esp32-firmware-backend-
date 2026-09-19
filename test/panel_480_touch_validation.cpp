#include <cassert>

#include "panel_ui_geometry.h"

int main() {
  using panel_ui_geometry::Route;

  static_assert(panel_ui_geometry::kScreenWidth == 480);
  static_assert(panel_ui_geometry::kScreenHeight == 480);
  static_assert(panel_ui_geometry::kProbeWslButton.left >= 0);
  static_assert(panel_ui_geometry::kProbeWslButton.right <= 480);
  static_assert(panel_ui_geometry::kSend3CButton.left >= 0);
  static_assert(panel_ui_geometry::kSend3CButton.right <= 480);
  static_assert(panel_ui_geometry::kProbeWslButton.right <= panel_ui_geometry::kSend3CButton.left);
  static_assert(panel_ui_geometry::kFooterHeight >= 20);

  const int y = panel_ui_geometry::kProbeWslButton.top + 10;

  assert(panel_ui_geometry::route(true, panel_ui_geometry::kProbeWslButton.left, y) == Route::ProbeWsl);
  assert(panel_ui_geometry::route(true, panel_ui_geometry::kProbeWslButton.right - 1, y) == Route::ProbeWsl);
  assert(panel_ui_geometry::route(true, panel_ui_geometry::kProbeWslButton.right, y) == Route::None);

  assert(panel_ui_geometry::route(true, panel_ui_geometry::kSend3CButton.left, y) == Route::Send3C);
  assert(panel_ui_geometry::route(true, panel_ui_geometry::kSend3CButton.right - 1, y) == Route::Send3C);
  assert(panel_ui_geometry::route(true, panel_ui_geometry::kSend3CButton.right, y) == Route::None);

  assert(panel_ui_geometry::route(true, panel_ui_geometry::kProbeWslButton.right + 1, y) == Route::None);
  assert(panel_ui_geometry::route(true, 10, panel_ui_geometry::kSend3CButton.bottom) == Route::None);
  assert(panel_ui_geometry::route(true, 10, 479) == Route::None);
  assert(panel_ui_geometry::route(true, 480, y) == Route::None);
  assert(panel_ui_geometry::route(false, 100, y) == Route::None);

  return 0;
}
