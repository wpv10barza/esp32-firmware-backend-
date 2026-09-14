# ESP32-S3-4848S040 Firmware + Backend Bridge

Firmware reproducible para el panel cuadrado **ESP32-S3-4848S040 (480×480)** conectado al backend 3C que se ejecuta en Ubuntu/WSL.

## Alcance verificable

- compila firmware para ESP32-S3 N16R8 con pantalla ST7701 y táctil GT911;
- consume el contrato HTTP `/api/device/v1/health`, `POST /api/device/v1/commands` y `GET /api/device/v1/commands/{command_id}`;
- muestra `pendiente`, `aplicado`, `rechazado` y `error` en la pantalla;
- conserva la confirmación humana antes de modificar Google Sheets;
- publica binarios y manifiesto SHA-256 desde GitHub Actions;
- despliega un sitio de estado documental mediante GitHub Pages.

GitHub Actions demuestra **compilación**, no carga ni validación física. La prueba física requiere el panel conectado por USB.

## Uso directo en Ubuntu/WSL

```bash
git clone https://github.com/wpv10barza/esp32-firmware-backend-.git
cd esp32-firmware-backend-
cp include/local_config.example.h include/local_config.h
nano include/local_config.h
python3 -m pip install platformio==6.1.18
pio run -e panel_4848s040
./scripts/flash-panel.sh
```

El script `scripts/flash-panel.sh` compila primero y después detecta automáticamente un único `/dev/ttyACM*` o `/dev/ttyUSB*`. También permite fijar el puerto:

```bash
./scripts/flash-panel.sh --port /dev/ttyACM0
./scripts/flash-panel.sh --port /dev/ttyUSB0
```

No es necesario crear ni activar `.venv` para este procedimiento. El comando `pio` funciona si PlatformIO está instalado en el entorno Python activo.

### USB de ESP32 hacia WSL 2

Si `pio device list` no muestra ningún puerto y no existen `/dev/ttyACM*` ni `/dev/ttyUSB*`, el problema está antes de PlatformIO: WSL no tiene acceso al USB del ESP32. En Windows, verifique el dispositivo con `usbipd list` y, desde una consola de Windows, adjúntelo a WSL con:

```powershell
usbipd list
usbipd attach --wsl --busid <BUSID>
```

Después, dentro de WSL, compruebe:

```bash
lsusb
pio device list
ls -l /dev/ttyACM* /dev/ttyUSB*
```

El `BUSID` es el identificador mostrado por `usbipd list`. Si el dispositivo aparece en Windows pero no en WSL, no fuerce `/dev/ttyUSB0`: el ESP32-S3 puede exponerse como otro puerto serie, por ejemplo `/dev/ttyACM0`. `pio device list` es la referencia para seleccionar el puerto real. citeturn429716search3turn429716search0

En `local_config.h`, use la **IPv4 LAN de Windows** para el backend. Un ESP32 físico no puede acceder a `127.0.0.1` de WSL.

> Seguridad: no versionar Wi-Fi, token, credenciales de Google ni identificadores privados. El ESP32 nunca escribe directamente en Sheets; solicita una vista previa al backend.
