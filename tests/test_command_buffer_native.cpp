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

  // moveLeft positions the cursor between A and B; deleteForward removes B.
  buffer.moveLeft();
  assert(buffer.deleteForward());
  assert(std::strcmp(buffer.c_str(), "01234A56789") == 0);
  assert(buffer.cursor() == 6 && buffer.invariantHolds());

  // Backspace at the same cursor removes A and restores the original string.
  assert(buffer.backspace());
  assert(std::strcmp(buffer.c_str(), "0123456789") == 0);
  assert(buffer.cursor() == 5 && buffer.invariantHolds());

  // Insert again at the middle and verify the cursor advances with the edit.
  assert(buffer.insert('X'));
  assert(std::strcmp(buffer.c_str(), "01234X56789") == 0);
  assert(buffer.cursor() == 6 && buffer.invariantHolds());

  // Backspace at an arbitrary early cursor must not corrupt the suffix.
  buffer.setCursor(1);
  assert(buffer.backspace());
  assert(std::strcmp(buffer.c_str(), "1234X56789") == 0);
  assert(buffer.cursor() == 0 && buffer.invariantHolds());

  // The same cursor remains a valid insertion point after deletion.
  assert(buffer.insert('Y'));
  assert(std::strcmp(buffer.c_str(), "Y1234X56789") == 0);
  assert(buffer.cursor() == 1 && buffer.invariantHolds());

  buffer.moveEnd();
  assert(buffer.backspace());
  assert(std::strcmp(buffer.c_str(), "Y1234X5678") == 0);
  assert(buffer.cursor() == 11 - 1 && buffer.invariantHolds());
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
