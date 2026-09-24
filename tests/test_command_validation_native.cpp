#include <cassert>

#include "command_validation.h"

int main() {
  using command_validation::isBlank;

  assert(isBlank(nullptr));
  assert(isBlank(""));
  assert(isBlank(" "));
  assert(isBlank("    "));
  assert(isBlank("\t"));
  assert(isBlank("\r\n\t "));
  assert(isBlank("\n\n"));

  assert(!isBlank("x"));
  assert(!isBlank(" x "));
  assert(!isBlank("Cambia la tarea J10 a mensual"));
  assert(!isBlank("0"));
  assert(!isBlank("!"));

  return 0;
}
