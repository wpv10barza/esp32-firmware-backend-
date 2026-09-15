#!/usr/bin/env bash
set -euo pipefail

SOURCE="src/main.cpp"
CONFIG="include/app_config.h"

assert_contains() {
  local pattern="$1"
  local file="$2"
  grep -Fq -- "$pattern" "$file" || {
    echo "FAIL: expected '$pattern' in $file" >&2
    exit 1
  }
}

assert_contains 'kHttpAttempts = 2' "$SOURCE"
assert_contains '[ERROR] transport' "$SOURCE"
assert_contains 'commandPollMs = 2500UL' "$CONFIG"

for forbidden in preparedCommand prepare3CCommand cancelPreparedCommand 'CONFIRMAR' 'CANCELAR' '"/confirm"' '"/reject"'; do
  if grep -Fq -- "$forbidden" "$SOURCE"; then
    echo "FAIL: forbidden symbol/text '$forbidden' found" >&2
    exit 1
  fi
done

echo "PASS: HTTP transport regression policy and invariants"
