# T-030 Condiciones Iniciales Unificadas: Firmware 4848S040 + Backend Asistente 3C

## 1. Condiciones iniciales

### 1.1 Plataforma electrónica - Firmware

El sistema se desarrolló sobre el panel ESP32-S3-4848S040. La configuración del objetivo de firmware establece un microcontrolador ESP32-S3 con memoria Flash de 16 MB y PSRAM OPI. La interfaz de visualización se implementó sobre un panel RGB de 480 × 480 píxeles con controlador ST7701S, mientras que la interacción táctil se realizó mediante el controlador GT911. La gestión gráfica se desarrolló mediante Arduino-GFX y la conectividad se estableció mediante Wi-Fi.

### 1.2 Conexiones principales del panel

La arquitectura electrónica considera la retroiluminación, el control de la pantalla y la comunicación del sistema táctil mediante el bus correspondiente. El detalle de las conexiones eléctricas y de las asignaciones de pines se presenta en el Anexo A. La resolución lógica de trabajo definida para la interfaz fue de 480 × 480 píxeles.

### 1.3 Condiciones de software - Backend Asistente 3C

El entorno de desarrollo se configuró para Ubuntu sobre WSL2, utilizando Git y PlatformIO para la construcción del firmware. La configuración del objetivo de firmware emplea la plataforma Espressif32 y el framework Arduino, con una dependencia gráfica basada en Arduino-GFX. El acceso físico al dispositivo se considera mediante la infraestructura USB disponible para el entorno de desarrollo.

El backend correspondiente al Asistente 3C se encuentra implementado como una aplicación web basada en Express y TypeScript. La aplicación incorpora una interfaz de procesamiento de comandos, un servicio de comunicación con el dispositivo y mecanismos de revisión de propuestas antes de su aplicación sobre la información de mantenimiento. La inspección del repositorio confirmó el uso de Google GenAI para la interpretación estructurada de comandos y la utilización de variables de entorno para la configuración de la aplicación.

La arquitectura del Asistente 3C establece una comunicación desacoplada entre el firmware y el backend. El dispositivo transmite las solicitudes al servicio de aplicación y el backend administra su procesamiento y su estado. La propuesta permanece sujeta a revisión humana antes de que se considere aplicada. De esta manera, el firmware no realiza escritura directa sobre la fuente maestra de datos.

El control humano forma parte de las condiciones iniciales del sistema mediante un flujo de propuesta, revisión, aprobación o rechazo. El backend mantiene estados diferenciados para las solicitudes y dispone de mecanismos de identificación e idempotencia que permiten controlar solicitudes repetidas. La protección de las comunicaciones se apoya en un token compartido entre el dispositivo y el servicio.

### 1.4 Fuente de datos y persistencia

El repositorio actual del Asistente 3C está orientado a la interacción con Google Sheets y a la validación de cambios sobre la estrategia 3C. La inspección realizada no identificó dependencias de Google Cloud Storage, variables denominadas `BUCKET_NAME` ni componentes que permitan afirmar la existencia de una capa de buckets de GCS en esta versión del backend. Por ello, la documentación de esta versión considera Google Sheets como fuente maestra y reserva la incorporación de almacenamiento mediante buckets para una implementación posterior que deberá quedar respaldada por código y configuración verificables.

### 1.5 Configuración local unificada

Previo a la compilación del firmware se requiere una configuración local derivada del archivo de referencia correspondiente. En dicha configuración se definen parámetros del entorno, entre ellos las credenciales de red, la dirección del backend, el identificador del dispositivo y el token de autenticación. Los valores reales se excluyen del control de versiones.

Para el backend se utiliza una configuración local mediante variables de entorno que contempla la clave de Google GenAI, el modelo de inferencia, la identificación de la hoja de cálculo, la hoja de trabajo, la fila de encabezados, el puerto del servicio y el token de autenticación del dispositivo. Los secretos reales permanecen fuera del repositorio.

La comunicación con el dispositivo debe utilizar la dirección de red accesible desde el panel y no una dirección de bucle local exclusiva del equipo donde se ejecuta el backend. La arquitectura de despliegue considera que el servicio se encuentra disponible para el dispositivo a través de la red local.

### 1.6 Condiciones iniciales adicionales

El backend Asistente 3C actúa como intermediario obligatorio entre el firmware y la fuente maestra. La validación de la estructura de datos, la identificación de la tarea y la revisión humana constituyen condiciones de operación previas a la aplicación de modificaciones. El firmware se mantiene como cliente del servicio y la lógica de interpretación, validación y control de cambios permanece centralizada en el backend.

---

## Criterio de redacción

La documentación principal del repositorio se mantiene en estilo académico y en tercera persona. La descripción general del sistema se presenta mediante párrafos técnicos, mientras que el código fuente, los comandos de instalación y ejecución, las asignaciones de pines, las rutas de servicio y los procedimientos operativos se reservan para los anexos correspondientes.

## Fuentes técnicas de referencia

La descripción del hardware se contrastó con la documentación técnica de Espressif Systems y con la documentación disponible para los controladores ST7701S y GT911. La descripción de software y del flujo 3C se contrastó directamente con la configuración y el código del repositorio de firmware y con el repositorio Asistente 3C en su rama principal.
