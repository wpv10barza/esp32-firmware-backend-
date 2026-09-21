# T-030 Sistema Completo ESP32-S3-4848S040 + Backend Asistente 3C - Condiciones Iniciales V4

## 1. Condiciones iniciales del sistema de desarrollo

El sistema de desarrollo comprende dos subsistemas obligatorios para la operación 3C: el firmware del panel ESP32-S3-4848S040 y el backend Asistente 3C. Ambos subsistemas se integran mediante una interfaz de comunicación HTTP y mantienen separadas la interacción física, el procesamiento de comandos, la revisión humana y el acceso a la fuente maestra de información.

### 1.1 Subsistema firmware - Condición inicial obligatoria 1

El subsistema firmware se desarrolló sobre el panel ESP32-S3-4848S040. La configuración del objetivo de compilación establece un microcontrolador ESP32-S3, memoria Flash de 16 MB y PSRAM OPI, con plataforma Espressif32 y framework Arduino. La interfaz de visualización se implementó sobre una pantalla RGB de 480 × 480 píxeles con controlador ST7701S, mientras que la interacción táctil se realizó mediante el controlador GT911. La gestión gráfica se efectuó mediante Arduino-GFX y la conectividad del dispositivo se estableció mediante Wi-Fi.

El firmware incorpora una máquina de estados para representar las condiciones de operación del panel, un búfer de comandos con control de cursor, edición de texto, inserción y eliminación, y un teclado virtual para la introducción de órdenes. El subsistema también implementa un cliente HTTP para consultar la disponibilidad del backend, transmitir órdenes y consultar su resultado.

La identificación de las solicitudes utiliza un identificador del dispositivo y un identificador de solicitud. El backend emplea esta información para detectar duplicados y mantener el control idempotente de las órdenes. El firmware no ejecuta escritura directa sobre Google Sheets; su función de integración se limita a la interacción con el servicio 3C y a la representación del estado del proceso.

### 1.2 Subsistema backend Asistente 3C - Condición inicial obligatoria 2

El subsistema backend se desarrolló en el repositorio asistente-3c como una aplicación web basada en TypeScript y Express ejecutada sobre Node.js. El proyecto utiliza Google GenAI para la interpretación estructurada de comandos y variables de entorno para la configuración del servicio.

La API del dispositivo dispone de mecanismos para comprobar la disponibilidad del servicio, recibir órdenes, consultar órdenes pendientes y consultar el estado de una orden. La autenticación del dispositivo se realiza mediante un token y el procesamiento de las solicitudes mantiene estados diferenciados de pendiente de confirmación, aplicación y rechazo.

El backend incorpora una capa de revisión de propuestas con bloqueo temporal de la fila involucrada. La propuesta de cambio se mantiene en estado de revisión hasta que se aprueba o se rechaza, evitando que dos propuestas concurrentes modifiquen simultáneamente la misma fila durante el período de revisión.

### 1.3 Interpretación, validación y control humano

La interpretación de comandos se realiza mediante Google GenAI con una salida estructurada. El resultado de la inferencia no se aplica directamente sobre la fuente maestra. Antes de una actualización, se verifica la correspondencia entre encabezados, campos autorizados, tipos de valores y catálogos existentes.

La aplicación utiliza una estrategia de validación determinista posterior a la interpretación. Entre las reglas verificadas se encuentran la identificación de la tarea, la restricción de las columnas modificables, la validación de frecuencia y unidad de tiempo, y la correspondencia de los valores de catálogo con registros existentes.

El control humano en el circuito constituye una condición inicial del sistema. La interfaz presenta una vista previa de las operaciones y requiere una acción explícita de confirmación para ejecutar los cambios. También se permite el rechazo de la propuesta, después del cual el sistema informa el resultado al dispositivo cuando la orden se originó en el panel.

### 1.4 Fuente maestra y acceso a Google Sheets

La fuente maestra de esta versión corresponde a Google Sheets. La interfaz web obtiene un token de acceso mediante Google OAuth 2.0 con alcance de hojas de cálculo y utiliza la API REST de Google Sheets para consultar encabezados, obtener catálogos, localizar tareas y actualizar las celdas autorizadas.

El backend no contiene una implementación de escritura directa mediante una cuenta de servicio de Google Sheets. Su función consiste en procesar comandos, mantener el estado de las órdenes, realizar la interpretación y validación, y coordinar la revisión humana. La escritura sobre la fuente maestra se ejecuta desde la interfaz web autenticada después de la confirmación.

La inspección de la versión actual no identificó dependencias de googleapis, google-auth-library, google-cloud-storage, una variable BUCKET_NAME ni una variable GOOGLE_SERVICE_ACCOUNT_JSON. Por tanto, Google Cloud Storage, buckets y autenticación mediante cuenta de servicio no se consideran condiciones iniciales de esta versión.

### 1.5 Conexiones principales como condición inicial del sistema completo

La arquitectura electrónica del firmware considera la pantalla RGB, la retroiluminación, la interfaz táctil y los elementos auxiliares definidos para la variante del panel. Las asignaciones de pines y el detalle eléctrico se reservan para el Anexo A. La resolución lógica de la interfaz es de 480 × 480 píxeles.

La arquitectura lógica requiere conectividad Wi-Fi entre el panel y el equipo que expone el backend. El servicio debe encontrarse accesible desde la red donde opera el dispositivo. Para el acceso a la fuente maestra, la interfaz web se comunica con los servicios HTTPS de Google mediante la API de Google Sheets.

### 1.6 Condiciones de software del sistema completo

El entorno de firmware se construye con PlatformIO y el framework Arduino. La configuración actual del objetivo utiliza Espressif32 6.8.1 y Arduino-GFX 1.5.9. La versión concreta del ejecutable de PlatformIO utilizada en una estación de trabajo no se fija en este README porque no se encuentra declarada en platformio.ini.

El backend se ejecuta mediante Node.js, TypeScript, Express y Vite. La configuración del proyecto define scripts de desarrollo, compilación, inicio, análisis estático y pruebas automatizadas. La interfaz web utiliza React y un mecanismo de autenticación Google OAuth para obtener el acceso requerido a Google Sheets.

### 1.7 Condiciones de red e infraestructura

La operación física requiere una red Wi-Fi con alcance entre el panel y el equipo donde se encuentra disponible el backend. El firmware debe utilizar una dirección de red accesible desde el dispositivo; una dirección de bucle local exclusiva del equipo de desarrollo no permite que el panel alcance el servicio.

El backend se configura para escuchar sobre todas las interfaces de red del host. En un entorno WSL2, el acceso efectivo desde el panel depende de la modalidad de red y de las reglas de exposición del sistema anfitrión. Por ello, la conectividad física debe verificarse antes de considerar operativo el flujo 3C.

La infraestructura de software requiere un entorno Ubuntu/WSL2 para el desarrollo del firmware y la ejecución del backend, además de un navegador capaz de realizar la autenticación Google OAuth y acceder a la API de Google Sheets.

### 1.8 Condiciones de seguridad

Las credenciales de red, el token de autenticación del dispositivo, la clave de Google GenAI y los demás parámetros sensibles se gestionan mediante archivos de configuración locales o variables de entorno y no deben incorporarse al control de versiones.

La comunicación entre el firmware y el backend se realiza mediante HTTP dentro de la infraestructura de red local en la versión actual. Por separado, el acceso de la interfaz web a Google Sheets se realiza mediante HTTPS y autenticación OAuth. La ausencia de TLS en el enlace local constituye una condición conocida de esta implementación y una consideración para futuras mejoras de despliegue.

La autenticación mediante token del dispositivo se complementa con la revisión humana obligatoria. La recepción de una orden no implica autorización automática para modificar la fuente maestra.

### 1.9 Flujo de datos como condición inicial

El flujo de operación comienza con la generación de una orden en el panel mediante interacción táctil o mediante la interfaz local de texto. El firmware construye la solicitud e identifica el dispositivo y la petición antes de enviarla al backend Asistente 3C.

El backend autentica la solicitud, la registra y la mantiene en estado de confirmación pendiente. La interfaz web consulta la orden, accede a Google Sheets mediante el token OAuth del usuario, audita la estructura de la hoja, obtiene los catálogos necesarios y envía el comando al módulo de interpretación con Google GenAI.

El resultado de la interpretación se somete a validación determinista y se presenta como una propuesta. La aplicación de los cambios ocurre únicamente después de la confirmación humana. La interfaz web actualiza las celdas autorizadas de Google Sheets y comunica el resultado al backend, mientras que el firmware consulta el estado de la orden para actualizar la representación del proceso en pantalla.

### 1.10 Contrato de integración entre ambos subsistemas

La integración entre firmware y backend se basa en un contrato de dispositivo versionado. El dispositivo dispone de operaciones para consultar la salud del servicio, enviar comandos, consultar órdenes pendientes y consultar el resultado de una orden. El contrato también contempla el estado de la orden y la exigencia de confirmación humana.

La identificación de solicitudes, el control de duplicados, el tiempo de permanencia de las órdenes y la actualización del estado forman parte del comportamiento esperado del contrato. Esta separación permite verificar de manera independiente el firmware, el backend y la interfaz web sin mezclar sus responsabilidades.

---

## Criterio de redacción

La documentación principal del repositorio se mantiene en estilo académico y en tercera persona. La descripción general del sistema se presenta mediante párrafos técnicos. El código fuente, los comandos de instalación y ejecución, las asignaciones de pines, las rutas completas de servicio y los procedimientos operativos se reservan para los anexos correspondientes.

## Fuentes técnicas de referencia

La descripción del firmware se contrastó con la configuración y el código del repositorio wpv10barza/esp32-firmware-backend-. La descripción del backend se contrastó con package.json, server.ts, la API de dispositivo, el control de revisión y la interfaz web del repositorio wpv10barza/asistente-3c. La caracterización del acceso a Google Sheets se contrastó con la implementación de Google OAuth y las solicitudes realizadas desde la interfaz web.

## Versión documental

V4. Esta versión establece explícitamente el firmware y el backend Asistente 3C como subsistemas obligatorios, amplía las condiciones iniciales de software, red, infraestructura, seguridad, flujo de datos y contrato de integración, y mantiene fuera de la documentación principal los detalles de implementación que corresponden a los anexos. Se excluyen componentes no sustentados por la versión actual, entre ellos Google Cloud Storage, buckets y cuentas de servicio para Google Sheets.
