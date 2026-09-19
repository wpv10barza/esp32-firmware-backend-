#include <cassert>
#include <cstdint>

#include "touch_feedback.h"

namespace {

using touch_feedback::Config;
using touch_feedback::Controller;

void pressRequiresStableDebounce() {
  Controller controller(Config{80, 120});

  assert(!controller.update(true, 7, 0).pressed);
  assert(!controller.update(true, 7, 40).pressed);
  const auto pressed = controller.update(true, 7, 80);
  assert(pressed.pressed);
  assert(pressed.keyIndex == 7);
  assert(controller.highlightedKey(100) == 7);
  assert(controller.highlightedKey(199) == 7);
  assert(controller.highlightedKey(200) == -1);
}

void shortReleaseBounceDoesNotCreateAnotherPress() {
  Controller controller(Config{80, 120});

  assert(!controller.update(true, 3, 0).pressed);
  assert(controller.update(true, 3, 80).pressed);

  // Contact chatter: false for only 20 ms, then true again. Stable touch
  // never transitions to released, so no second press can be emitted.
  assert(!controller.update(false, -1, 100).released);
  assert(!controller.update(false, -1, 120).released);
  assert(!controller.update(true, 3, 130).pressed);
  assert(!controller.update(true, 3, 250).pressed);
}

void NewPressNeedsStableReleaseAndPress() {
  Controller controller(Config{80, 120});

  assert(controller.update(true, 2, 0).pressed == false);
  assert(controller.update(true, 2, 80).pressed);

  assert(!controller.update(false, -1, 100).released);
  const auto released = controller.update(false, -1, 180);
  assert(released.released);
  assert(!released.pressed);

  assert(!controller.update(true, 5, 200).pressed);
  const auto pressed = controller.update(true, 5, 280);
  assert(pressed.pressed);
  assert(pressed.keyIndex == 5);
}

void HeldTouchNeverRepeatsAfterLongDuration() {
  Controller controller(Config{80, 120});

  assert(controller.update(true, 1, 0).pressed == false);
  assert(controller.update(true, 1, 80).pressed);

  for (uint32_t now = 100; now < 2000; now += 100) {
    assert(!controller.update(true, 1, now).pressed);
  }
}

void MillisRolloverKeepsDebounceCorrect() {
  Controller controller(Config{80, 120});
  const uint32_t nearWrap = 0xFFFFFFF0u;

  assert(!controller.update(true, 4, nearWrap).pressed);
  const auto pressed = controller.update(true, 4, 0x00000040u);
  assert(pressed.pressed);
  assert(pressed.keyIndex == 4);
}

void FingerCanMoveBeforePressWithoutGeneratingMultipleEvents() {
  Controller controller(Config{80, 120});

  assert(!controller.update(true, 10, 0).pressed);
  assert(!controller.update(true, 11, 40).pressed);
  const auto pressed = controller.update(true, 11, 80);
  assert(pressed.pressed);
  assert(pressed.keyIndex == 11);
}

}  // namespace

int main() {
  pressRequiresStableDebounce();
  shortReleaseBounceDoesNotCreateAnotherPress();
  NewPressNeedsStableReleaseAndPress();
  HeldTouchNeverRepeatsAfterLongDuration();
  MillisRolloverKeepsDebounceCorrect();
  FingerCanMoveBeforePressWithoutGeneratingMultipleEvents();
  return 0;
}
