#!/usr/bin/env python3
"""Probe the configured backend health endpoint without echoing secrets."""

import json
import os
import sys
import urllib.error
import urllib.request

url = os.environ.get("BACKEND_HEALTH_URL", "").strip()
token = os.environ.get("BACKEND_HEALTH_TOKEN", "").strip()

if not url:
    print("BACKEND HEALTH SMOKE: SKIPPED (BACKEND_HEALTH_URL secret is not configured)")
    raise SystemExit(0)

headers = {"Accept": "application/json"}
if token:
    headers["X-3C-Device-Token"] = token

request = urllib.request.Request(url, method="GET", headers=headers)
try:
    with urllib.request.urlopen(request, timeout=15) as response:
        body = response.read().decode("utf-8", errors="replace")
        status = response.status
except urllib.error.HTTPError as exc:
    print(f"BACKEND HEALTH SMOKE: FAIL HTTP {exc.code}")
    raise SystemExit(1)
except (urllib.error.URLError, TimeoutError) as exc:
    print(f"BACKEND HEALTH SMOKE: FAIL network error: {exc.reason if hasattr(exc, 'reason') else type(exc).__name__}")
    raise SystemExit(1)

if status != 200:
    print(f"BACKEND HEALTH SMOKE: FAIL expected HTTP 200, got {status}")
    raise SystemExit(1)

try:
    payload = json.loads(body)
except json.JSONDecodeError:
    print("BACKEND HEALTH SMOKE: FAIL response is not valid JSON")
    raise SystemExit(1)

if not isinstance(payload, dict):
    print("BACKEND HEALTH SMOKE: FAIL response JSON is not an object")
    raise SystemExit(1)

if payload.get("ok") is not True:
    print("BACKEND HEALTH SMOKE: FAIL response JSON does not contain ok=true")
    raise SystemExit(1)

print("BACKEND HEALTH SMOKE: PASS (HTTP 200, JSON ok=true)")
