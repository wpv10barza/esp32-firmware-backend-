#include <unity.h>

#include "command_buffer.h"
#include "command_source.h"

void test_send_source_tracks_runtime_edits() {
  CommandBuffer<32> buffer;
  TEST_ASSERT_TRUE(buffer.set("Comando inicial"));

  TEST_ASSERT_EQUAL_STRING("Comando inicial", command_source::for3C(buffer));

  buffer.setCursor(buffer.length());
  TEST_ASSERT_TRUE(buffer.insert(" actualizado"));

  TEST_ASSERT_EQUAL_STRING(
      "Comando inicial actualizado",
      command_source::for3C(buffer));
}

void test_send_source_never_falls_back_when_runtime_buffer_is_empty() {
  CommandBuffer<32> buffer;
  TEST_ASSERT_TRUE(buffer.set("Comando inicial"));

  buffer.clear();

  TEST_ASSERT_EQUAL_UINT(0, buffer.length());
  TEST_ASSERT_EQUAL_STRING("", command_source::for3C(buffer));
  TEST_ASSERT_TRUE(buffer.invariantHolds());
}

void test_send_source_preserves_inline_editing() {
  CommandBuffer<32> buffer;
  TEST_ASSERT_TRUE(buffer.set("AC"));
  buffer.setCursor(1);
  TEST_ASSERT_TRUE(buffer.insert('B'));

  TEST_ASSERT_EQUAL_STRING("ABC", command_source::for3C(buffer));
  TEST_ASSERT_EQUAL_UINT(2, buffer.cursor());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_send_source_tracks_runtime_edits);
  RUN_TEST(test_send_source_never_falls_back_when_runtime_buffer_is_empty);
  RUN_TEST(test_send_source_preserves_inline_editing);
  return UNITY_END();
}
