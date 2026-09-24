#pragma once

#include <cstddef>

namespace panel_state {

enum class State {
  Booting,
  Offline,
  Ready,
  Busy,
  Pending,
  Applied,
  Rejected,
  Error,
};

inline const char* label(State state) {
  switch (state) {
    case State::Booting: return "INICIANDO";
    case State::Offline: return "SIN CONEXION";
    case State::Ready: return "WSL DISPONIBLE";
    case State::Busy: return "PROCESANDO";
    case State::Pending: return "PENDIENTE";
    case State::Applied: return "APLICADO";
    case State::Rejected: return "RECHAZADO";
    case State::Error: return "ERROR";
  }
  return "3C";
}

inline bool equalsIgnoreCase(const char* a, const char* b) {
  if (!a || !b) return false;
  while (*a && *b) {
    char ca = *a++;
    char cb = *b++;
    if (ca >= 'A' && ca <= 'Z') ca = static_cast<char>(ca - 'A' + 'a');
    if (cb >= 'A' && cb <= 'Z') cb = static_cast<char>(cb - 'A' + 'a');
    if (ca != cb) return false;
  }
  return *a == '\0' && *b == '\0';
}

inline State fromHealth(bool wifiConnected, int httpCode) {
  if (!wifiConnected) return State::Offline;
  return httpCode == 200 ? State::Ready : State::Error;
}

inline State fromPost(int httpCode, bool commandIdPresent) {
  if ((httpCode == 200 || httpCode == 202) && commandIdPresent) {
    return State::Pending;
  }
  return State::Error;
}

inline State fromPollStatus(const char* status) {
  if (equalsIgnoreCase(status, "applied")) return State::Applied;
  if (equalsIgnoreCase(status, "rejected")) return State::Rejected;
  if (equalsIgnoreCase(status, "pending_confirmation") ||
      equalsIgnoreCase(status, "pending") ||
      equalsIgnoreCase(status, "pendiente")) {
    return State::Pending;
  }
  if (equalsIgnoreCase(status, "error") ||
      equalsIgnoreCase(status, "failed") ||
      equalsIgnoreCase(status, "fallido")) {
    return State::Error;
  }
  return State::Error;
}

inline State fromPollHttp(int httpCode) {
  return httpCode == 200 ? State::Pending : State::Error;
}

inline bool isError(State state) {
  return state == State::Error;
}

inline bool isTerminal(State state) {
  return state == State::Applied ||
         state == State::Rejected ||
         state == State::Error;
}

}  // namespace panel_state
