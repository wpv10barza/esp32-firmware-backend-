# T-030 Sistema de Desarrollo Completo - Condiciones Iniciales V6 Benchmark

## 1. Condiciones iniciales del sistema de desarrollo

El sistema de desarrollo comprende dos subsistemas obligatorios e inseparables para la operación del sistema T-030: el firmware del panel ESP32-S3-4848S040 y el backend Asistente 3C. Ambos subsistemas cumplen funciones complementarias. El firmware concentra la interacción física con el usuario, la representación visual, la captura de eventos táctiles y la comunicación con el servicio. El backend concentra la recepción de órdenes, su interpretación mediante inteligencia artificial, la validación determinista, el control de la revisión humana y la coordinación del acceso a la fuente maestra. La separación de responsabilidades permite mantener un control explícito sobre las operaciones que pueden modificar la información de mantenimiento.

### 1.1 Plataforma electrónica - Subsistema firmware

El sistema se desarrolló sobre un panel ESP32-S3-4848S040 configurado como objetivo específico de compilación dentro del entorno Arduino para microcontroladores Espressif. La configuración vigente establece una memoria Flash de 16 MB y el uso de PSRAM OPI. La interfaz de usuario trabaja con una resolución lógica de 480 × 480 píxeles y emplea una pantalla RGB asociada a un controlador ST7701S y un sistema táctil capacitivo basado en GT911. La representación gráfica se implementó mediante Arduino-GFX y la conectividad del dispositivo se estableció mediante Wi-Fi.

El firmware incorpora una máquina de estados para representar las condiciones de operación del panel, diferenciando situaciones de inicio, indisponibilidad, disponibilidad, procesamiento, espera de confirmación, aplicación, rechazo y error. Esta separación permite que el estado de la comunicación y del procesamiento sea visible para el usuario sin confundir una orden recibida con una orden aplicada. La interfaz también dispone de un mecanismo de edición de comandos con cursor, inserción de caracteres, eliminación, desplazamiento y teclado virtual adaptado a la geometría de la pantalla.

La gestión de órdenes utiliza un búfer de comandos con capacidad definida y mantiene una posición de cursor independiente del contenido almacenado. El firmware genera solicitudes asociadas a un identificador de dispositivo y a un identificador de petición. Este diseño permite que el backend reconozca reenvíos de una misma petición y evite tratar una retransmisión como una nueva operación lógica. La comunicación se ejecuta mediante solicitudes HTTP hacia el servicio 3C, incluyendo mecanismos de consulta de disponibilidad, envío de comandos, consulta de órdenes pendientes y consulta de resultados.

El dispositivo no ejecuta escritura directa sobre la fuente maestra. La función del firmware consiste en capturar la interacción, estructurar la orden, transmitirla al backend y presentar al usuario el estado de su procesamiento. De esta forma, la modificación de la información queda desacoplada del equipo embebido y se conserva una barrera lógica entre la interfaz física y la persistencia de datos.

### 1.2 Plataforma backend Asistente 3C - Subsistema obligatorio

El subsistema backend se desarrolló como una aplicación web basada en Node.js, TypeScript y Express, complementada con una interfaz React ejecutada mediante Vite. La configuración vigente incorpora la biblioteca oficial de Google GenAI para realizar la interpretación estructurada de comandos. La aplicación utiliza variables de entorno para parámetros sensibles y de operación, evitando incorporar valores privados dentro del código versionado.

La arquitectura del backend se organiza alrededor de una API de dispositivo y un mecanismo de almacenamiento temporal de órdenes. Las solicitudes provenientes del panel son normalizadas y verificadas antes de ingresar a la cola. Cada orden conserva un identificador único, un identificador de petición, la identificación del dispositivo, el texto recibido y un estado de procesamiento. Los estados principales distinguen una orden pendiente de confirmación humana, una orden aplicada y una orden rechazada.

El control de acceso del dispositivo se realiza mediante un token configurado en el entorno del backend. Cuando el token requerido no está disponible y no se habilita explícitamente un modo inseguro de desarrollo, la recepción de órdenes se bloquea. Esta condición evita que una instancia del servicio quede disponible para aceptar comandos del dispositivo sin un mecanismo mínimo de autenticación.

La revisión humana se implementa mediante un mecanismo de propuestas con estados de propuesta, aprobación y rechazo. Cada propuesta identifica la fila que será revisada, los campos que se pretende modificar y el identificador de la orden que la originó cuando corresponde. Durante la revisión se mantiene un bloqueo temporal de la fila para impedir que otra propuesta concurrente modifique simultáneamente la misma posición lógica. El bloqueo expira después de un período definido, lo que evita mantener indefinidamente un recurso reservado.

### 1.3 Interpretación, validación y control humano

La interpretación de las órdenes se ejecuta mediante Google GenAI con una salida estructurada orientada a operaciones deterministas. El modelo no recibe autorización para modificar directamente la fuente maestra. Su función se limita a transformar el lenguaje natural en una estructura que contiene la tarea buscada, las operaciones solicitadas y una indicación de si la orden requiere revisión.

Después de la interpretación se ejecuta una etapa de validación determinista. En ella se verifican los campos permitidos, la correspondencia de encabezados, la validez de tipos de datos, las unidades de tiempo y la pertenencia de los valores de catálogo a los registros existentes. Las reglas restringen explícitamente las columnas modificables y bloquean operaciones que no correspondan a la estructura esperada de la estrategia. Cuando una condición no puede comprobarse, el procesamiento se detiene y la solicitud se dirige a revisión.

La interfaz humana presenta una vista previa antes de efectuar cualquier escritura. La aplicación identifica de manera controlada la fila objetivo y muestra los cambios propuestos junto con sus valores resultantes. La escritura únicamente se habilita después de una acción explícita de confirmación. La acción de rechazo cierra la propuesta sin aplicar cambios y permite comunicar al panel que la orden no fue ejecutada.

### 1.4 Fuente maestra y acceso a Google Sheets

La fuente maestra se encuentra implementada en Google Sheets. El acceso desde la interfaz web se realiza mediante autenticación OAuth de Google y solicitudes HTTPS contra la API de Google Sheets. La aplicación obtiene la configuración de la hoja, verifica la estructura esperada, consulta los encabezados reales, identifica los catálogos necesarios y localiza la tarea antes de construir una propuesta de modificación.

La aplicación cliente ejecuta directamente las operaciones de lectura y escritura sobre la API de Google Sheets utilizando el token de acceso obtenido con OAuth. El backend participa en la interpretación, la validación, el almacenamiento temporal de órdenes y el control de revisión, mientras que la interfaz autenticada realiza la persistencia efectiva una vez que la persona responsable confirma la vista previa. Esta distribución debe mantenerse diferenciada para evitar atribuir al backend una función de escritura que actualmente se ejecuta en la interfaz.

La versión actual no presenta dependencias del servicio Google Cloud Storage, de buckets ni de autenticación mediante una cuenta de servicio para la persistencia descrita. Tampoco se evidenció un backend basado en FastAPI o Flask. Por tanto, dichos componentes no se consideran condiciones iniciales de esta versión y no deben incorporarse a la descripción de las funcionalidades implementadas.

### 1.5 Conexiones principales del sistema completo

La arquitectura electrónica considera la alimentación del panel, la interfaz de visualización RGB, el sistema táctil y los elementos auxiliares definidos para la variante física empleada. El detalle de las asignaciones eléctricas se reserva para el Anexo A. La interfaz lógica mantiene una resolución de 480 × 480 píxeles para la interacción y la presentación de estados.

La arquitectura lógica requiere conectividad de red entre el panel y el equipo que expone el backend. El dispositivo debe utilizar una dirección accesible dentro de la red local; una dirección de bucle local exclusiva del equipo de desarrollo no proporciona conectividad física al panel. El servicio debe mantenerse disponible sobre la interfaz de red adecuada del host para permitir que el dispositivo consulte su estado y transmita órdenes.

El acceso a la fuente maestra se realiza mediante HTTPS desde la interfaz web hacia los servicios de Google. La conexión entre el panel y el backend utiliza HTTP en la versión actual dentro de la infraestructura local. La diferencia entre ambos tramos debe conservarse en la descripción técnica, ya que representa una condición de seguridad distinta para cada segmento de comunicación.

### 1.6 Condiciones de software del sistema completo

El entorno de desarrollo se configura sobre Ubuntu mediante WSL2. Para el firmware se requiere un entorno compatible con PlatformIO, Arduino y las dependencias gráficas definidas para el panel. Para el backend se requiere Node.js, npm y el conjunto de dependencias declarado por el proyecto, incluyendo TypeScript, Express, Vite, React, Google GenAI y el mecanismo de autenticación OAuth utilizado por la interfaz.

La configuración local del firmware debe definir los parámetros de red, la dirección del backend, el identificador del dispositivo y el token de autenticación correspondiente. La configuración local del backend debe contener los parámetros necesarios para el servicio, la integración con Google y la funcionalidad de inteligencia artificial. Los valores sensibles deben permanecer fuera del control de versiones y utilizar archivos locales o variables de entorno.

La ejecución del sistema completo requiere que firmware y backend estén configurados de manera compatible. Una discrepancia en la dirección del servicio, el token, la configuración de Google o los parámetros de la hoja impide que el flujo alcance la etapa de actualización. La condición inicial, por tanto, no se limita a disponer de los dos repositorios, sino que exige coherencia entre sus parámetros de comunicación y operación.

### 1.7 Condiciones de red e infraestructura

La infraestructura mínima requiere una red Wi-Fi con capacidad para establecer comunicación LAN entre el panel y el equipo que ejecuta el backend. Cuando el desarrollo se realiza mediante WSL2, la modalidad de red y las reglas de exposición del sistema anfitrión deben permitir que el dispositivo físico alcance el servicio. La dirección efectiva utilizada por el firmware debe corresponder a una interfaz accesible desde el panel.

El equipo de desarrollo debe disponer de conectividad hacia los servicios externos requeridos por Google para la autenticación, la generación de contenido estructurado y el acceso a la fuente maestra. También se requiere el entorno físico de conexión del panel y los medios necesarios para su alimentación y programación.

La disponibilidad de la infraestructura debe verificarse antes de considerar operativo el flujo. El estado disponible del backend, la correcta autenticación del dispositivo, la autenticación de la cuenta de Google y la accesibilidad de la hoja constituyen comprobaciones independientes. El fallo de cualquiera de estas condiciones interrumpe el procesamiento de una orden antes de su aplicación.

### 1.8 Condiciones de seguridad

Las credenciales de red, los tokens de autenticación, las claves de Google GenAI y los parámetros sensibles de acceso deben mantenerse fuera del control de versiones. La configuración local de cada subsistema se utiliza como mecanismo de separación entre el código reproducible y los valores propios del entorno de ejecución. Esta separación es obligatoria para evitar que información sensible quede incorporada en el repositorio.

La comunicación entre firmware y backend se ejecuta mediante HTTP local en la versión vigente. Esta condición constituye una limitación explícita de seguridad y deja abierta como trabajo futuro la adopción de mecanismos de protección de transporte adecuados para entornos fuera de la red controlada. El acceso a Google se mantiene mediante HTTPS y autenticación OAuth, por lo que los dos segmentos poseen mecanismos de protección diferentes.

La confirmación humana se mantiene como barrera previa a la persistencia. La recepción, interpretación o validación de una orden no implica autorización automática de escritura. Esta condición constituye un control funcional y de seguridad del sistema, ya que separa la propuesta generada por inteligencia artificial de la modificación efectiva de la información maestra.

### 1.9 Flujo de datos del sistema completo como condición inicial

El flujo mínimo comienza con una interacción del usuario en el panel ESP32-S3-4848S040. El firmware transforma la interacción en una orden y la transmite al backend mediante la red local. El backend valida la autenticación del dispositivo, registra la orden y conserva su identificador para mantener el control de duplicados.

La interfaz web recibe la orden pendiente y procede con la lectura de la estructura real de la hoja. Se consultan encabezados y catálogos, se interpreta el lenguaje natural mediante Google GenAI y posteriormente se aplican reglas deterministas. La tarea objetivo se localiza de forma controlada y se genera una propuesta temporal con los campos que podrían modificarse.

La propuesta se presenta a la persona responsable mediante la interfaz de revisión humana. Mientras la propuesta permanece en revisión se conserva el control temporal de la fila correspondiente. Cuando la persona confirma, la interfaz autenticada ejecuta la escritura autorizada sobre Google Sheets. Cuando la persona rechaza, los cambios no se escriben.

Una vez concluido el proceso, el resultado se comunica al backend y, cuando la orden fue originada en el panel, el estado se reporta nuevamente al dispositivo. El panel presenta el resultado mediante sus estados de operación. De esta manera, el flujo completo mantiene separados la captura, el procesamiento de inteligencia artificial, la validación determinista, la aprobación humana y la persistencia.

### 1.10 Componentes no establecidos como condición inicial

La inspección de la implementación vigente no evidenció una arquitectura de persistencia basada en Google Cloud Storage para el flujo 3C descrito. Tampoco se evidenció un servicio Python basado en FastAPI o Flask dentro del backend actualmente integrado. Por tanto, esos componentes no se incorporan como dependencias ni como funcionalidades implementadas de la versión V6.

La documentación tampoco atribuye al firmware una función de escritura directa sobre Google Sheets, debido a que la persistencia efectiva se realiza en la interfaz autenticada. Esta diferenciación evita confundir el canal de transporte del dispositivo con el mecanismo de actualización de la fuente maestra y preserva la separación funcional entre el sistema embebido y la aplicación web.

### 1.11 Resumen de condiciones iniciales mínimas obligatorias

El sistema completo T-030 requiere como mínimo la operación conjunta del firmware ESP32-S3-4848S040 y del backend Asistente 3C. El primer subsistema proporciona la interacción física, la representación visual, la edición de comandos, la conectividad y el seguimiento del estado. El segundo proporciona la recepción de órdenes, la autenticación del dispositivo, la interpretación mediante Google GenAI, la validación determinista, la gestión de propuestas y el control de la revisión humana.

La interfaz web autenticada constituye el componente que coordina la consulta y la actualización de Google Sheets dentro del flujo vigente. La disponibilidad de red, la autenticación, la configuración de Google, la estructura esperada de la hoja y la coherencia de los parámetros entre subsistemas son condiciones necesarias para completar una operación. La ausencia de cualquiera de los elementos esenciales impide considerar operativo el flujo completo.

## Criterio de redacción

La documentación principal del repositorio se mantiene en tercera persona y mediante párrafos técnicos. Los detalles de implementación de bajo nivel, código fuente, comandos de instalación, asignaciones de pines, direcciones concretas, procedimientos de diagnóstico y configuraciones sensibles se reservan para los anexos y documentos técnicos correspondientes.

Fuentes verificadas en la versión V6: configuración vigente del firmware, implementación actual del panel, configuración del proyecto Asistente 3C, API de dispositivo, control de propuestas y revisión, interfaz web y archivos de exclusión de credenciales. La redacción se ajusta a la implementación observada y evita incorporar componentes no sustentados.

Versión V6 Benchmark - Sistema completo obligatorio - Firmware + Backend - Sin componentes no evidenciados
