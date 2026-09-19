#pragma once

#include <cstddef>

namespace command_validation {

// A command is executable only when it contains at least one byte that is not
// ASCII whitespace. The function is deliberately Arduino-free so the same
// rule can be unit-tested natively and reused by every input path.
inline bool hasContent(const char* text, std::size_t length) {
  if (text == nullptr || length == 0) return false;

  for (std::size_t index = 0; index < length; ++index) {
    const unsigned char value = static_cast<unsigned char>(text[index]);
    if (value > static_cast<unsigned char>(' ')) return true;
  }
  return false;
}

inline bool hasContent(const char* text) {
  if (text == nullptr) return false;

  std::size_t length = 0;
  while (text[length] != '\0') ++length;
  return hasContent(text, length);
}

}  // namespace command_validation
