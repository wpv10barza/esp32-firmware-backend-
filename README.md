# ESP32-S3-4848S040 Firmware + Backend Bridge - T-030

## 1. Condiciones iniciales

### 1.1 Plataforma electrónica

El sistema se desarrolló sobre el panel ESP32-S3-4848S040. Este dispositivo integra un microcontrolador ESP32-S3 con memoria Flash y memoria PSRAM. Incorpora una pantalla RGB con controlador ST7701 y un sistema táctil GT911 con comunicación I2C. La gestión gráfica se realizó mediante Arduino-GFX y la conectividad mediante Wi-Fi.

### 1.2 Conexiones principales del panel

La arquitectura electrónica considera las conexiones de retroiluminación, control de pantalla y comunicación táctil. El detalle de las conexiones se presenta en el Anexo A. La resolución lógica de trabajo definida fue de 480 × 480 píxeles.

### 1.3 Condiciones de software

El entorno de desarrollo se configuró en Ubuntu sobre WSL2. Se emplearon Python, PlatformIO y Git. Para el acceso físico se utilizó USB/IP. El backend 3C se ejecutó de forma externa y la comunicación se estableció mediante HTTP. El dispositivo no realiza escritura directa sobre la fuente maestra.

### 1.4 Configuración local

Previo a la compilación se requirió la creación del archivo de configuración local a partir del archivo de referencia correspondiente. En dicha configuración se definieron parámetros del entorno, como las credenciales de red y la dirección del backend. Las credenciales reales no fueron versionadas por razones de seguridad.

---

## Criterio de redacción

La documentación principal del repositorio se mantiene en estilo académico y en tercera persona. La descripción general del sistema se presenta mediante párrafos técnicos, mientras que los detalles de implementación, código fuente, comandos, asignaciones de pines y procedimientos operativos se reservan para los anexos correspondientes.