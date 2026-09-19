# Guition monitor UI transfer

This panel firmware keeps the 3C device/API behavior from esp32-firmware-backend- and ports the visual language of alaltitov/Guition-ESP32-S3-4848S040, branch 2026.8.2, rather than copying the ESPHome application wholesale.

## Ported visual model

The reference firmware uses a 480x480 ST7701S RGB display and GT911 touch panel. Its visual palette defines slate blue/gray, steel blue, misty blue, white, sky blue, mint, amber and related status colors. The target now exposes that palette through include/guition_monitor_theme.h.

The 3C normal screen uses the same compact dark monitor language while keeping the existing status machine and command field. The action buttons reuse the same geometry for drawing and touch dispatch, so the visible and interactive regions cannot silently diverge.

## 480x480 acceptance contract

- LCD surface: exactly 480x480.
- PROBAR WSL: x=20..229, y=370..451.
- ENVIAR 3C: x=250..459, y=370..451.
- 20px non-interactive gap between buttons.
- 28px non-interactive footer at the bottom.
- Labels must fit the button width at the fixed text size.
- Touch coordinates outside those half-open rectangles do nothing.
- The boot-time RGB diagnostic is not executed automatically; it previously blocked the UI for about 1.6 seconds.

## Hardware note

GitHub Actions can verify geometry, routing and buildability. It cannot prove that a physical GT911 reports a finger press or that a particular LCD unit is optically legible. The final field check must flash the panel and touch both buttons near their left/right boundaries while observing the screen and serial log.
