#!/usr/bin/env bash
set -Eeuo pipefail

REPO="${V15_REPO:-wpv10barza/esp32-firmware-backend-}"
BRANCH="${V15_BRANCH:-main}"
ROOT="$(git rev-parse --show-toplevel 2>/dev/null || true)"

log() { printf '%s\n' "$*"; }
die() { log "ERROR: $*"; exit 1; }

[[ -n "$ROOT" ]] || die "not inside a Git repository"
cd "$ROOT"

find_v141() {
  local configured="${V141_README:-}"
  if [[ -n "$configured" ]]; then
    [[ -f "$configured" ]] || return 2
    [[ "$configured" != http://* && "$configured" != https://* ]] || return 2
    realpath "$configured"
    return 0
  fi

  [[ -f "$ROOT/README.md" ]] || return 1
  realpath "$ROOT/README.md"
}

validate_v141() {
  local file="$1"
  local first_line
  first_line="$(head -n 1 "$file" | tr -d '\r')"

  [[ "$first_line" == "T-030 System ESP32-S3-4848S040 plus Backend Asistente 3C" ]] || {
    log "MISSING/INVALID: first title line"
    return 1
  }

  local required=(
    "V14.1"
    "3.2 Design of the system"
    "3.2.1 Electronic design"
    "3.2.2 Software design"
    "st7701_type8_init_operations"
    "server/deviceApi.ts"
    "server/deviceCommands.ts"
    "server/reviewControl.ts"
    "WEB FIRST then REPOSITORY AFTER"
    "No modification or reflashing of firmware"
  )

  local marker
  for marker in "${required[@]}"; do
    grep -Fq "$marker" "$file" || {
      log "MISSING: $marker"
      return 1
    }
  done

  if grep -Fq "T-030 Sistema de Desarrollo Completo - Condiciones Iniciales V6 Benchmark" "$file" ||
     grep -Fq "Version V6 Benchmark" "$file" ||
     grep -Fq "Versión V6 Benchmark" "$file"; then
    log "INVALID: candidate is the V6 Benchmark README"
    return 1
  fi

  return 0
}

V141="$(find_v141)" || {
  log "V15 BLOCKED: authoritative local V14.1 README not found."
  log "Use local README.md or V141_README=/absolute/path/README.md."
  log "No remote GitHub write was attempted."
  exit 22
}

log "V14.1 candidate: $V141"
validate_v141 "$V141" || {
  log "V15 BLOCKED: V14.1 validation failed."
  exit 23
}
log "V14.1 validation: PASS"

if [[ "${V15_DRY_RUN:-0}" == "1" ]]; then
  log "DRY RUN: no remote access or write."
  exit 0
fi

command -v gh >/dev/null 2>&1 || die "GitHub CLI (gh) is required"
gh auth status >/dev/null 2>&1 || die "gh authentication required"

REMOTE_HEAD="$(gh api "repos/$REPO/git/ref/heads/$BRANCH" --jq '.object.sha')"
REMOTE_README_SHA="$(gh api "repos/$REPO/contents/README.md?ref=$REMOTE_HEAD" --jq '.sha')"
log "Remote HEAD frozen: $REMOTE_HEAD"
log "Remote README blob SHA frozen: $REMOTE_README_SHA"

BACKUP_DIR="${V15_BACKUP_DIR:-${RUNNER_TEMP:-/tmp}/v15-backup}"
mkdir -p "$BACKUP_DIR"
TIMESTAMP="$(date -u +%Y%m%dT%H%M%SZ)"
BACKUP="$BACKUP_DIR/README_REMOTE_PRE_V15_${TIMESTAMP}.md"
REMOTE_SHA256_FILE="$BACKUP_DIR/README_REMOTE_PRE_V15_${TIMESTAMP}.sha256"
LOCAL_SHA256_FILE="$BACKUP_DIR/README_V14.1_PRE_V15_${TIMESTAMP}.sha256"

gh api "repos/$REPO/contents/README.md?ref=$REMOTE_HEAD" --jq '.content' |
  tr -d '\n' | base64 --decode > "$BACKUP"

REMOTE_SHA256="$(sha256sum "$BACKUP" | awk '{print $1}')"
REMOTE_WORD_COUNT="$(wc -w < "$BACKUP" | tr -d ' ')"
printf '%s  %s\n' "$REMOTE_SHA256" "$BACKUP" > "$REMOTE_SHA256_FILE"

LOCAL_SHA256="$(sha256sum "$V141" | awk '{print $1}')"
LOCAL_WORD_COUNT="$(wc -w < "$V141" | tr -d ' ')"
printf '%s  %s\n' "$LOCAL_SHA256" "$V141" > "$LOCAL_SHA256_FILE"

log "Remote PRE-V15 SHA-256: $REMOTE_SHA256"
log "Remote PRE-V15 word count: $REMOTE_WORD_COUNT"
log "Local V14.1 SHA-256: $LOCAL_SHA256"
log "Local V14.1 word count: $LOCAL_WORD_COUNT"

DIFF_FILE="$BACKUP_DIR/README_PRE_V15_vs_V14.1_${TIMESTAMP}.diff"
if cmp -s "$V141" "$BACKUP"; then
  log "LOCAL/REMOTE: IDENTICAL"
  log "No consolidation required; stopping before write."
  exit 0
fi

diff -u "$BACKUP" "$V141" > "$DIFF_FILE" || true
log "LOCAL/REMOTE: DIFFERENCE CONFIRMED"
log "Controlled diff: $DIFF_FILE"

REMOTE_CHECK="$(gh api "repos/$REPO/git/ref/heads/$BRANCH" --jq '.object.sha')"
[[ "$REMOTE_CHECK" == "$REMOTE_HEAD" ]] || die "remote HEAD changed before publication"

CONTENT_B64="$(base64 -w0 "$V141")"
RESPONSE="$(gh api --method PUT \
  -H "Accept: application/vnd.github+json" \
  -H "X-GitHub-Api-Version: 2026-03-10" \
  "repos/$REPO/contents/README.md" \
  -f message="docs V15 controlled consolidation WEB first until 3.2.2" \
  -f content="$CONTENT_B64" \
  -f sha="$REMOTE_README_SHA" \
  -f branch="$BRANCH")"

COMMIT_SHA="$(jq -r '.commit.sha' <<< "$RESPONSE")"
[[ -n "$COMMIT_SHA" && "$COMMIT_SHA" != "null" ]] || die "README publication did not return a commit SHA"

FINAL_REMOTE="$(mktemp)"
trap 'rm -f "$FINAL_REMOTE"' EXIT
gh api "repos/$REPO/contents/README.md?ref=$COMMIT_SHA" --jq '.content' |
  tr -d '\n' | base64 --decode > "$FINAL_REMOTE"

cmp -s "$V141" "$FINAL_REMOTE" || die "remote README is not byte-for-byte identical to V14.1"

FINAL_SHA256="$(sha256sum "$FINAL_REMOTE" | awk '{print $1}')"
FINAL_WORD_COUNT="$(wc -w < "$FINAL_REMOTE" | tr -d ' ')"

[[ "$FINAL_SHA256" == "$LOCAL_SHA256" ]] || die "final SHA-256 does not match local V14.1"
[[ "$FINAL_WORD_COUNT" == "$LOCAL_WORD_COUNT" ]] || die "final word count does not match local V14.1"

CHANGED_FILES="$(gh api "repos/$REPO/commits/$COMMIT_SHA" --jq '.files[].filename')"
[[ "$CHANGED_FILES" == "README.md" ]] || die "publication changed files other than README.md"

log "V15 COMPLETE"
log "Commit: $COMMIT_SHA"
log "Final remote SHA-256: $FINAL_SHA256"
log "Final remote word count: $FINAL_WORD_COUNT"
log "Byte-for-byte comparison: IDENTICAL"
log "Changed files: README.md only"
log "Firmware/hardware modification: NONE"
