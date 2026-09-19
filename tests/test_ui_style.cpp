#include <cassert>
#include <cstdint>

#include "ui_style.h"

int main() {
  static_assert(ui_style::color_slate_blue_gray == 0x343645);
  static_assert(ui_style::color_steel_blue == 0x606682);
  static_assert(ui_style::color_misty_blue == 0x9BA2BC);
  static_assert(ui_style::color_white == 0xF2F0EB);
  static_assert(ui_style::color_light_blue == 0x2FC0FF);
  static_assert(ui_style::color_blue == 0x4C9FFF);
  static_assert(ui_style::color_mint == 0x39D19C);
  static_assert(ui_style::color_green == 0x5CA848);
  static_assert(ui_style::title_text_size == 2);
  static_assert(ui_style::body_text_size == 1);
  static_assert(ui_style::editor_text_size == 2);
  static_assert(ui_style::button_text_size == 2);

  assert(ui_style::red(ui_style::color_slate_blue_gray) == 0x34);
  assert(ui_style::green(ui_style::color_slate_blue_gray) == 0x36);
  assert(ui_style::blue(ui_style::color_slate_blue_gray) == 0x45);
  assert(ui_style::red(ui_style::color_mint) == 0x39);
  assert(ui_style::green(ui_style::color_mint) == 0xD1);
  assert(ui_style::blue(ui_style::color_mint) == 0x9C);
  return 0;
}
