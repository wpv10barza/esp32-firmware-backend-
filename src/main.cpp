#if defined(BOARD_PANEL_4848S040)

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <driver/i2s.h>
#include <esp_system.h>

#include "app_config.h"

namespace pins {
constexpr int backlight = 38;
constexpr int lcdCs = 39;
constexpr int lcdClock = 48;
constexpr int lcdMosi = 47;
constexpr int touchSda = 19;
constexpr int touchScl = 45;
constexpr int audioBclk = 1;
constexpr int audioLrclk = 2;
constexpr int audioData = 40;
}  // namespace pins

namespace {
constexpr uint8_t kTouchAddress = 0x5D;
constexpr uint16_t kTouchStatusRegister = 0x814E;
constexpr uint16_t kTouchPointRegister = 0x814F;
constexpr int kScreenWidth = 480;
constexpr int kScreenHeight = 480;

WebServer web(80);
Arduino_ESP32SPI* displayBus = nullptr;
Arduino_RGB_Display* display = nullptr;

enum class PanelState {
  Booting,
  Offline,
  Ready,
  Busy,
  Pending,
  Applied,
  Rejected,
  Error,
};

PanelState panelState = PanelState::Booting;
String panelDetail = "Iniciando";
String lastBackendMessage = "Sin verificar";
String lastCommandId;
unsigned long lastWifiAttempt = 0;
unsigned long lastHealthCheck = 0;
unsigned long lastCommandPoll = 0;
bool backendAvailable = false;
bool displayReady = false;
bool audioReady = false;
bool mdnsReady = false;
bool wifiAnnounced = false;
bool touchDown = false;

struct TouchSample {
  bool ready = false;
  bool touched = false;
  uint16_t x = 0;
  uint16_t y = 0;
};

uint16_t color565(uint8_t red, uint8_t green, uint8_t blue) {
  return display ? display->color565(red, green, blue) : 0;
}

const char* stateLabel(PanelState state) {
  switch (state) {
    case PanelState::Booting: return "INICIANDO";
    case PanelState::Offline: return "SIN CONEXION";
    case PanelState::Ready: return "WSL DISPONIBLE";
    case PanelState::Busy: return "PROCESANDO";
    case PanelState::Pending: return "PENDIENTE";
    case PanelState::Applied: return "APLICADO";
    case PanelState::Rejected: return "RECHAZADO";
    case PanelState::Error: return "ERROR";
  }
  return "3C";
}

uint16_t stateBackground(PanelState state) {
  switch (state) {
    case PanelState::Ready: return color565(5, 45, 27);
    case PanelState::Busy: return color565(8, 28, 58);
    case PanelState::Pending: return color565(68, 43, 2);
    case PanelState::Applied: return color565(2, 65, 28);
    case PanelState::Rejected: return color565(62, 29, 3);
    case PanelState::Error: return color565(65, 5, 9);
    case PanelState::Offline: return color565(18, 22, 30);
    case PanelState::Booting: return color565(10, 18, 38);
  }
  return 0;
}

void drawCentered(const String& text, int y, uint8_t size, uint16_t color) {
  if (!displayReady) return;
  int16_t x1 = 0;
  int16_t y1 = 0;
  uint16_t width = 0;
  uint16_t height = 0;
  display->setTextSize(size);
  display->getTextBounds(text, 0, y, &x1, &y1, &width, &height);
  int x = (kScreenWidth - static_cast<int>(width)) / 2;
  if (x < 4) x = 4;
  display->setTextColor(color);
  display->setCursor(x, y);
  display->print(text);
}

void drawButton(int x, int y, int width, int height, const char* label, uint16_t fill) {
  if (!displayReady) return;
  display->fillRoundRect(x, y, width, height, 16, fill);
  display->drawRoundRect(x, y, width, height, 16, color565(185, 210, 230));
  display->setTextSize(2);
  int16_t x1 = 0;
  int16_t y1 = 0;
  uint16_t textWidth = 0;
  uint16_t textHeight = 0;
  display->getTextBounds(label, 0, 0, &x1, &y1, &textWidth, &textHeight);
  display->setTextColor(WHITE);
  display->setCursor(x + (width - textWidth) / 2, y + (height - textHeight) / 2);
  display->print(label);
}

void drawPanel() {
  if (!displayReady) return;

  const uint16_t background = color565(8, 14, 22);
  const uint16_t panel = color565(17, 26, 36);
  const uint16_t panelAlt = color565(13, 21, 30);
  const uint16_t border = color565(62, 82, 100);
  const uint16_t titleColor = color565(170, 220, 255);
  const uint16_t labelColor = color565(145, 170, 190);
  const uint16_t valueColor = WHITE;
  const uint16_t accent = panelState == PanelState::Offline ? color565(125, 135, 145) : color565(60, 200, 150);
  const bool online = WiFi.status() == WL_CONNECTED && backendAvailable;

  display->fillScreen(background);

  // Header
  display->fillRect(0, 0, kScreenWidth, 58, panel);
  drawCentered("Interfaz Portátil", 16, 2, titleColor);
  display->drawFastHLine(0, 58, kScreenWidth, border);
  display->fillCircle(419, 28, 6, online ? color565(60, 200, 150) : color565(185, 100, 90));
  display->setTextSize(2);
  display->setTextColor(valueColor);
  display->setCursor(430, 21);
  display->print(online ? "ONLINE" : "OFFLINE");

  // Backend / device information
  display->fillRoundRect(16, 72, 448, 104, 12, panelAlt);
  display->drawRoundRect(16, 72, 448, 104, 12, border);
  display->setTextSize(1);
  display->setTextColor(labelColor);
  display->setCursor(30, 86);
  display->print("BACKEND");
  display->setTextSize(2);
  display->setTextColor(accent);
  display->setCursor(30, 106);
  display->print(backendAvailable ? "WSL DISPONIBLE" : "SIN CONEXION");

  display->setTextSize(1);
  display->setTextColor(labelColor);
  display->setCursor(30, 134);
  display->print("IP / DISPOSITIVO");
  display->setTextColor(valueColor);
  display->setCursor(30, 150);
  if (WiFi.status() == WL_CONNECTED) {
    display->print(WiFi.localIP().toString());
  } else {
    display->print("--");
  }
  display->setCursor(220, 150);
  display->print(app_config::deviceId);

  // Última operación
  display->fillRoundRect(16, 190, 448, 106, 12, panel);
  display->drawRoundRect(16, 190, 448, 106, 12, border);
  display->setTextSize(1);
  display->setTextColor(labelColor);
  display->setCursor(30, 205);
  display->print("ULTIMA OPERACION");
  display->setTextSize(2);
  display->setTextColor(valueColor);
  display->setCursor(30, 224);
  display->print(stateLabel(panelState));

  String detail = panelDetail;
  if (detail.length() > 58) detail = detail.substring(0, 55) + "...";
  display->setTextSize(1);
  display->setTextColor(color565(210, 225, 235));
  display->setCursor(30, 250);
  display->print(detail);

  display->setTextColor(labelColor);
  display->setCursor(30, 270);
  display->print("command_id: ");
  if (lastCommandId.length()) {
    String shortId = lastCommandId;
    if (shortId.length() > 12) shortId = shortId.substring(0, 8) + "..." + shortId.substring(shortId.length() - 4);
    display->setTextColor(valueColor);
    display->print(shortId);
  } else {
    display->setTextColor(valueColor);
    display->print("--");
  }

  // Resultado del último comando / espera de polling
  display->fillRoundRect(16, 310, 448, 54, 12, panelAlt);
  display->drawRoundRect(16, 310, 448, 54, 12, border);
  display->setTextSize(1);
  display->setTextColor(labelColor);
  display->setCursor(30, 324);
  display->print("RESULTADO");
  display->setTextSize(2);
  display->setCursor(30, 342);
  if (panelState == PanelState::Applied || panelState == PanelState::Rejected || panelState == PanelState::Error) {
    display->setTextColor(valueColor);
    display->print(stateLabel(panelState));
  } else if (panelState == PanelState::Pending) {
    display->setTextColor(color565(230, 190, 90));
    display->print("PENDIENTE");
  } else {
    display->setTextColor(valueColor);
    display->print(stateLabel(panelState));
  }

  display->setTextSize(1);
  display->setTextColor(labelColor);
  if (panelState == PanelState::Pending) {
    display->setCursor(185, 348);
    display->print("CONFIRMACIÓN REQUERIDA EN WEB");
  } else if (panelState == PanelState::Busy && lastCommandId.length()) {
    display->setCursor(185, 348);
    display->print("ESPERANDO POLLING");
  }

  // Health result remains visible without changing the HTTP contract.
  display->setTextColor(labelColor);
  display->setCursor(30, 382);
  display->print("HEALTH: ");
  display->setTextColor(backendAvailable ? color565(80, 205, 155) : color565(190, 115, 105));
  display->print(backendAvailable ? "OK" : "NO DISPONIBLE");
  String healthDetail = lastBackendMessage;
  if (healthDetail.length() > 42) healthDetail = healthDetail.substring(0, 39) + "...";
  display->setTextColor(color565(170, 190, 205));
  display->setCursor(112, 382);
  display->print(healthDetail);

  // Exactly two touch actions.
  drawButton(16, 402, 214, 62, "PROBAR WSL", color565(15, 82, 135));
  drawButton(250, 402, 214, 62, "ENVIAR 3C", color565(18, 105, 73));
}

void playTone(uint16_t frequency, uint16_t durationMs) {
  if (!audioReady || !app_config::panelAudioEnabled || frequency == 0) return;
  constexpr uint32_t sampleRate = 16000;
  constexpr size_t framesPerChunk = 128;
  int16_t samples[framesPerChunk * 2];
  const uint32_t totalFrames = sampleRate * durationMs / 1000;
  uint32_t frame = 0;
  while (frame < totalFrames) {
    size_t frames = totalFrames - frame;
    if (frames > framesPerChunk) frames = framesPerChunk;
    for (size_t index = 0; index < frames; ++index) {
      const uint32_t phase = ((frame + index) * frequency * 2U) / sampleRate;
      const int16_t sample = (phase & 1U) ? 2600 : -2600;
      samples[index * 2] = sample;
      samples[index * 2 + 1] = sample;
    }
    size_t written = 0;
    i2s_write(I2S_NUM_0, samples, frames * 2 * sizeof(int16_t), &written, portMAX_DELAY);
    frame += frames;
  }
  i2s_zero_dma_buffer(I2S_NUM_0);
}

void updatePanel(PanelState state, const String& detail, bool sound = false) {
  const bool changed = state != panelState;
  panelState = state;
  panelDetail = detail;
  drawPanel();
  if (!sound || !changed) return;
  if (state == PanelState::Applied || state == PanelState::Ready) playTone(880, 70);
  else if (state == PanelState::Pending || state == PanelState::Busy) playTone(620, 55);
  else if (state == PanelState::Rejected || state == PanelState::Error) playTone(220, 110);
}

bool initializeAudio() {
  if (!app_config::panelAudioEnabled) {
    Serial.println("Audio deshabilitado: GPIO 1/2/40 reservados para relays.");
    return false;
  }
  i2s_config_t config = {};
  config.mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX);
  config.sample_rate = 16000;
  config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
  config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
  config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  config.dma_buf_count = 4;
  config.dma_buf_len = 128;
  config.use_apll = false;
  config.tx_desc_auto_clear = true;
  config.fixed_mclk = 0;

  i2s_pin_config_t pinConfig = {};
  pinConfig.bck_io_num = pins::audioBclk;
  pinConfig.ws_io_num = pins::audioLrclk;
  pinConfig.data_out_num = pins::audioData;
  pinConfig.data_in_num = I2S_PIN_NO_CHANGE;
  if (i2s_driver_install(I2S_NUM_0, &config, 0, nullptr) != ESP_OK) return false;
  if (i2s_set_pin(I2S_NUM_0, &pinConfig) != ESP_OK) {
    i2s_driver_uninstall(I2S_NUM_0);
    return false;
  }
  i2s_zero_dma_buffer(I2S_NUM_0);
  return true;
}

void runDisplayDiagnostic() {
  if (!displayReady) return;
  Serial.println("DISPLAY DIAGNOSTIC: RED");
  display->fillScreen(color565(255, 0, 0));
  delay(400);
  Serial.println("DISPLAY DIAGNOSTIC: GREEN");
  display->fillScreen(color565(0, 255, 0));
  delay(400);
  Serial.println("DISPLAY DIAGNOSTIC: BLUE");
  display->fillScreen(color565(0, 0, 255));
  delay(400);
  Serial.println("DISPLAY DIAGNOSTIC: WHITE");
  display->fillScreen(color565(255, 255, 255));
  delay(400);
}

bool initializeDisplay() {
  Serial.println("DISPLAY: creating 9-bit SPI command bus");
  displayBus = new Arduino_ESP32SPI(
    GFX_NOT_DEFINED, pins::lcdCs, pins::lcdClock, pins::lcdMosi, GFX_NOT_DEFINED);
  Serial.println("DISPLAY: creating RGB panel 480x480");
  auto* rgbPanel = new Arduino_ESP32RGBPanel(
    18, 17, 16, 21,
    11, 12, 13, 14, 0,
    8, 20, 3, 46, 9, 10,
    4, 5, 6, 7, 15,
    1, 10, 8, 50,
    1, 10, 8, 20);
  Serial.println("DISPLAY: using Arduino-GFX ST7701 type8 init sequence");
  display = new Arduino_RGB_Display(
    kScreenWidth, kScreenHeight, rgbPanel, 0, true,
    displayBus, GFX_NOT_DEFINED,
    st7701_type8_init_operations, sizeof(st7701_type8_init_operations));
  Serial.println("DISPLAY: calling display->begin()");
  if (!display->begin()) {
    Serial.println("DISPLAY: display->begin() FAILED");
    return false;
  }
  Serial.println("DISPLAY: display->begin() OK");
  pinMode(pins::backlight, OUTPUT);
  analogWrite(pins::backlight, app_config::panelBrightness);
  Serial.printf("DISPLAY: backlight GPIO %d PWM=%u\n", pins::backlight, app_config::panelBrightness);
  display->displayOn();
  Serial.println("DISPLAY: displayOn() OK");
  runDisplayDiagnostic();
  return true;
}

bool i2cRead(uint16_t reg, uint8_t* data, size_t length) {
  Wire.beginTransmission(kTouchAddress);
  Wire.write(static_cast<uint8_t>(reg >> 8));
  Wire.write(static_cast<uint8_t>(reg & 0xFF));
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(kTouchAddress, static_cast<uint8_t>(length)) != length) return false;
  for (size_t index = 0; index < length; ++index) data[index] = Wire.read();
  return true;
}

bool i2cWriteByte(uint16_t reg, uint8_t value) {
  Wire.beginTransmission(kTouchAddress);
  Wire.write(static_cast<uint8_t>(reg >> 8));
  Wire.write(static_cast<uint8_t>(reg & 0xFF));
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

TouchSample readTouch() {
  TouchSample sample;
  uint8_t status = 0;
  if (!i2cRead(kTouchStatusRegister, &status, 1) || !(status & 0x80)) return sample;
  sample.ready = true;
  const uint8_t points = status & 0x0F;
  if (points > 0 && points <= 5) {
    uint8_t data[7] = {};
    if (i2cRead(kTouchPointRegister, data, sizeof(data))) {
      const uint16_t rawX = data[1] | (static_cast<uint16_t>(data[2]) << 8);
      const uint16_t rawY = data[3] | (static_cast<uint16_t>(data[4]) << 8);
      sample.x = rawX < kScreenWidth ? kScreenWidth - 1 - rawX : 0;
      sample.y = rawY < kScreenHeight ? kScreenHeight - 1 - rawY : 0;
      sample.touched = true;
    }
  }
  i2cWriteByte(kTouchStatusRegister, 0);
  return sample;
}

String endpoint(const String& path) {
  String base(app_config::assistantBaseUrl);
  while (base.endsWith("/")) base.remove(base.length() - 1);
  return base + path;
}

String jsonEscape(const String& input) {
  String output;
  output.reserve(input.length() + 16);
  for (size_t index = 0; index < input.length(); ++index) {
    const char value = input[index];
    if (value == '\\' || value == '"') { output += '\\'; output += value; }
    else if (value == '\n') output += "\\n";
    else if (static_cast<uint8_t>(value) >= 0x20) output += value;
  }
  return output;
}

String jsonStringValue(const String& json, const char* key) {
  const String token = String("\"") + key + "\"";
  int position = json.indexOf(token);
  if (position < 0) return "";
  position = json.indexOf(':', position + token.length());
  if (position < 0) return "";
  position++;
  while (position < static_cast<int>(json.length()) && isspace(json[position])) position++;
  if (position >= static_cast<int>(json.length()) || json[position] != '"') return "";
  position++;
  String value;
  while (position < static_cast<int>(json.length())) {
    const char current = json[position++];
    if (current == '"') break;
    if (current == '\\' && position < static_cast<int>(json.length())) value += json[position++];
    else value += current;
  }
  return value;
}

void addDeviceToken(HTTPClient& http) {
  if (strlen(app_config::apiToken)) http.addHeader("X-3C-Device-Token", app_config::apiToken);
}

bool checkBackendHealth() {
  if (WiFi.status() != WL_CONNECTED) {
    backendAvailable = false;
    updatePanel(PanelState::Offline, "Wi-Fi desconectado");
    return false;
  }
  updatePanel(PanelState::Busy, "Verificando endpoint WSL");
  HTTPClient http;
  http.setTimeout(app_config::httpTimeoutMs);
  http.begin(endpoint("/api/device/v1/health"));
  const int code = http.GET();
  lastBackendMessage = code > 0 ? http.getString() : http.errorToString(code);
  http.end();
  backendAvailable = code == 200;
  updatePanel(
    backendAvailable ? PanelState::Ready : PanelState::Error,
    backendAvailable ? "Endpoint 3C conectado" : String("HTTP ") + code,
    true);
  Serial.printf("GET health -> %d %s\n", code, lastBackendMessage.c_str());
  return backendAvailable;
}

int send3CCommand(const String& rawCommand) {
  String command = rawCommand;
  command.trim();
  if (!command.length()) {
    updatePanel(PanelState::Error, "Comando vacio", true);
    return 400;
  }
  if (WiFi.status() != WL_CONNECTED) {
    updatePanel(PanelState::Offline, "Wi-Fi desconectado", true);
    return 503;
  }

  updatePanel(PanelState::Busy, "Enviando vista previa", true);
  HTTPClient http;
  http.setTimeout(app_config::httpTimeoutMs);
  http.begin(endpoint("/api/device/v1/commands"));
  http.addHeader("Content-Type", "application/json");
  addDeviceToken(http);

  char randomPart[9];
  snprintf(randomPart, sizeof(randomPart), "%08lx", static_cast<unsigned long>(esp_random()));
  const String requestId = String(app_config::deviceId) + "-" + randomPart + "-" + String(millis());
  const String body = "{\"device_id\":\"" + jsonEscape(app_config::deviceId) +
    "\",\"request_id\":\"" + jsonEscape(requestId) +
    "\",\"text\":\"" + jsonEscape(command) + "\"}";
  const int code = http.POST(body);
  lastBackendMessage = code > 0 ? http.getString() : http.errorToString(code);
  http.end();

  if (code == 200 || code == 202) {
    backendAvailable = true;
    lastCommandId = jsonStringValue(lastBackendMessage, "command_id");
    lastCommandPoll = millis();
    updatePanel(PanelState::Pending, "CONFIRMACIÓN REQUERIDA EN WEB", true);
  } else {
    backendAvailable = false;
    updatePanel(PanelState::Error, String("Envio HTTP ") + code, true);
  }
  Serial.printf("POST 3C -> %d %s\n", code, lastBackendMessage.c_str());
  return code;
}

void pollCommandStatus() {
  if (!lastCommandId.length() || WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  http.setTimeout(app_config::httpTimeoutMs);
  http.begin(endpoint("/api/device/v1/commands/" + lastCommandId));
  addDeviceToken(http);
  const int code = http.GET();
  const String body = code > 0 ? http.getString() : http.errorToString(code);
  http.end();
  if (code != 200) {
    Serial.printf("GET command status -> %d %s\n", code, body.c_str());
    return;
  }
  const String status = jsonStringValue(body, "status");
  const String result = jsonStringValue(body, "result");
  if (status == "applied") {
    updatePanel(PanelState::Applied, result.length() ? result : "Confirmado en WSL", true);
    lastCommandId = "";
  } else if (status == "rejected") {
    updatePanel(PanelState::Rejected, result.length() ? result : "Rechazado en WSL", true);
    lastCommandId = "";
  } else if (status == "error" || status == "failed" || status == "fallido") {
    updatePanel(PanelState::Error, result.length() ? result : "Error reportado por WSL", true);
    lastCommandId = "";
  } else if (status == "pending_confirmation" || status == "pending" || status == "pendiente") {
    updatePanel(PanelState::Pending, "CONFIRMACIÓN REQUERIDA EN WEB");
  }
}

const char controlPage[] PROGMEM = R"HTML(
<!doctype html><html lang="es"><meta name="viewport" content="width=device-width,initial-scale=1">
<style>body{font-family:system-ui;max-width:680px;margin:auto;padding:24px;background:#eef3f7}section{background:white;padding:20px;border-radius:16px;box-shadow:0 5px 20px #0001}button,textarea{font:inherit}button{padding:13px 18px;border:0;border-radius:10px;background:#08784f;color:white}textarea{box-sizing:border-box;width:100%;min-height:120px;padding:12px;margin:8px 0 12px}.warn{color:#805500}</style>
<h1>Panel ESP32-4848S040 3C</h1><section><p class="warn">La orden se envía al backend y queda pendiente de confirmación en la web. Google Sheets cambia solo después de la confirmación web.</p><textarea id="text" placeholder="Cambia la tarea J10 a mensual"></textarea><button onclick="send3c()">Enviar al asistente</button><button onclick="health()">Probar WSL</button><pre id="result"></pre></section>
<script>async function send3c(){const b=new URLSearchParams({text:document.querySelector('#text').value});const r=await fetch('/api/3c',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:b});result.textContent=r.status+' '+await r.text()}async function health(){const r=await fetch('/api/backend-health',{method:'POST'});result.textContent=r.status+' '+await r.text()}</script></html>
)HTML";

void configureWebServer() {
  web.on("/", HTTP_GET, [] { web.send_P(200, "text/html; charset=utf-8", controlPage); });
  web.on("/health", HTTP_GET, [] {
    const String body = String("{\"ok\":true,\"board\":\"ESP32-4848S040\",\"wifi\":") +
      (WiFi.status() == WL_CONNECTED ? "true" : "false") +
      ",\"backend\":" + (backendAvailable ? "true" : "false") +
      ",\"pending\":" + (lastCommandId.length() ? "true" : "false") +
      ",\"ip\":\"" + WiFi.localIP().toString() + "\"}";
    web.send(200, "application/json", body);
  });
  web.on("/api/backend-health", HTTP_POST, [] {
    web.send(checkBackendHealth() ? 200 : 502, "application/json", lastBackendMessage);
  });
  web.on("/api/3c", HTTP_POST, [] {
    const int code = send3CCommand(web.arg("text"));
    web.send(code == 200 || code == 202 ? 202 : 502, "application/json", lastBackendMessage);
  });
  web.onNotFound([] { web.send(404, "application/json", "{\"error\":\"not found\"}"); });
  web.begin();
}

void connectWifi() {
  if (!strlen(app_config::wifiSsid)) {
    updatePanel(PanelState::Offline, "Configure local_config.h");
    Serial.println("Configure include/local_config.h antes de usar Wi-Fi.");
    return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(app_config::deviceId);
  WiFi.begin(app_config::wifiSsid, app_config::wifiPassword);
  lastWifiAttempt = millis();
  updatePanel(PanelState::Busy, "Conectando Wi-Fi");
}

void handleTouch() {
  const TouchSample sample = readTouch();
  if (!sample.ready) return;
  if (sample.touched && !touchDown && sample.y >= 350) {
    if (sample.x < 240) checkBackendHealth();
    else send3CCommand(app_config::defaultCommand);
  }
  touchDown = sample.touched;
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.printf("ESP32-4848S040 3C | PSRAM: %s | %u bytes\n",
    psramFound() ? "OK" : "NO", ESP.getPsramSize());

  displayReady = initializeDisplay();
  if (!displayReady) Serial.println("No se pudo inicializar la pantalla ST7701.");
  Wire.begin(pins::touchSda, pins::touchScl, 400000);
  audioReady = initializeAudio();
  updatePanel(PanelState::Booting, "Hardware inicializado");
  playTone(520, 60);
  connectWifi();
  configureWebServer();
}

void loop() {
  web.handleClient();
  handleTouch();

  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiAnnounced) {
      wifiAnnounced = true;
      Serial.printf("Wi-Fi listo: http://%s/\n", WiFi.localIP().toString().c_str());
      if (!mdnsReady) {
        mdnsReady = MDNS.begin("esp32-panel-3c");
        if (mdnsReady) MDNS.addService("http", "tcp", 80);
      }
      checkBackendHealth();
    }
    if (lastCommandId.length() && millis() - lastCommandPoll >= app_config::commandPollMs) {
      lastCommandPoll = millis();
      pollCommandStatus();
    }
    if (!lastCommandId.length() && millis() - lastHealthCheck >= app_config::healthCheckMs) {
      lastHealthCheck = millis();
      checkBackendHealth();
    }
  } else {
    wifiAnnounced = false;
    if (strlen(app_config::wifiSsid) && millis() - lastWifiAttempt >= app_config::wifiRetryMs) {
      lastWifiAttempt = millis();
      WiFi.disconnect();
      WiFi.begin(app_config::wifiSsid, app_config::wifiPassword);
      updatePanel(PanelState::Busy, "Reconectando Wi-Fi");
    }
  }
  delay(5);
}

#endif  // BOARD_PANEL_4848S040
