# ESP32-S3-4848S040 Firmware + Backend Bridge

Firmware reproducible para el panel cuadrado **ESP32-S3-4848S040 (480×480)**, integrando la parte electrónica del panel con el firmware de control, la interfaz táctil y el backend 3C ejecutado en Ubuntu/WSL.

El sistema separa claramente:

- la capa electrónica del dispositivo;
- la interfaz gráfica y táctil;
- la lógica de firmware;
- la comunicación con el backend;
- la confirmación humana antes de modificar Google Sheets.

## 1. Condiciones iniciales

### 1.1 Plataforma electrónica

El sistema está diseñado para el panel **ESP32-S3-4848S040** con:

- microcontrolador ESP32-S3;
- memoria Flash de 16 MB;
- PSRAM OPI;
- pantalla RGB de **480×480 píxeles**;
- controlador LCD **ST7701**;
- controlador táctil **GT911**;
- comunicación táctil mediante I²C;
- interfaz gráfica mediante Arduino-GFX;
- interfaz de red mediante Wi-Fi;
- interfaz de audio I²S opcional.

La configuración de PlatformIO utiliza:

```text
Board: esp32-s3-devkitm-1
Framework: Arduino
Platform: espressif32@6.8.1
Flash: 16 MB
PSRAM: OPI
Monitor serie: 115200 baud
```

### 1.2 Arquitectura funcional

La solución se organiza en capas para mantener separadas las responsabilidades de hardware, interfaz, control y servicios:

```text
ESP32-S3-4848S040
├── Hardware
│   ├── ESP32-S3
│   ├── LCD ST7701
│   ├── Touch GT911 / I²C
│   └── Wi-Fi / I²S opcional
│
├── Firmware
│   ├── inicialización del dispositivo
│   ├── interfaz gráfica
│   ├── eventos táctiles
│   ├── editor / commandBuffer
│   └── comunicación HTTP
│
└── Backend 3C en Ubuntu/WSL
    ├── recepción de comandos
    ├── validación
    ├── vista previa
    ├── confirmación humana
    └── aplicación controlada sobre Google Sheets
```

El ESP32 actúa como cliente del servicio. La lógica de modificación de datos permanece en el backend y la escritura en Google Sheets requiere confirmación humana.

## 2. Alcance verificable

- compila firmware para ESP32-S3 N16R8 con pantalla ST7701 y táctil GT911;
- consume el contrato HTTP `/api/device/v1/health`, `POST /api/device/v1/commands` y `GET /api/device/v1/commands/{command_id}`;
- muestra `pendiente`, `aplicado`, `rechazado` y `error` en la pantalla;
- conserva la confirmación humana antes de modificar Google Sheets;
- publica binarios y manifiesto SHA-256 desde GitHub Actions;
- despliega un sitio de estado documental mediante GitHub Pages.

GitHub Actions demuestra **compilación**, no carga ni validación física. La prueba física requiere el panel conectado por USB.

## 3. Uso directo en Ubuntu/WSL

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

### 3.1 USB de ESP32 hacia WSL 2

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

El `BUSID` es el identificador mostrado por `usbipd list`. Si el dispositivo aparece en Windows pero no en WSL, no fuerce `/dev/ttyUSB0`: el ESP32-S3 puede exponerse como otro puerto serie, por ejemplo `/dev/ttyACM0`. `pio device list` es la referencia para seleccionar el puerto real.

En `local_config.h`, use la **IPv4 LAN de Windows** para el backend. Un ESP32 físico no puede acceder a `127.0.0.1` de WSL.

> Seguridad: no versionar Wi-Fi, token, credenciales de Google ni identificadores privados. El ESP32 nunca escribe directamente en Sheets; solicita una vista previa al backend.

## 4. E2E real contra Google Sheets en GitHub Actions

Este repositorio separa dos niveles de validación:

1. El workflow normal comprueba firmware, teclado virtual, viewport, `commandBuffer` y el contrato HTTP del Monitor.
2. `.github/workflows/e2e-real-google-sheets.yml` ejecuta el **Monitor real** de `wpv10barza/asistente-3c`, crea un comando mediante la misma API que usa el ESP32, abre la interfaz web en modo E2E, obtiene la vista previa, confirma el cambio y verifica una escritura real en Google Sheets. Al finalizar restaura exactamente las columnas `L` y `M` modificadas por la prueba.

La CI no puede convertir un runner de GitHub en un ESP32 físico. Por ello, la prueba E2E reproduce exactamente el contrato HTTP que ejecuta `send3CCommand()` y `pollCommandStatus()`, mientras que Google Sheets sí es el servicio real.

### 4.1 Secretos requeridos

Configurar en **Settings → Secrets and variables → Actions → Secrets** del repositorio:

- `GOOGLE_SERVICE_ACCOUNT_JSON`: JSON completo de una cuenta de servicio de Google con acceso de edición a la hoja de prueba.
- `E2E_SPREADSHEET_ID`: ID del spreadsheet usado exclusivamente para la prueba E2E.
- `E2E_SHEET_NAME`: nombre real de la hoja dentro del spreadsheet.
- `E2E_TASK_NAME`: valor **único** de la columna `F=Nombre` de una fila dedicada a CI.
- `GEMINI_API_KEY`: clave utilizada por el Monitor para interpretar el comando real.

La fila indicada por `E2E_TASK_NAME` debe ser una fila de prueba dedicada. La prueba verifica los encabezados `E=TareaId`, `F=Nombre`, `L=Frecuencia` y `M=UnidadTiempo`, guarda el estado original de `L:M`, ejecuta el comando `Cambia la tarea <E2E_TASK_NAME> a mensual`, comprueba `L=1` y `M=Mes`, verifica que `A:O` no cambió fuera de esas dos columnas y finalmente restaura los valores originales.

La cuenta de servicio debe tener acceso directo al spreadsheet. No se copia la credencial al repositorio y el workflow nunca imprime su contenido.
