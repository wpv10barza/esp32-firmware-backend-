#pragma once

#include <cstdint>

namespace touch_feedback {

// Millisecond durations are intentionally small enough to keep the editor
// responsive while filtering GT911 contact bounce.
struct Config {
  uint32_t debounceMs = 80;
  uint32_t highlightMs = 120;
};

struct Event {
  bool pressed = false;
  bool released = false;
  int keyIndex = -1;
};

// Debounces the raw GT911 touch stream and emits at most one press event for a
// stable touch gesture. A short false sample during a held touch does not
// re-arm the keyboard, which prevents repeated key insertion from bounce.
class Controller {
 public:
  explicit Controller(Config config = {}) : config_(config) {}

  Event update(bool rawTouched, int rawKeyIndex, uint32_t nowMs) {
    Event event;

    if (rawTouched != candidateTouched_) {
      candidateTouched_ = rawTouched;
      candidateSinceMs_ = nowMs;
      if (candidateTouched_) {
        candidateKeyIndex_ = rawKeyIndex;
      } else {
        candidateKeyIndex_ = -1;
      }
    } else if (candidateTouched_) {
      // Track the latest key under the finger without generating another
      // press event until the current gesture is released and re-armed.
      candidateKeyIndex_ = rawKeyIndex;
    }

    if (candidateTouched_ == stableTouched_) return event;
    if (static_cast<uint32_t>(nowMs - candidateSinceMs_) < config_.debounceMs) {
      return event;
    }

    stableTouched_ = candidateTouched_;
    if (stableTouched_) {
      event.pressed = true;
      event.keyIndex = candidateKeyIndex_;
      if (event.keyIndex >= 0 && config_.highlightMs > 0) {
        highlightedKeyIndex_ = event.keyIndex;
        highlightStartedMs_ = nowMs;
      } else {
        highlightedKeyIndex_ = -1;
      }
    } else {
      event.released = true;
      candidateKeyIndex_ = -1;
    }
    return event;
  }

  int highlightedKey(uint32_t nowMs) const {
    if (highlightedKeyIndex_ < 0 || config_.highlightMs == 0) return -1;
    if (static_cast<uint32_t>(nowMs - highlightStartedMs_) >= config_.highlightMs) {
      return -1;
    }
    return highlightedKeyIndex_;
  }

  bool isStableTouched() const { return stableTouched_; }

  void reset() {
    candidateTouched_ = false;
    stableTouched_ = false;
    candidateKeyIndex_ = -1;
    highlightedKeyIndex_ = -1;
    candidateSinceMs_ = 0;
    highlightStartedMs_ = 0;
  }

 private:
  Config config_;
  bool candidateTouched_ = false;
  bool stableTouched_ = false;
  int candidateKeyIndex_ = -1;
  int highlightedKeyIndex_ = -1;
  uint32_t candidateSinceMs_ = 0;
  uint32_t highlightStartedMs_ = 0;
};

}  // namespace touch_feedback
