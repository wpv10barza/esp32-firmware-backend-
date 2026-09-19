#pragma once

#include <cctype>
#include <cstddef>

namespace command_validation {

constexpr const char* kEmptyCommandMessage = "COMANDO VACIO";

inline bool isBlank(const char* command) {
  if (command == nullptr) return true;
  for (const unsigned char* cursor =
           reinterpret_cast<const unsigned char*>(command);
       *cursor != '\0'; ++cursor) {
    if (!std::isspace(*cursor)) return false;
  }
  return true;
}

}  // namespace command_validation
