#include <cassert>
#include <cstring>
#include <iostream>

#include "panel_state_machine.h"

using panel_state::State;

int main() {
  // Health: backend connectivity failures must become a visible ERROR.
  assert(panel_state::fromHealth(true, 200) == State::Ready);
  assert(panel_state::fromHealth(true, 500) == State::Error);
  assert(panel_state::fromHealth(true, 503) == State::Error);
  assert(panel_state::fromHealth(true, -1) == State::Error);
  assert(panel_state::fromHealth(false, 200) == State::Offline);

  // POST: only 200/202 with a command_id may enter PENDING.
  assert(panel_state::fromPost(202, true) == State::Pending);
  assert(panel_state::fromPost(200, true) == State::Pending);
  assert(panel_state::fromPost(202, false) == State::Error);
  assert(panel_state::fromPost(200, false) == State::Error);
  assert(panel_state::fromPost(400, true) == State::Error);
  assert(panel_state::fromPost(500, true) == State::Error);

  // Polling: all documented terminal/error spellings must map explicitly.
  assert(panel_state::fromPollStatus("pending_confirmation") == State::Pending);
  assert(panel_state::fromPollStatus("PENDING") == State::Pending);
  assert(panel_state::fromPollStatus("pendiente") == State::Pending);
  assert(panel_state::fromPollStatus("applied") == State::Applied);
  assert(panel_state::fromPollStatus("APPLIED") == State::Applied);
  assert(panel_state::fromPollStatus("rejected") == State::Rejected);
  assert(panel_state::fromPollStatus("REJECTED") == State::Rejected);

  for (const char* value : {"error", "failed", "fallido", "ERROR", "FAILED", "FALLIDO"}) {
    assert(panel_state::fromPollStatus(value) == State::Error);
  }

  // Unknown/missing status must never leave the screen stuck in PENDING.
  assert(panel_state::fromPollStatus("") == State::Error);
  assert(panel_state::fromPollStatus(nullptr) == State::Error);
  assert(panel_state::fromPollStatus("unexpected") == State::Error);

  // Rendering contract: these labels are the strings shown by the panel.
  assert(std::strcmp(panel_state::label(State::Error), "ERROR") == 0);
  assert(std::strcmp(panel_state::label(State::Pending), "PENDIENTE") == 0);
  assert(std::strcmp(panel_state::label(State::Applied), "APLICADO") == 0);
  assert(std::strcmp(panel_state::label(State::Rejected), "RECHAZADO") == 0);

  assert(panel_state::isError(State::Error));
  assert(panel_state::isTerminal(State::Applied));
  assert(panel_state::isTerminal(State::Rejected));
  assert(panel_state::isTerminal(State::Error));

  std::cout << "Panel state E2E model: PASS\n";
  std::cout << "- Health failure -> ERROR\n";
  std::cout << "- POST failure / missing command_id -> ERROR\n";
  std::cout << "- Poll error/failed/fallido -> ERROR\n";
  std::cout << "- Unknown/missing status -> ERROR\n";
  std::cout << "- Pending/applied/rejected remain explicit\n";
  return 0;
}
