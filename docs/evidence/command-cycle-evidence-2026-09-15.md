# Command-cycle operational evidence register

Date: 2026-09-15 15:47-05:00 (America/Lima)
Repository: `wpv10barza/esp32-firmware-backend-`
Branch under validation: `ui/panel-human-confirmation`
PR: #9

## Evidence boundary

This register deliberately separates four evidence domains:

1. **Backend transport/API evidence** — HTTP request/response status and response body, including `command_id`.
2. **ESP32 application-state evidence** — the firmware state transitions driven by those HTTP results.
3. **Physical ESP32/display evidence** — serial output, display capture, touch interaction capture, and power/boot observations from the real panel.
4. **CI evidence** — reproducible source/compile checks. CI is not physical hardware proof.

A passing backend health check MUST NOT be recorded as proof that the physical ESP32, ST7701 display, GT911 touch controller, or speaker are healthy.

## Command-cycle trace

| Step | Timestamp (ISO-8601) | Evidence | Expected / observed | Result | Capture |
|---|---|---|---|---|---|
| 0 | 2026-09-15T15:47:00-05:00 | Test run prepared | Test identity and environment recorded | RECORDED | `capture-00-environment.txt` |
| 1 | TBD | Physical boot | Serial shows display initialization and `display->begin() OK`; screen diagnostics execute | NOT EXECUTED IN THIS SESSION | `capture-01-boot.txt` + `capture-01-screen.jpg` |
| 2 | TBD | Backend health | `GET /api/device/v1/health` -> HTTP 200 | NOT EXECUTED IN THIS SESSION | `capture-02-health.txt` |
| 3 | TBD | Command submission | `POST /api/device/v1/commands` -> HTTP 200 or 202; response contains `command_id` | NOT EXECUTED IN THIS SESSION | `capture-03-post.txt` |
| 4 | TBD | Pending state | ESP32 renders `PENDIENTE` / `CONFIRMACIÓN REQUERIDA EN WEB` and retains `command_id` | NOT EXECUTED IN THIS SESSION | `capture-04-pending.jpg` + `capture-04-serial.txt` |
| 5 | TBD | Web/backend decision | Human confirmation or rejection recorded outside the ESP32 health check | NOT EXECUTED IN THIS SESSION | `capture-05-web-decision.jpg` |
| 6 | TBD | Command polling | `GET /api/device/v1/commands/{command_id}` -> HTTP 200 with terminal status | NOT EXECUTED IN THIS SESSION | `capture-06-poll.txt` |
| 7 | TBD | Applied/rejected/error UI | ESP32 renders `APLICADO`, `RECHAZADO`, or `ERROR` according to returned status | NOT EXECUTED IN THIS SESSION | `capture-07-final.jpg` + `capture-07-serial.txt` |
| 8 | TBD | End-to-end closure | Same `command_id` is traceable from POST response through terminal poll | NOT EXECUTED IN THIS SESSION | `capture-08-trace.txt` |

## Required command identifiers

- `device_id`: expected default for the panel is `panel-4848s040-3c-01`.
- `request_id`: generated per command by the firmware.
- `command_id`: MUST be copied exactly from the `POST /api/device/v1/commands` JSON response and reused for polling.

The firmware currently constructs a unique `request_id` and extracts `command_id` from the POST response before starting the 2500 ms polling cycle.

## HTTP evidence rules

### Health

Record:

```text
timestamp=<ISO-8601>
request=GET /api/device/v1/health
http_status=<code>
response_body=<sanitized body>
classification=BACKEND_REACHABILITY
```

HTTP 200 means the configured backend endpoint answered successfully. It does **not** prove the physical panel is healthy.

### Submit command

Record:

```text
timestamp=<ISO-8601>
request=POST /api/device/v1/commands
http_status=<200|202|other>
request_id=<value>
command_id=<value returned by backend>
response_body=<sanitized body>
classification=COMMAND_ACCEPTANCE
```

HTTP 200/202 is evidence that the backend accepted the command request. It is not evidence that the physical display rendered the final state.

### Poll command status

Record:

```text
timestamp=<ISO-8601>
request=GET /api/device/v1/commands/<command_id>
http_status=<code>
command_id=<same command_id as submit step>
status=<pending_confirmation|pending|applied|rejected|error|failed|fallido>
result=<sanitized result>
classification=COMMAND_LIFECYCLE
```

The firmware polls this endpoint every 2500 ms and maps terminal statuses to `APLICADO`, `RECHAZADO`, or `ERROR` on the display.

## Physical-panel evidence rules

Physical proof requires at least:

- one serial capture showing panel initialization, including `display->begin() OK`;
- one photo/screenshot of the panel while `PENDIENTE` is visible with the tested command context;
- one photo/screenshot of the terminal UI state (`APLICADO`, `RECHAZADO`, or `ERROR`);
- serial log covering the corresponding HTTP transaction(s);
- the exact `command_id` cross-referenced in all applicable captures.

Do not replace these captures with GitHub Actions status, source inspection, or a backend-only HTTP check.

## CI evidence

Current workflow evidence is limited to source/compile/package validation. The workflow itself writes a manifest with `validation=compiled_not_physically_flashed`, so CI must be cited as build evidence only.

## Current status

**Operational end-to-end proof: NOT YET ESTABLISHED.**

The repository contains the required lifecycle instrumentation and UI state mapping, but this session did not have access to the real ESP32 panel or the LAN backend at the configured private endpoint. Therefore no real `command_id`, HTTP response transcript, or physical screen capture is fabricated here.

## Acceptance criterion

Mark the cycle `PROVEN` only when steps 1-8 have real timestamps and captures, the same `command_id` is present across POST and poll records, and the physical final-screen capture agrees with the terminal backend status.
