#pragma once

namespace command_source {

// Single runtime source of truth for the 3C transport path.
// The command is read from the editable CommandBuffer at send time.
template <typename Buffer>
const char* for3C(const Buffer& buffer) {
  return buffer.c_str();
}

}  // namespace command_source
