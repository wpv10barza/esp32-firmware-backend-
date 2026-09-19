#include <cassert>
#include "touch_priority_dispatch.h"

int main() {
  using touch_priority_dispatch::Route;
  using touch_priority_dispatch::route;

  assert(route(true, 20, 370, false) == Route::ProbeWsl);
  assert(route(true, 229, 451, false) == Route::ProbeWsl);
  assert(route(true, 250, 370, false) == Route::Send3C);
  assert(route(true, 459, 451, false) == Route::Send3C);

  assert(route(true, 230, 400, false) == Route::None);
  assert(route(true, 249, 400, false) == Route::None);
  assert(route(true, 10, 400, false) == Route::None);
  assert(route(true, 470, 400, false) == Route::None);
  assert(route(true, 240, 460, false) == Route::None);
  assert(route(false, 100, 400, false) == Route::None);
  assert(route(true, 100, 400, true) == Route::None);
  return 0;
}
