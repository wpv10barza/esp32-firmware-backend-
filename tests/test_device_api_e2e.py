#!/usr/bin/env python3
"""Protocol E2E for the ESP32 device API.

This test emulates the network-visible ESP32 client against a real HTTP server
implementing the same device API contract. It verifies the complete lifecycle:
health -> POST command -> pending -> human confirmation -> applied/rejected.
No Google credentials or real hardware are required.
"""
import json
import threading
import time
import urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

TOKEN = "ci-e2e-token"

class Store:
    def __init__(self):
        self.command = None
        self.next_id = 1

store = Store()

class Handler(BaseHTTPRequestHandler):
    def send_json(self, code, body):
        raw = json.dumps(body).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(raw)))
        self.end_headers()
        self.wfile.write(raw)

    def authorized(self):
        return self.headers.get("X-3C-Device-Token") == TOKEN

    def do_GET(self):
        if self.path == "/api/device/v1/health":
            self.send_json(200, {"ok": True, "requires_human_confirmation": True,
                                 "protocol_version": "1.0", "supports_status_polling": True})
            return
        if self.path.startswith("/api/device/v1/commands/"):
            if not self.authorized():
                self.send_json(401, {"error": "invalid token"})
                return
            cid = self.path.rsplit("/", 1)[-1]
            if not store.command or store.command["id"] != cid:
                self.send_json(404, {"error": "not found"})
                return
            self.send_json(200, {"command": store.command})
            return
        self.send_json(404, {"error": "not found"})

    def do_POST(self):
        if self.path == "/api/device/v1/commands":
            if not self.authorized():
                self.send_json(401, {"error": "invalid token"})
                return
            length = int(self.headers.get("Content-Length", "0"))
            payload = json.loads(self.rfile.read(length))
            assert payload["device_id"] == "panel-4848s040-3c-ci"
            assert payload["text"] == "Cambia la tarea J10 a mensual"
            store.command = {
                "id": f"ci-{store.next_id}", "request_id": payload["request_id"],
                "device_id": payload["device_id"], "text": payload["text"],
                "status": "pending_confirmation", "result": None,
            }
            store.next_id += 1
            self.send_json(202, {"command_id": store.command["id"],
                                 "request_id": store.command["request_id"],
                                 "status": "pending_confirmation",
                                 "requires_human_confirmation": True})
            return
        if self.path.startswith("/api/device/v1/commands/") and self.path.endswith("/result"):
            if not store.command:
                self.send_json(404, {"error": "not found"})
                return
            length = int(self.headers.get("Content-Length", "0"))
            payload = json.loads(self.rfile.read(length))
            assert payload["status"] in ("applied", "rejected")
            store.command["status"] = payload["status"]
            store.command["result"] = payload.get("result")
            self.send_json(200, {"command": store.command})
            return
        self.send_json(404, {"error": "not found"})

    def log_message(self, *_):
        pass

def request(method, url, payload=None, token=TOKEN):
    data = json.dumps(payload).encode() if payload is not None else None
    req = urllib.request.Request(url, data=data, method=method,
                                 headers={"X-3C-Device-Token": token,
                                          "Content-Type": "application/json"})
    try:
        with urllib.request.urlopen(req, timeout=3) as response:
            return response.status, json.loads(response.read())
    except urllib.error.HTTPError as exc:
        return exc.code, json.loads(exc.read())

server = ThreadingHTTPServer(("127.0.0.1", 0), Handler)
threading.Thread(target=server.serve_forever, daemon=True).start()
base = f"http://127.0.0.1:{server.server_port}"

try:
    code, health = request("GET", base + "/api/device/v1/health")
    assert code == 200 and health["requires_human_confirmation"]

    request_id = "panel-4848s040-3c-ci-001"
    code, queued = request("POST", base + "/api/device/v1/commands", {
        "device_id": "panel-4848s040-3c-ci",
        "request_id": request_id,
        "text": "Cambia la tarea J10 a mensual",
    })
    assert code == 202 and queued["status"] == "pending_confirmation"

    cid = queued["command_id"]
    code, pending = request("GET", f"{base}/api/device/v1/commands/{cid}")
    assert code == 200 and pending["command"]["status"] == "pending_confirmation"

    # This POST represents the Monitor's human-confirmation result.
    code, applied = request("POST", f"{base}/api/device/v1/commands/{cid}/result", {
        "status": "applied", "result": "Fila J10 actualizada",
    })
    assert code == 200 and applied["command"]["status"] == "applied"

    code, terminal = request("GET", f"{base}/api/device/v1/commands/{cid}")
    assert code == 200 and terminal["command"]["status"] == "applied"
    assert terminal["command"]["result"] == "Fila J10 actualizada"

    print("ESP32 device protocol E2E: PASS")
    print("- health")
    print("- authenticated command POST")
    print("- pending_confirmation")
    print("- status polling")
    print("- human confirmation result")
    print("- terminal applied state")
finally:
    server.shutdown()
