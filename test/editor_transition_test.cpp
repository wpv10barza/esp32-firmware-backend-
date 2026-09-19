#include <cassert>
#include <cstring>

#include "command_editor_transaction.h"

int main() {
  using Session = CommandEditSession<32>;
  CommandBuffer<32> buffer;
  Session session;

  assert(buffer.set("original"));
  session.begin(buffer);
  assert(session.active());
  assert(buffer.set("edited"));
  assert(session.cancel(buffer));
  assert(!session.active());
  assert(std::strcmp(buffer.c_str(), "original") == 0);

  session.begin(buffer);
  assert(buffer.set("committed"));
  assert(session.save(buffer));
  assert(!session.active());
  assert(std::strcmp(buffer.c_str(), "committed") == 0);

  session.begin(buffer);
  buffer.clear();
  assert(!session.save(buffer));
  assert(session.active());
  assert(session.cancel(buffer));
  assert(std::strcmp(buffer.c_str(), "committed") == 0);

  assert(!session.cancel(buffer));
  assert(!session.save(buffer));
  return 0;
}
