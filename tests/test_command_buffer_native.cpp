#include <cassert>
#include <cstring>

#include "command_buffer.h"

static void test_arbitrary_cursor_edit_integrity() {
  CommandBuffer<64> buffer;
  assert(buffer.set("0123456789"));
  assert(buffer.invariantHolds());

  buffer.setCursor(5);
  assert(buffer.insert("AB", 2));
  assert(std::strcmp(buffer.c_str(), "01234AB56789") == 0);
  assert(buffer.cursor() == 7 && buffer.invariantHolds());

  buffer.moveLeft();
  assert(buffer.deleteForward());
  assert(std::strcmp(buffer.c_str(), "012345B56789") == 0);
  assert(buffer.cursor() == 6 && buffer.invariantHolds());

  buffer.setCursor(1);
  assert(buffer.backspace() == true);
  assert(std::strcmp(buffer.c_str(), "12345B56789") == 0);
  assert(buffer.cursor() == 0 && buffer.invariantHolds());

  assert(buffer.insert('X'));
  assert(std::strcmp(buffer.c_str(), "X12345B56789") == 0);
  assert(buffer.cursor() == 1 && buffer.invariantHolds());

  buffer.moveEnd();
  assert(buffer.backspace());
  assert(std::strcmp(buffer.c_str(), "X12345B5678") == 0);
  assert(buffer.cursor() == 11 && buffer.invariantHolds());
}

int main() {
  test_arbitrary_cursor_edit_integrity();
  CommandBuffer<48> buffer;
  assert(buffer.set("Cambia la tarea J10 a mensual"));
  assert(buffer.invariantHolds());

  buffer.setCursor(3);
  assert(buffer.insert('X'));
  assert(std::strcmp(buffer.c_str(), "CamXbia la tarea J10 a mensual") == 0);
  assert(buffer.cursor() == 4);

  assert(buffer.backspace());
  assert(std::strcmp(buffer.c_str(), "Cambia la tarea J10 a mensual") == 0);

  buffer.moveEnd();
  assert(buffer.insert('!'));
  assert(std::strstr(buffer.c_str(), "!") != nullptr);

  CommandBuffer<4> bounded;
  assert(bounded.set("ABCD"));
  bounded.setCursor(2);
  assert(!bounded.insert('X'));
  assert(std::strcmp(bounded.c_str(), "ABCD") == 0);
  assert(bounded.invariantHolds());

  return 0;
}
