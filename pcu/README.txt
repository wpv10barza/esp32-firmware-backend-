PCU 4848 — ESP32-S3-4848S040 + backend 3C mDNS
==================================================

OBJETIVO
- Detectar el panel cuando se conecta por USB.
- Recordar el BUSID fisico de Windows (por ejemplo 1-1 o 2-2).
- Adjuntar automaticamente ese USB a Ubuntu/WSL con usbipd.
- Mostrar el puerto serie Linux (/dev/ttyACM* o /dev/ttyUSB*) y la topologia USB.
- Detectar la IP del panel por esp32-panel-3c.local, /health o el log serie.
- Compilar y flashear el firmware validado sin subir SSID, password ni token a GitHub.

WINDOWS + WSL2
1. Conecte el ESP32-S3-4848S040 por USB.
2. Abra PCU-4848.cmd.
3. PCU se eleva, detecta el ESP32 y recuerda el BUSID fisico.
4. PCU adjunta el dispositivo a WSL y muestra el estado desde Ubuntu.
5. En el menu, F flashea, M abre monitor y B hace ambas cosas.

No hace falta escribir manualmente comandos PowerShell, CMD ni Python.
Si hay varios adaptadores serie conectados, PCU pide seleccionar uno para evitar flashear un equipo equivocado.

UBUNTU NATIVO
Ejecute pcu-4848.sh. Con --flash compila/flashea y con --monitor abre el monitor serie.

CONFIGURACION PRIVADA
La primera vez que se selecciona flasheo, PCU pide:
- SSID Wi-Fi 2.4 GHz
- password Wi-Fi
- token del Device API 3C (si se usa)
- Device ID (opcional; hay valor predeterminado)
- audio y brillo

Estos datos se guardan SOLO en ~/.config/pcu-4848/local_config.h con permisos 600.
El source incluido en el paquete y GitHub permanecen sin secretos.

CONEXION FINAL
- Panel mDNS: esp32-panel-3c.local
- Backend logico: 3c-backend.local
- Servicio backend: _3c._tcp
- El firmware conserva en NVS la ultima direccion/puerto valido del backend y redescubre por mDNS si falla health.

FIRMWARE
El paquete contiene un firmware generico compilado por GitHub Actions para verificacion/recovery y un source minimo validado.
Para tener Wi-Fi/IP/backend funcionales, PCU recompila localmente ese mismo source SHA usando la configuracion privada de Ubuntu/WSL antes de flashear.

SEGURIDAD
- Nunca flashea automaticamente solo por conectar el USB.
- Nunca selecciona silenciosamente entre varios dispositivos ambiguos.
- No publica credenciales en GitHub.
- No usa git force ni modifica el backend durante el flasheo.
