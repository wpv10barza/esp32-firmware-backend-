# T-030 ESP32-S3-4848S040 Firmware + Backend Asistente 3C - Condiciones Iniciales V2 Avanzada

## 1. Condiciones iniciales

### 1.1 Plataforma electrónica

El sistema se desarrolló sobre el panel ESP32-S3-4848S040. La configuración del objetivo de firmware establece un microcontrolador ESP32-S3 con memoria Flash de 16 MB y PSRAM OPI. La interfaz de visualización se implementó sobre un panel RGB de 480 × 480 píxeles con controlador ST7701S, mientras que la interacción táctil se realizó mediante el controlador GT911. La gestión gráfica se desarrolló mediante Arduino-GFX y la conectividad se estableció mediante Wi-Fi. La configuración del objetivo utiliza la plataforma Espressif32 y el framework Arduino.

### 1.2 Conexiones principales

La arquitectura electrónica considera la retroiluminación, el control de la pantalla RGB y la interfaz táctil mediante el bus correspondiente. El detalle específico de las conexiones eléctricas y de las asignaciones de pines se reserva para el Anexo A. La resolución lógica de trabajo establecida para la interfaz es de 480 × 480 píxeles.

### 1.3 Condiciones de software - Firmware

El entorno de desarrollo se configuró para Ubuntu sobre WSL2, utilizando Git y PlatformIO para la construcción del firmware. El firmware implementa un cliente Wi-Fi con reconexión automática y un cliente HTTP para la comunicación con el backend. También incorpora verificación periódica de disponibilidad del servicio y consulta periódica del estado de las órdenes pendientes. El firmware no realiza escritura directa sobre la fuente maestra de datos.

La identificación de las solicitudes utiliza un identificador de dispositivo y un identificador de solicitud generado por el firmware. El backend utiliza esta información para detectar solicitudes duplicadas y conservar la idempotencia del flujo. Esta condición permite evitar la creación repetida de una misma orden cuando se recibe nuevamente la misma identificación de solicitud.

### 1.4 Condiciones de software - Backend Asistente 3C

El backend se desarrolló en el repositorio `asistente-3c` como una aplicación web basada en TypeScript y Express ejecutada sobre Node.js. Esta caracterización corrige la descripción previa que asociaba el backend con Python. El repositorio utiliza Google GenAI para la interpretación estructurada de comandos y mantiene la lógica de configuración mediante variables de entorno.

La aplicación dispone de una API específica para la comunicación con dispositivos y de una capa de revisión de propuestas. La interfaz de dispositivo permite verificar la disponibilidad del servicio, recibir órdenes y consultar su estado. La información del dispositivo se autentica mediante un token y las solicitudes se mantienen en estados diferenciados, incluyendo la condición de pendiente de confirmación, aplicación y rechazo.

La interpretación del comando mediante Google GenAI no implica la aplicación automática del cambio. El backend realiza validaciones deterministas sobre los campos permitidos, las columnas auditadas, los tipos de valores y los catálogos disponibles. Posteriormente, la interfaz web presenta una vista previa que debe ser revisada por una persona antes de realizar la modificación de los datos.

### 1.5 Fuente de datos y mecanismo de escritura

La fuente maestra utilizada por esta versión corresponde a Google Sheets. La inspección del repositorio mostró que la interfaz web accede a Google Sheets mediante la API REST de Google y un token obtenido mediante autenticación OAuth 2.0. El navegador consulta los encabezados y catálogos de la hoja, localiza la tarea y, después de la confirmación humana, ejecuta las actualizaciones autorizadas.

En consecuencia, el backend Asistente 3C actúa como intermediario de interpretación, validación, cola de órdenes y control de la revisión, mientras que la interfaz web autenticada realiza el acceso de lectura y escritura sobre Google Sheets. El firmware no accede directamente a Google Sheets.

La inspección de la versión actual no identificó dependencias de `googleapis`, `google-auth-library`, Google Cloud Storage, `google-cloud-storage` ni una variable `BUCKET_NAME`. Tampoco se identificó el uso de una cuenta de servicio mediante `GOOGLE_SERVICE_ACCOUNT_JSON`. Por ello, dichos elementos no se consideran condiciones iniciales de esta versión y cualquier incorporación futura deberá sustentarse mediante implementación verificable.

### 1.6 Condiciones de red e infraestructura

La comunicación entre el firmware y el backend requiere conectividad Wi-Fi en la misma infraestructura de red local que permita al dispositivo alcanzar el equipo donde se expone el servicio. El backend se configura para escuchar en todas las interfaces de red del host, de modo que el servicio pueda ser alcanzado por el panel desde la red local.

La configuración del firmware debe utilizar una dirección de red accesible desde el dispositivo. Una dirección de bucle local exclusiva del equipo donde se ejecuta el backend no constituye una dirección válida para el acceso desde el ESP32. En el entorno WSL2, la disponibilidad efectiva del servicio depende además de la modalidad de red utilizada y de las reglas de acceso del sistema anfitrión.

La infraestructura de software requiere Ubuntu/WSL2 para el entorno de desarrollo, PlatformIO para la construcción del firmware y Node.js con TypeScript para la ejecución del backend. La interfaz web requiere autorización de la cuenta de Google con alcance de acceso a Google Sheets para realizar la lectura y escritura sobre la hoja configurada.

### 1.7 Condiciones de seguridad

Las credenciales de red, los tokens de autenticación y las claves de servicios externos se mantienen fuera del control de versiones. El firmware utiliza un archivo de configuración local y el backend utiliza variables de entorno. Los valores reales no se almacenan en el repositorio.

La comunicación entre el firmware y el backend se realiza actualmente mediante HTTP dentro de la infraestructura de red local. Esta condición implica que la protección del canal no se basa en TLS en esta versión y constituye una consideración para una implementación futura cuando el sistema se traslade a redes que requieran cifrado en tránsito.

La autenticación del dispositivo mediante token y la revisión humana obligatoria constituyen mecanismos complementarios de control. La existencia de una orden recibida por el backend no autoriza por sí misma la modificación de la fuente maestra.

### 1.8 Flujo de datos como condición inicial

El flujo funcional se inicia en el panel mediante una interacción táctil o mediante la interfaz local disponible para introducir una orden. El firmware construye la solicitud y la transmite al backend Asistente 3C, donde se verifica la autenticación y se registra la orden en estado pendiente de confirmación.

Posteriormente, la interfaz web recibe la orden, consulta la configuración de la hoja y audita sus encabezados y catálogos. El comando se interpreta mediante Google GenAI y el resultado se somete a validaciones deterministas. La interfaz localiza una única tarea y genera una propuesta de cambios para revisión humana.

La aplicación de modificaciones ocurre únicamente después de la confirmación de la propuesta. Una vez ejecutada la actualización sobre Google Sheets, el resultado se comunica al backend y el firmware consulta el estado de la orden para reflejar en pantalla la condición correspondiente. El flujo desacoplado mantiene separadas la interfaz física, la lógica de procesamiento y la fuente maestra.

---

## Criterio de redacción

La documentación principal del repositorio se mantiene en estilo académico y en tercera persona. La descripción general del sistema se presenta mediante párrafos técnicos, mientras que el código fuente, los comandos de instalación y ejecución, las asignaciones de pines, las rutas completas de servicio y los procedimientos operativos se reservan para los anexos correspondientes.

## Fuentes técnicas de referencia

La descripción del firmware se contrastó con la configuración y el código del repositorio `wpv10barza/esp32-firmware-backend-`. La descripción del backend y del flujo de integración se contrastó con la configuración, el código del servidor, la API de dispositivo y la interfaz web del repositorio `wpv10barza/asistente-3c`. La descripción de los componentes del panel se mantiene alineada con la documentación técnica correspondiente de ESP32-S3, ST7701S y GT911.

## Versión documental

V2 Avanzada. Esta versión amplía las condiciones iniciales con aspectos de red, infraestructura, seguridad y flujo de datos, y corrige la caracterización tecnológica del backend a TypeScript/Express con acceso web autenticado a Google Sheets. Se excluyen componentes no sustentados por la implementación actual, entre ellos Google Cloud Storage y cuentas de servicio para Google Sheets.
