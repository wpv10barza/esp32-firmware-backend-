#pragma once

#include "command_buffer.h"

template <std::size_t Capacity>
class CommandEditSession {
 public:
  void begin(const CommandBuffer<Capacity>& current) {
    saved_.set(current.c_str());
    active_ = true;
  }

  bool save(CommandBuffer<Capacity>& current) {
    if (!active_ || current.length() == 0) return false;
    active_ = false;
    return true;
  }

  bool cancel(CommandBuffer<Capacity>& current) {
    if (!active_) return false;
    current.set(saved_.c_str());
    active_ = false;
    return true;
  }

  bool active() const { return active_; }

 private:
  CommandBuffer<Capacity> saved_;
  bool active_ = false;
};
