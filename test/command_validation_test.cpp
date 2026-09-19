#include <cassert>
#include <cstddef>

#include "command_validation.h"

int main() {
  using command_validation::hasContent;

  // Empty input must never be executable.
  assert(!hasContent(nullptr, 0));
  assert(!hasContent("", 0));
  assert(!hasContent(""));

  // Whitespace-only input is also empty after normalization.
  assert(!hasContent(" ", 1));
  assert(!hasContent("    ", 4));
  assert(!hasContent("\t\t", 2));
  assert(!hasContent("\n\r ", 3));
  assert(!hasContent(" \t\r\n ", 5));

  // One meaningful character is enough to make the command executable.
  assert(hasContent("A", 1));
  assert(hasContent("A"));
  assert(hasContent(" C "));
  assert(hasContent("Cambia la tarea J10 a mensual"));

  // A caller-supplied length is authoritative and never reads past it.
  assert(!hasContent("ABC", 0));
  assert(hasContent("ABC", 1));
  assert(!hasContent("   ABC", 3));
  assert(hasContent("   ABC", 4));

  return 0;
}
