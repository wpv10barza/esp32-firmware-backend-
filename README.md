T-030 System ESP32-S3-4848S040 plus Backend Asistente 3C

# V15 Integrated Documentation — V14.1 + V15 (V6 Benchmark retained)

This README is the single consolidated documentation record for the T-030 system. It **retains the V6 Benchmark baseline** and **integrates the V14.1 design/evidence expansion**. Neither version is treated as a replacement for the other.

The governing documentation sequence is:

**WEB FIRST then REPOSITORY AFTER**

## Índice de secciones

- **3.1** Initial conditions and documentation scope
- **3.2** Design of the system
  - **3.2.1** Electronic design
  - **3.2.2** Diseño de software
  - **3.2.3** Diseño del agente de IA: Modelo de inteligencia artificial y procesamiento controlado
  - **3.2.4** Arquitectura de compilación y enlazado
  - **3.2.5** Tactile interface design
  - **3.2.6** Tactile treatment
  - **3.2.7** Integrated evidence status
- **3.3** Implementation
- **3.4** Validation
- **3.5** Commissioning
- **3.6** Final integrated documentation control

The integration rule is: preserve documented technical content, remove only direct duplication or obsolete version labels, and resolve differences by explicitly contrasting the two records. No technical statement is promoted from “documented” to “physically tested” unless execution evidence exists.

## Version integration and traceability

This preface is intentionally unnumbered. It records the relationship between V6, V14.1 and V15; the technical hierarchy of the consolidated record begins at Section 3.1.

This README is the **single documentation record** obtained by integrating the V14.1 and V15 documentation versions. The V14.1 content is preserved as the design/evidence layer; V15 is the consolidation and control layer. The earlier V6 Benchmark baseline remains retained because it contains the complete initial-conditions record used by both versions.

**Neither V14.1 nor V15 is replaced or discarded. The two records are joined, contrasted, and reconciled in one document.**

### V14.1 preserved layer

V14.1 establishes the controlled design record reached before the V15 consolidation. Its documented scope includes:

1. Initial conditions and documentation boundaries for the ESP32-S3-4848S040 panel and Asistente 3C backend.
2. **3.2 Design of the system** as the architectural layer separating interaction, transport, interpretation, deterministic validation, human review, persistence, and result reporting.
3. **3.2.1 Electronic design**, including the 480 × 480 RGB/ST7701 display architecture, GT911 touch interface, `panel_4848s040` target, repository evidence markers, and external WEB references.
4. **3.2.2 Software design**, including Node.js, Express, React, GenAI, and the repository implementation markers.
5. Reserved sections for tactile design, tactile treatment, implementation, validation, and commissioning where execution evidence had not yet been consolidated.
6. The explicit control boundary:

**No modification or reflashing of firmware**

### V15 consolidation layer

V15 adds the integration controls required to preserve the V14.1 record while joining it with the broader V6 baseline. The V15 layer therefore:

1. Retains the V6 Benchmark initial conditions instead of deleting them.
2. Keeps the V14.1 design sections in the same README.
3. Contrasts the two versions when wording, scope, or evidence level differs.
4. Distinguishes external WEB evidence, repository evidence, and execution evidence.
5. Preserves the controlled sequence:

**WEB FIRST → VERIFY → REPOSITORY AFTER → BYTE-FOR-BYTE VALIDATION**

6. Prohibits interpreting a documentation change as authorization to modify or reflash firmware.

### Cross-version contrast and reconciliation

| Documentation aspect | V14.1 record | V15 record | Unified treatment |
|---|---|---|---|
| Initial conditions | Focused on the current controlled documentation stage and the two coordinated subsystems. | Retains the broader V6 Benchmark baseline as the historical technical foundation. | The full V6 baseline remains intact; V14.1 is layered over it as the structured design record. |
| System architecture | Defines the separation of interaction, interpretation, deterministic validation, review, and persistence. | Consolidates those responsibilities with the existing V6 operational description. | The architectural separation is retained without assigning write authority to the AI or the embedded panel. |
| Electronic design | Documents ESP32-S3-4848S040, RGB/ST7701, GT911, `panel_4848s040`, and implementation markers. | Adds traceability and evidence-level control around those statements. | V14.1 hardware/design content remains the authoritative documentation layer; V15 adds provenance and control. |
| Software design | Documents Node.js, Express, React, GenAI and the required source-reference markers. | Reconciles these references with the V6 backend/data-flow description. | Both are retained; duplicated wording is consolidated while distinct implementation boundaries remain explicit. |
| Validation | States that later validation must use actual evidence. | Explicitly separates WEB, repository, and execution evidence. | No runtime result is inferred from documentation or source inspection alone. |
| Change boundary | No firmware modification or reflashing. | Makes the same rule a final consolidation control. | The restriction is preserved unchanged and repeated only where it provides traceability. |
| Documentation purpose | Controlled V14.1 design/evidence record. | Single consolidated V15 repository record. | One README now contains both purposes without treating either version as deleted. |

### Version-control rule

When the two versions contain equivalent statements, the unified README keeps one technically consistent statement and preserves the version relationship through this section. When a V14.1 statement adds detail that is not present in V15, that detail is retained in the relevant V14.1 design section. When V15 adds broader baseline or control information, that information is retained in the V6/V15 sections below.

Where a statement would imply a physical test, flashing event, deployment, or runtime result, the unified document keeps the more conservative evidence wording unless explicit execution evidence exists.

## 3.1 Initial conditions and documentation scope

### 3.1.1 Condiciones iniciales del sistema de desarrollo

El sistema de desarrollo comprende dos subsistemas obligatorios e inseparables para la operación del sistema T-030: el firmware del panel ESP32-S3-4848S040 y el backend Asistente 3C. Ambos subsistemas cumplen funciones complementarias. El firmware concentra la interacción física con el usuario, la representación visual, la captura de eventos táctiles y la comunicación con el servicio. El backend concentra la recepción de órdenes, su interpretación mediante inteligencia artificial, la validación determinista, el control de la revisión humana y la coordinación del acceso a la fuente maestra. La separación de responsabilidades permite mantener un control explícito sobre las operaciones que pueden modificar la información de mantenimiento.

### 3.1.2 Plataforma electrónica - Subsistema firmware

El sistema se desarrolló sobre un panel ESP32-S3-4848S040 configurado como objetivo específico de compilación dentro del entorno Arduino para microcontroladores Espressif. La configuración vigente establece una memoria Flash de 16 MB y el uso de PSRAM OPI. La interfaz de usuario trabaja con una resolución lógica de 480 × 480 píxeles y emplea una pantalla RGB asociada a un controlador ST7701S y un sistema táctil capacitivo basado en GT911. La representación gráfica se implementó mediante Arduino-GFX y la conectividad del dispositivo se estableció mediante Wi-Fi.

El firmware incorpora una máquina de estados para representar las condiciones de operación del panel, diferenciando situaciones de inicio, indisponibilidad, disponibilidad, procesamiento, espera de confirmación, aplicación, rechazo y error. Esta separación permite que el estado de la comunicación y del procesamiento sea visible para el usuario sin confundir una orden recibida con una orden aplicada. La interfaz también dispone de un mecanismo de edición de comandos con cursor, inserción de caracteres, eliminación, desplazamiento y teclado virtual adaptado a la geometría de la pantalla.

La gestión de órdenes utiliza un búfer de comandos con capacidad definida y mantiene una posición de cursor independiente del contenido almacenado. El firmware genera solicitudes asociadas a un identificador de dispositivo y a un identificador de petición. Este diseño permite que el backend reconozca reenvíos de una misma petición y evite tratar una retransmisión como una nueva operación lógica. La comunicación se ejecuta mediante solicitudes HTTP hacia el servicio 3C, incluyendo mecanismos de consulta de disponibilidad, envío de comandos, consulta de órdenes pendientes y consulta de resultados.

El dispositivo no ejecuta escritura directa sobre la fuente maestra. La función del firmware consiste en capturar la interacción, estructurar la orden, transmitirla al backend y presentar al usuario el estado de su procesamiento. De esta forma, la modificación de la información queda desacoplada del equipo embebido y se conserva una barrera lógica entre la interfaz física y la persistencia de datos.

### 3.1.3 Plataforma backend Asistente 3C - Subsistema obligatorio

El subsistema backend se desarrolló como una aplicación web basada en Node.js, TypeScript y Express, complementada con una interfaz React ejecutada mediante Vite. La configuración vigente incorpora la biblioteca oficial de Google GenAI para realizar la interpretación estructurada de comandos. La aplicación utiliza variables de entorno para parámetros sensibles y de operación, evitando incorporar valores privados dentro del código versionado.

La arquitectura del backend se organiza alrededor de una API de dispositivo y un mecanismo de almacenamiento temporal de órdenes. Las solicitudes provenientes del panel son normalizadas y verificadas antes de ingresar a la cola. Cada orden conserva un identificador único, un identificador de petición, la identificación del dispositivo, el texto recibido y un estado de procesamiento. Los estados principales distinguen una orden pendiente de confirmación humana, una orden aplicada y una orden rechazada.

El control de acceso del dispositivo se realiza mediante un token configurado en el entorno del backend. Cuando el token requerido no está disponible y no se habilita explícitamente un modo inseguro de desarrollo, la recepción de órdenes se bloquea. Esta condición evita que una instancia del servicio quede disponible para aceptar comandos del dispositivo sin un mecanismo mínimo de autenticación.

La revisión humana se implementa mediante un mecanismo de propuestas con estados de propuesta, aprobación y rechazo. Cada propuesta identifica la fila que será revisada, los campos que se pretende modificar y el identificador de la orden que la originó cuando corresponde. Durante la revisión se mantiene un bloqueo temporal de la fila para impedir que otra propuesta concurrente modifique simultáneamente la misma posición lógica. El bloqueo expira después de un período definido, lo que evita mantener indefinidamente un recurso reservado.

### 3.1.4 Interpretación, validación y control humano

La interpretación de las órdenes se ejecuta mediante Google GenAI con una salida estructurada orientada a operaciones deterministas. El modelo no recibe autorización para modificar directamente la fuente maestra. Su función se limita a transformar el lenguaje natural en una estructura que contiene la tarea buscada, las operaciones solicitadas y una indicación de si la orden requiere revisión.

Después de la interpretación se ejecuta una etapa de validación determinista. En ella se verifican los campos permitidos, la correspondencia de encabezados, la validez de tipos de datos, las unidades de tiempo y la pertenencia de los valores de catálogo a los registros existentes. Las reglas restringen explícitamente las columnas modificables y bloquean operaciones que no correspondan a la estructura esperada de la estrategia. Cuando una condición no puede comprobarse, el procesamiento se detiene y la solicitud se dirige a revisión.

La interfaz humana presenta una vista previa antes de efectuar cualquier escritura. La aplicación identifica de manera controlada la fila objetivo y muestra los cambios propuestos junto con sus valores resultantes. La escritura únicamente se habilita después de una acción explícita de confirmación. La acción de rechazo cierra la propuesta sin aplicar cambios y permite comunicar al panel que la orden no fue ejecutada.

### 3.1.5 Fuente maestra y acceso a Google Sheets

La fuente maestra se encuentra implementada en Google Sheets. El acceso desde la interfaz web se realiza mediante autenticación OAuth de Google y solicitudes HTTPS contra la API de Google Sheets. La aplicación obtiene la configuración de la hoja, verifica la estructura esperada, consulta los encabezados reales, identifica los catálogos necesarios y localiza la tarea antes de construir una propuesta de modificación.

La aplicación cliente ejecuta directamente las operaciones de lectura y escritura sobre la API de Google Sheets utilizando el token de acceso obtenido con OAuth. El backend participa en la interpretación, la validación, el almacenamiento temporal de órdenes y el control de revisión, mientras que la interfaz autenticada realiza la persistencia efectiva una vez que la persona responsable confirma la vista previa. Esta distribución debe mantenerse diferenciada para evitar atribuir al backend una función de escritura que actualmente se ejecuta en la interfaz.

La versión actual no presenta dependencias del servicio Google Cloud Storage, de buckets ni de autenticación mediante una cuenta de servicio para la persistencia descrita. Tampoco se evidenció un backend basado en FastAPI o Flask. Por tanto, dichos componentes no se consideran condiciones iniciales de esta versión y no deben incorporarse a la descripción de las funcionalidades implementadas.

### 3.1.6 Conexiones principales del sistema completo

La arquitectura electrónica considera la alimentación del panel, la interfaz de visualización RGB, el sistema táctil y los elementos auxiliares definidos para la variante física empleada. El detalle de las asignaciones eléctricas se reserva para el Anexo A. La interfaz lógica mantiene una resolución de 480 × 480 píxeles para la interacción y la presentación de estados.

La arquitectura lógica requiere conectividad de red entre el panel y el equipo que expone el backend. El dispositivo debe utilizar una dirección accesible dentro de la red local; una dirección de bucle local exclusiva del equipo de desarrollo no proporciona conectividad física al panel. El servicio debe mantenerse disponible sobre la interfaz de red adecuada del host para permitir que el dispositivo consulte su estado y transmita órdenes.

El acceso a la fuente maestra se realiza mediante HTTPS desde la interfaz web hacia los servicios de Google. La conexión entre el panel y el backend utiliza HTTP en la versión actual dentro de la infraestructura local. La diferencia entre ambos tramos debe conservarse en la descripción técnica, ya que representa una condición de seguridad distinta para cada segmento de comunicación.

### 3.1.7 Condiciones de software del sistema completo

El entorno de desarrollo se configura sobre Ubuntu mediante WSL2. Para el firmware se requiere un entorno compatible con PlatformIO, Arduino y las dependencias gráficas definidas para el panel. Para el backend se requiere Node.js, npm y el conjunto de dependencias declarado por el proyecto, incluyendo TypeScript, Express, Vite, React, Google GenAI y el mecanismo de autenticación OAuth utilizado por la interfaz.

La configuración local del firmware debe definir los parámetros de red, la dirección del backend, el identificador del dispositivo y el token de autenticación correspondiente. La configuración local del backend debe contener los parámetros necesarios para el servicio, la integración con Google y la funcionalidad de inteligencia artificial. Los valores sensibles deben permanecer fuera del control de versiones y utilizar archivos locales o variables de entorno.

La ejecución del sistema completo requiere que firmware y backend estén configurados de manera compatible. Una discrepancia en la dirección del servicio, el token, la configuración de Google o los parámetros de la hoja impide que el flujo alcance la etapa de actualización. La condición inicial, por tanto, no se limita a disponer de los dos repositorios, sino que exige coherencia entre sus parámetros de comunicación y operación.


### 3.1.7.1 Preparación reproducible Windows → WSL2 → PlatformIO

Para programar físicamente el ESP32-S3-4848S040 desde una laptop Windows con WSL2, la conexión USB y el entorno Python se mantienen como dos pasos separados.

**Paso 1 — PowerShell como Administrador**

Ejecutar:

`powershell -ExecutionPolicy Bypass -File .\scripts\connect-esp32-wsl.ps1`

El script muestra `usbipd list`, solicita el `BUSID` del ESP32, ejecuta `usbipd bind --busid <BUSID>` y después `usbipd attach --wsl --busid <BUSID>`. No contiene credenciales del firmware ni del backend.

**Paso 2 — WSL**

Dentro de WSL:

`cd "$HOME/projects/4848-production"`

`bash ./scripts/setup-wsl-env.sh`

El script crea o reutiliza `.venv`, instala `platformio==6.2.0`, verifica Python/PlatformIO y muestra los dispositivos serie visibles en WSL. La configuración privada de `include/local_config.h` permanece separada y no se crea automáticamente.

**Paso 3 — verificar USB**

`ls -l /dev/ttyACM* /dev/ttyUSB* 2>/dev/null`

`pio device list`

**Paso 4 — compilar y cargar**

`source .venv/bin/activate`

`./scripts/flash-panel.sh --monitor`

El script de flash usa exclusivamente el entorno `panel_4848s040` y el puerto serie seleccionado. GitHub Actions valida la sintaxis y el contrato de estos scripts, pero la conexión USB y la carga física siguen siendo una operación local de la laptop.

### 3.1.8 Condiciones de red e infraestructura

La infraestructura mínima requiere una red Wi-Fi con capacidad para establecer comunicación LAN entre el panel y el equipo que ejecuta el backend. Cuando el desarrollo se realiza mediante WSL2, la modalidad de red y las reglas de exposición del sistema anfitrión deben permitir que el dispositivo físico alcance el servicio. La dirección efectiva utilizada por el firmware debe corresponder a una interfaz accesible desde el panel.

El equipo de desarrollo debe disponer de conectividad hacia los servicios externos requeridos por Google para la autenticación, la generación de contenido estructurado y el acceso a la fuente maestra. También se requiere el entorno físico de conexión del panel y los medios necesarios para su alimentación y programación.

La disponibilidad de la infraestructura debe verificarse antes de considerar operativo el flujo. El estado disponible del backend, la correcta autenticación del dispositivo, la autenticación de la cuenta de Google y la accesibilidad de la hoja constituyen comprobaciones independientes. El fallo de cualquiera de estas condiciones interrumpe el procesamiento de una orden antes de su aplicación.

### 3.1.9 Condiciones de seguridad

Las credenciales de red, los tokens de autenticación, las claves de Google GenAI y los parámetros sensibles de acceso deben mantenerse fuera del control de versiones. La configuración local de cada subsistema se utiliza como mecanismo de separación entre el código reproducible y los valores propios del entorno de ejecución. Esta separación es obligatoria para evitar que información sensible quede incorporada en el repositorio.

La comunicación entre firmware y backend se ejecuta mediante HTTP local en la versión vigente. Esta condición constituye una limitación explícita de seguridad y deja abierta como trabajo futuro la adopción de mecanismos de protección de transporte adecuados para entornos fuera de la red controlada. El acceso a Google se mantiene mediante HTTPS y autenticación OAuth, por lo que los dos segmentos poseen mecanismos de protección diferentes.

La confirmación humana se mantiene como barrera previa a la persistencia. La recepción, interpretación o validación de una orden no implica autorización automática de escritura. Esta condición constituye un control funcional y de seguridad del sistema, ya que separa la propuesta generada por inteligencia artificial de la modificación efectiva de la información maestra.

### 3.1.10 Flujo de datos del sistema completo como condición inicial

El flujo mínimo comienza con una interacción del usuario en el panel ESP32-S3-4848S040. El firmware transforma la interacción en una orden y la transmite al backend mediante la red local. El backend valida la autenticación del dispositivo, registra la orden y conserva su identificador para mantener el control de duplicados.

La interfaz web recibe la orden pendiente y procede con la lectura de la estructura real de la hoja. Se consultan encabezados y catálogos, se interpreta el lenguaje natural mediante Google GenAI y posteriormente se aplican reglas deterministas. La tarea objetivo se localiza de forma controlada y se genera una propuesta temporal con los campos que podrían modificarse.

La propuesta se presenta a la persona responsable mediante la interfaz de revisión humana. Mientras la propuesta permanece en revisión se conserva el control temporal de la fila correspondiente. Cuando la persona confirma, la interfaz autenticada ejecuta la escritura autorizada sobre Google Sheets. Cuando la persona rechaza, los cambios no se escriben.

Una vez concluido el proceso, el resultado se comunica al backend y, cuando la orden fue originada en el panel, el estado se reporta nuevamente al dispositivo. El panel presenta el resultado mediante sus estados de operación. De esta manera, el flujo completo mantiene separados la captura, el procesamiento de inteligencia artificial, la validación determinista, la aprobación humana y la persistencia.

### 3.1.11 Componentes no establecidos como condición inicial

La inspección de la implementación vigente no evidenció una arquitectura de persistencia basada en Google Cloud Storage para el flujo 3C descrito. Tampoco se evidenció un servicio Python basado en FastAPI o Flask dentro del backend actualmente integrado. Por tanto, esos componentes no se incorporan como dependencias ni como funcionalidades implementadas de la versión V6.

La documentación tampoco atribuye al firmware una función de escritura directa sobre Google Sheets, debido a que la persistencia efectiva se realiza en la interfaz autenticada. Esta diferenciación evita confundir el canal de transporte del dispositivo con el mecanismo de actualización de la fuente maestra y preserva la separación funcional entre el sistema embebido y la aplicación web.

### 3.1.12 Resumen de condiciones iniciales mínimas obligatorias

El sistema completo T-030 requiere como mínimo la operación conjunta del firmware ESP32-S3-4848S040 y del backend Asistente 3C. El primer subsistema proporciona la interacción física, la representación visual, la edición de comandos, la conectividad y el seguimiento del estado. El segundo proporciona la recepción de órdenes, la autenticación del dispositivo, la interpretación mediante Google GenAI, la validación determinista, la gestión de propuestas y el control de la revisión humana.

La interfaz web autenticada constituye el componente que coordina la consulta y la actualización de Google Sheets dentro del flujo vigente. La disponibilidad de red, la autenticación, la configuración de Google, la estructura esperada de la hoja y la coherencia de los parámetros entre subsistemas son condiciones necesarias para completar una operación. La ausencia de cualquiera de los elementos esenciales impide considerar operativo el flujo completo.

### 3.1.13 Criterio editorial y de control V6

The V6 Benchmark baseline is retained as a documented technical source within this consolidated Chapter III record. Its statements are preserved as baseline documentation and are not reinterpreted as execution results.

### 3.1.14 Criterio de redacción y fuentes de la línea base

La documentación principal del repositorio se mantiene en tercera persona y mediante párrafos técnicos. Los detalles de implementación de bajo nivel, código fuente, comandos de instalación, asignaciones de pines, direcciones concretas, procedimientos de diagnóstico y configuraciones sensibles se reservan para los anexos y documentos técnicos correspondientes.

Fuentes verificadas en la versión V6: configuración vigente del firmware, implementación actual del panel, configuración del proyecto Asistente 3C, API de dispositivo, control de propuestas y revisión, interfaz web y archivos de exclusión de credenciales. La redacción se ajusta a la implementación observada y evita incorporar componentes no sustentados.

Versión V6 Benchmark - Sistema completo obligatorio - Firmware + Backend - Sin componentes no evidenciados

**V14.1 Design expansion.** The following Section 3.2 preserves the V14.1 design/evidence layer inside the consolidated Chapter III hierarchy.

## 3.2 Design of the system

The system architecture is divided into an embedded interaction layer and an application/backend coordination layer. The ESP32-S3-4848S040 panel provides the local user interface, display interaction, tactile input, network communication, and presentation of command status. The Asistente 3C layer coordinates command reception, interpretation, validation, review control, and the controlled interaction with the maintenance information source.

The architectural principle is separation of concerns. Probabilistic interpretation is separated from deterministic validation and from persistence authority. A command received from the panel is therefore treated as an input to a controlled process rather than as an unrestricted write instruction.

The repository documentation distinguishes the following logical stages: user interaction, command transport, request interpretation, deterministic checking, review or confirmation, persistence, and result reporting. This separation is maintained in the documentation even when individual implementation components evolve.

### 3.2.1 Electronic design

The electronic documentation concerns the ESP32-S3-4848S040 panel and its display/touch subsystem. The controlled hardware description identifies a 480 × 480 RGB display architecture, an ST7701-class display controller, and a GT911 touch controller. The repository configuration also documents a `panel_4848s040` PlatformIO target using an ESP32-S3 board definition, 16 MB flash configuration, OPI PSRAM configuration, and the GFX Library for Arduino dependency.

#### Espressif WEB evidence

Espressif documentation describes RGB LCD operation on ESP32-S3 and identifies the RGB panel configuration as dependent on data width, pixel format, and panel timing. Espressif also documents RGB display considerations involving PSRAM bandwidth and frame-buffer requirements on ESP32-S3 systems. These external references are used as the technical background for the display architecture and are not interpreted as evidence of a physical hardware test on this repository.

The ST7701S documentation from Espressif's display material describes RGB interface configuration, including DE and SYNC modes and supported color formats. The exact electrical configuration remains a board-level property and must be kept consistent with the verified hardware configuration.

#### ST7701 evidence

The controlled implementation evidence reference is:

`st7701_type8_init_operations`

This identifier is retained as the implementation reference required by the V14.1 documentation record. The reference does not by itself constitute a claim that the physical panel has been reinitialized or reflashed during this consolidation.

#### GT911 evidence

Espressif's current board-manager documentation describes GT911 as an I2C touch controller and identifies an `esp_lcd_touch_gt911` component for GT911-based touch configurations. The V14.1 record therefore treats GT911 as an I2C touch-interface component whose exact address, coordinates, and board timing must follow the verified panel configuration rather than being inferred from a generic board.

#### Guition evidence

Guition-related panel implementation material is treated as supporting reference evidence for the ESP32-S3-4848S040 display architecture. It is not used to transfer undocumented GPIO assignments or to authorize firmware changes in this consolidation.

#### Repository verification

The repository-side hardware configuration currently includes the `panel_4848s040` PlatformIO environment. The recorded configuration identifies ESP32-S3, Arduino framework, 16 MB flash, OPI PSRAM, and the Arduino GFX dependency. These values provide repository-level evidence for the documented build target.

The following implementation reference markers are preserved in the controlled documentation record because they are part of the requested evidence index:

- `st7701_type8_init_operations`
- `server/deviceApi.ts`
- `server/deviceCommands.ts`
- `server/reviewControl.ts`

These markers are documentation references. Their presence in the README does not authorize modification of the referenced source files.

### 3.2.2 Diseño de software

En la documentación de la arquitectura de software se describe un sistema coordinado de comunicación web y de dispositivos. En la capa de aplicación se utilizan componentes de servidor orientados a Node.js, un patrón de API HTTP compatible con Express, una interfaz basada en React y una interpretación de comandos asistida por inteligencia artificial generativa. Por su parte, el panel embebido se comunica con el servicio a través de un contrato de dispositivo controlado, en lugar de escribir directamente en una fuente de datos maestra.

**A. Entorno Node.js**

En el diseño de software se registra a Node.js como la familia de servidor y entorno de ejecución empleada por la capa de aplicación del Asistente 3C. El propósito del servidor es recibir y coordinar las solicitudes estructuradas de los dispositivos, preservar la identidad de la solicitud y respaldar el ciclo de vida controlado de los comandos.

**B. Capa de servicio Express**

Se documenta a Express como la capa de servicio HTTP utilizada para exponer el contrato de cara al dispositivo. En esta arquitectura se separa el transporte de la decisión de aplicar un cambio. En consecuencia, una solicitud HTTP aceptada representa la recepción o progresión a través del flujo de trabajo, y no una autorización automática para modificar o escribir sobre los datos maestros.

**C. Interfaz React**

Se documenta a React como la capa de interfaz de usuario para el flujo de trabajo de revisión controlada. Mediante esta interfaz se puede presentar una operación propuesta, exponer los campos o el estado resultante, y mantener un paso de confirmación humana entre la interpretación y la persistencia de datos.

**D. Inteligencia Artificial Generativa (GenAI)**

La inteligencia artificial generativa se documenta como un componente de interpretación. Su función consiste en convertir instrucciones en lenguaje natural en una representación estructurada adecuada para comprobaciones deterministas. Asimismo, se tiene en cuenta que el componente de IA no opera como la autoridad directa para la persistencia de la información.

**E. Verificación del repositorio**

Los marcadores de evidencia de software requeridos para el registro V14.1 se conservan explícitamente de la siguiente manera:

* `server/deviceApi.ts`
* `server/deviceCommands.ts`
* `server/reviewControl.ts`

De este modo, se retiene a `server/deviceApi.ts` como la referencia de evidencia de la API del dispositivo; a `server/deviceCommands.ts` como la referencia de estado y normalización de comandos; y a `server/reviewControl.ts` como la referencia de control de revisión humana. En el documento principal (*README*) se registran estos archivos como marcadores de evidencia de implementación, por lo cual no se modifica su contenido como parte de esta consolidación de la documentación.

### 3.2.3 Diseño del agente de IA: Modelo de inteligencia artificial y procesamiento controlado

El agente de inteligencia artificial constituye la capa de interpretación semántica del sistema Asistente 3C. Su función principal consiste en transformar una instrucción expresada en lenguaje natural en una representación estructurada de la tarea y de las operaciones solicitadas, la cual posteriormente se somete a validaciones deterministas y al flujo de revisión humana. En consecuencia, la generación por parte del modelo no se considera una autorización autónoma para modificar la fuente maestra.

Para la implementación documentada se utiliza la biblioteca `@google/genai`. El modelo configurado por defecto corresponde a `gemini-2.5-flash`, aunque su selección puede sustituirse mediante la variable de entorno `GEMINI_MODEL`. Mediante esta configuración se mantiene separado el comportamiento del software respecto del identificador concreto del modelo utilizado durante una ejecución determinada.

#### 3.2.3.1 Función del modelo

La llamada al modelo se realiza mediante la función `ai.models.generateContent`. Se tiene en cuenta que la configuración establece `temperature: 0`, lo que orienta la generación hacia un comportamiento controlado y reduce la variabilidad en la interpretación de comandos equivalentes.

Asimismo, la salida se solicita mediante `responseMimeType: "application/json"` y un `responseSchema` definido explícitamente. En dicho esquema se establecen los campos para la tarea buscada, el identificador de tarea cuando corresponda, las operaciones propuestas y la indicación de si se requiere revisión. Por consiguiente, el resultado del modelo se procesa como una estructura verificable y no como texto libre destinado a ejecutar cambios.

La función del modelo se limita a la interpretación semántica. En el flujo lógico posterior se conserva la separación entre el modelo, la salida estructurada, la validación determinista, la localización de la tarea, la propuesta, la revisión humana y la persistencia autorizada.

#### 3.2.3.2 Entrada contextual y grounding con información real

En la etapa de interpretación, la implementación recibe `detectedHeaders` y `detectedCatalogs` como contexto. Los encabezados permiten contrastar la estructura real de la hoja, mientras que los catálogos proporcionan los valores existentes que pueden utilizarse en los campos categóricos controlados.

De igual manera, la identificación de la tarea se mantiene vinculada a la estructura de la estrategia. Se establece la búsqueda por `Nombre` en la columna F y se permite utilizar `TareaId` en la columna E cuando el usuario lo especifica explícitamente. Esta distinción evita que el modelo invente identificadores o interprete como identidad una columna diferente a la establecida por el contrato.

Los catálogos utilizados como contexto corresponden a `ItemMantenible`, `ModoDeFalla`, `Especialidad` y `Labour1`. La finalidad de este mecanismo es restringir la interpretación exclusivamente a valores que existen en la fuente contextualizada.

Cabe precisar que en esta implementación específica no se evidencia una recuperación vectorial para la etapa de `/api/extract`. El grounding documentado para este componente es de tipo tabular y estructural (encabezados, catálogos y reglas de operación), por lo que no se atribuye una arquitectura RAG vectorial en este flujo.

#### 3.2.3.3 Contrato de salida estructurada

En el `responseSchema` se define una estructura de respuesta que contiene, como mínimo, los campos `tarea_buscada`, `operaciones` y `requiere_revision`, además de `tarea_id` y `motivo_revision` cuando corresponda.

Cada operación identifica un campo permitido, su valor propuesto y, opcionalmente, una razón asociada. Dado que los campos permitidos se encuentran definidos previamente en `FIELD_RULES`, el modelo no determina libremente qué columnas del sistema se pueden modificar.

La lista blanca documentada comprende los campos asociados a las columnas B, C, H, I, J, K, L, M, N y O. Por consiguiente, las columnas de identidad, búsqueda o cualquier columna fuera de la lista autorizada permanecen fuera del dominio de modificación. De este modo, el contrato estructurado establece una frontera entre la generación y la ejecución: la inteligencia artificial propone una estructura y la aplicación determina si dicha estructura es aceptable.

#### 3.2.3.4 Validación determinista posterior al modelo

La respuesta generada se procesa mediante una segunda etapa de validación programática. Antes de aceptar cada operación, se verifica que el campo recibido pertenezca a `FIELD_RULES`.

Posteriormente, se comprueba que la columna asociada coincida con el encabezado esperado. Cuando existe una discrepancia entre la estructura detectada y la definición de una columna, la auditoría se detiene en lugar de continuar con una operación potencialmente incorrecta.

Los valores de catálogo se normalizan para su comparación; sin embargo, el valor finalmente utilizado debe corresponder a un elemento existente del catálogo. Esto evita convertir una variación de mayúsculas, minúsculas o acentuación en un valor nuevo no autorizado.

Asimismo, las frecuencias se convierten en valores enteros mayores o iguales a uno, mientras que las unidades de tiempo se normalizan hacia representaciones canónicas como `Mes`, `Año`, `Semana`, `Día` y `Hora`. Por su parte, los campos de texto largo rechazan expresiones incompletas (como `...`, `…` o `etc.`), debido a que la información destinada a la fuente maestra debe conservar el contenido descriptivo completo.

#### 3.2.3.5 Límites de autoridad del agente de IA

El agente no posee autoridad directa para modificar la fuente maestra. A través del endpoint `/api/extract` se interpreta la instrucción y se devuelve una estructura de operaciones validada, pero no se ejecuta por sí mismo una escritura sobre Google Sheets.

Asimismo, la recepción de una orden y su interpretación no equivalen a su aplicación. El resultado del modelo se incorpora al proceso de propuesta y revisión, manteniendo separadas la interpretación, la validación y la persistencia.

En consecuencia, la autoridad de la inteligencia artificial se limita a interpretar la intención expresada por el usuario dentro del contrato de campos, encabezados, catálogos y reglas proporcionado como contexto.

#### 3.2.3.6 Integración con revisión humana

Cuando la estructura resultante requiere revisión o no contiene operaciones válidas, en el sistema se establece `requiere_revision`. Posteriormente, la propuesta se puede registrar mediante `reviewStore`.

En la propuesta se conserva la fila objetivo, la coincidencia localizada, las operaciones solicitadas y, cuando corresponde, el identificador de la orden externa que originó el proceso. Por tanto, la revisión humana se mantiene como una condición indispensable entre la propuesta generada y la persistencia.

Este diseño impide interpretar una respuesta correcta del modelo como una escritura automática, por lo que la decisión final permanece separada de la generación probabilística y se ejecuta mediante el flujo de aprobación o rechazo.

#### 3.2.3.7 Secuencia completa de procesamiento de una instrucción

El procesamiento de una instrucción se estructura de forma secuencial mediante los siguientes pasos técnicos:

1. Recepción de la instrucción en lenguaje natural.
2. Incorporación de encabezados, estructura y catálogos disponibles como contexto.
3. Envío de la solicitud al modelo Gemini mediante `@google/genai`.
4. Generación de la respuesta en formato JSON con el esquema definido (`responseSchema`).
5. Parseo y extracción de la respuesta estructurada.
6. Verificación determinista del campo y de la columna asociada según `FIELD_RULES`.
7. Validación de catálogos, frecuencias, unidades y contenido textual.
8. Determinación de la tarea objetivo por *Nombre* o `TareaId`.
9. Generación de una propuesta controlada de modificación.
10. Ejecución del flujo de revisión humana.
11. Persistencia de datos tras obtener la autorización correspondiente.

Esta secuencia mantiene separadas las responsabilidades de interpretación semántica y ejecución determinista. Se tiene en cuenta que un error de formato, una discrepancia de encabezado, un valor de catálogo inexistente o una condición no verificable interrumpe el avance normal del proceso, evitando que la salida del modelo se convierta en una modificación directa sobre la fuente de datos.

### 3.2.4 Arquitectura de compilación y enlazado

La construcción del firmware 3C del panel ESP32-S3-4848S040 utiliza el entorno `panel_4848s040` de PlatformIO y se describe conceptualmente en cuatro etapas técnicas: **preprocesamiento, compilación, enlazado y empaquetado**. Este flujo transforma el código fuente y sus dependencias en los artefactos binarios destinados al microcontrolador, manteniendo separadas las responsabilidades de la lógica de interacción, los controladores gráficos, la interfaz táctil, la comunicación y el servicio de dispositivo.

En el repositorio, el punto de entrada del objetivo `panel_4848s040` es `src/main.cpp`. El archivo `platformio.ini` define `panel_4848s040` como entorno de PlatformIO, utiliza la plataforma `espressif32@6.8.1`, la placa `esp32-s3-devkitm-1` y el framework Arduino. El `platformio.ini` vigente no declara explícitamente `src_dir` ni `build_src_filter`; por ello, esta sección no atribuye esas propiedades al archivo. La fuente documentada que interviene en el firmware corresponde a `src/main.cpp`, conforme a la estructura de directorios observada en el repositorio.

#### A. Fase de preprocesamiento

La primera etapa resuelve las directivas del preprocesador, las cabeceras y las macros que determinan la configuración concreta de la compilación. En `src/main.cpp` se incluyen las bibliotecas de plataforma y comunicación, entre ellas `Arduino.h`, `Arduino_GFX_Library.h`, `ESPmDNS.h`, `HTTPClient.h`, `Preferences.h`, `WebServer.h`, `WiFi.h`, `Wire.h` y `driver/i2s.h`, además de las cabeceras locales `app_config.h`, `command_buffer.h`, `command_text_viewport.h` y `virtual_keyboard.h`.

La selección del objetivo del panel queda condicionada por la definición:

```cpp
#if defined(BOARD_PANEL_4848S040)
```

Esta macro se proporciona desde `platformio.ini` mediante:

```
-D BOARD_PANEL_4848S040=1
-D BOARD_HAS_PSRAM=1
-D CORE_DEBUG_LEVEL=3
```

De esta manera, el preprocesador determina qué partes del código pertenecen al objetivo físico `panel_4848s040`. La configuración también establece el tipo de PSRAM OPI y la memoria Flash de 16 MB.

La dependencia gráfica del firmware 3C se incorpora mediante PlatformIO como `moononournation/GFX Library for Arduino@1.5.9`. En consecuencia, `Arduino_GFX_Library.h` corresponde a una dependencia administrada por PlatformIO y no a una copia local duplicada del controlador gráfico.

El repositorio actual no contiene una segunda descripción ESPHome del mismo panel dentro de la estructura versionada. Por tanto, la documentación de compilación no debe presentar dos objetivos simultáneos ni atribuir una doble compilación de ST7701S o GT911. La implementación del firmware utilizada por este entorno se concentra en el objetivo PlatformIO `panel_4848s040`.

#### B. Fase de compilación

Después del preprocesamiento, cada unidad de traducción se transforma conceptualmente en código objeto para la plataforma ESP32-S3. En este proyecto, `src/main.cpp` concentra la lógica principal del panel: inicialización del display, lectura del GT911, representación de estados, editor de comandos, teclado virtual, comunicación HTTP, mDNS, Wi-Fi, polling y servidor local del dispositivo.

La construcción utiliza el entorno:

```
[env:panel_4848s040]
platform = espressif32@6.8.1
board = esp32-s3-devkitm-1
framework = arduino
```

La cadena de herramientas proporcionada por la plataforma Espressif seleccionada por PlatformIO prepara el código para la arquitectura de ejecución del ESP32-S3. La compilación integra el código del proyecto con las bibliotecas de Arduino, Arduino-GFX y las funciones de comunicación y periféricos utilizadas por `src/main.cpp`.

Debe distinguirse entre lo configurado explícitamente por el repositorio y las opciones internas de la cadena de herramientas. En el `platformio.ini` vigente no aparece `build_type = release` ni una bandera de optimización manual como `-Os` u `-O2`. Por ello, la documentación no debe presentar ninguna de esas opciones como una configuración explícita del proyecto.

La arquitectura de dependencias puede resumirse de la siguiente forma:

```
Cabeceras y macros
       │
       ▼
Unidades de traducción .cpp
       │
       ▼
Código objeto para ESP32-S3
       │
       ├── lógica del panel
       ├── Arduino-GFX
       ├── Arduino / Wi-Fi / HTTP
       ├── Wire / GT911
       ├── mDNS
       └── I²S, cuando se habilita
```

Este nivel explica la relación entre las partes del firmware sin introducir listados extensos de objetos ni detalles de ensamblador que no son necesarios para la descripción académica del diseño.

#### C. Fase de enlazado

La tercera etapa reúne el código objeto y las bibliotecas necesarias en una imagen ejecutable para la placa. En términos del modelo clásico de compilación, el enlazador resuelve referencias entre unidades de traducción, bibliotecas y símbolos del framework Arduino y construye el ejecutable ELF del firmware.

En el objetivo `panel_4848s040`, la memoria de Flash se configura explícitamente mediante:

```
board_build.flash_size = 16MB
board_upload.flash_size = 16MB
board_build.partitions = default_16MB.csv
```

Asimismo, el proyecto declara:

```
board_build.psram_type = opi
board_build.arduino.memory_type = qio_opi
```

Estos parámetros describen la plataforma de memoria sobre la que se ejecutará el firmware. Debe distinguirse la función de cada recurso: la Flash de 16 MB y la tabla de particiones determinan la organización de la imagen persistente, mientras que la PSRAM OPI constituye memoria adicional de ejecución disponible para el sistema. La presencia de PSRAM no implica que todos los objetos compilados sean enlazados directamente dentro de ella.

El artefacto ELF esperado por PlatformIO para este entorno corresponde a:

```
.pio/build/panel_4848s040/firmware.elf
```

El ELF conserva la información necesaria para representar el programa enlazado y constituye el artefacto principal de construcción antes del empaquetado binario.

En `src/main.cpp`, el código de inicialización del hardware forma parte de la unidad de traducción que se compila y enlaza para el objetivo `panel_4848s040`. La instancia de `Arduino_RGB_Display` se construye con un panel RGB de 480 × 480, rotación 1 y la secuencia de inicialización `st7701_type9_init_operations`.

Las señales principales del panel y del táctil se declaran de forma explícita en `src/main.cpp`:

```
ST7701S / RGB:
DE       GPIO18
VSYNC    GPIO17
HSYNC    GPIO16
PCLK     GPIO21

RGB data:
R0       GPIO11
R1       GPIO12
R2       GPIO13
R3       GPIO14
R4       GPIO0
G0       GPIO8
G1       GPIO20
G2       GPIO3
G3       GPIO46
G4       GPIO9
G5       GPIO10
B0       GPIO4
B1       GPIO5
B2       GPIO6
B3       GPIO7
B4       GPIO15

SPI de comandos:
CS       GPIO39
CLK      GPIO48
MOSI     GPIO47

GT911:
SDA      GPIO19
SCL      GPIO45
I²C      0x5D

Backlight:
GPIO38
```

En esta arquitectura, GPIO19 y GPIO20 pertenecen a funciones diferentes: GPIO19 actúa como SDA del GT911, mientras que GPIO20 forma parte del bus de datos RGB utilizado por el panel. Esta separación debe conservarse en la documentación para evitar presentar ambos GPIO como parte del mismo subsistema eléctrico.

#### D. Fase de empaquetado de la imagen

Después del enlace, PlatformIO transforma el resultado de la construcción en los artefactos binarios utilizados para carga y distribución del firmware. El flujo operativo documentado utiliza `scripts/flash-panel.sh`, que ejecuta explícitamente:

```bash
pio run -e panel_4848s040
```

Si la compilación finaliza correctamente, el mismo script ejecuta la tarea de carga mediante:

```bash
pio run -e panel_4848s040 -t upload --upload-port "$PORT"
```

Los artefactos estándar asociados al entorno incluyen:

```
firmware.bin
bootloader.bin
partitions.bin
firmware.elf
```

El artefacto `firmware.bin` constituye la representación binaria del programa preparada para el proceso de carga. `bootloader.bin` y `partitions.bin` complementan la imagen de ejecución con los componentes correspondientes al arranque y a la organización de la Flash. La generación de estos artefactos constituye evidencia de construcción; el script de carga no debe interpretarse como una verificación automática del funcionamiento eléctrico del panel.

Por ello, en el contexto del firmware 3C, la secuencia académicamente correcta se expresa como:

```
Cabeceras + macros
        │
        ▼
Fuentes .cpp
        │
        ▼
Compilación para ESP32-S3
        │
        ▼
Objetos + bibliotecas
        │
        ▼
Enlazado
        │
        ▼
firmware.elf
        │
        ▼
Empaquetado PlatformIO
        │
        ├── firmware.bin
        ├── bootloader.bin
        └── partitions.bin
```

#### Integración con el flujo operativo del proyecto

Esta arquitectura de compilación se materializa en el flujo reproducible del repositorio. En particular, `scripts/flash-panel.sh` ejecuta primero `pio run -e panel_4848s040` y solo continúa con la carga si dicha compilación termina correctamente. Por tanto, el comando de construcción no representa una operación monolítica aislada: desencadena la cadena completa que comienza en el preprocesamiento y termina en la generación de los artefactos de firmware.

La relación entre verificación, construcción y dispositivo físico puede expresarse de la siguiente manera:

```
Verify / configuración
        │
        ▼
PlatformIO
        │
        ├── compilación
        ├── enlazado
        └── generación de artefactos
                │
                ▼
          firmware.bin
                │
                ▼
      carga sobre ESP32-S3
                │
                ▼
         monitor serie
```

La secuencia anterior debe interpretarse como la correspondencia entre el ciclo clásico de construcción y el flujo operativo del firmware 3C: las condiciones y verificaciones preceden a la construcción; la compilación y el enlazado producen la imagen; el empaquetado genera los binarios; y la carga sobre el ESP32-S3 pertenece a un nivel posterior de validación física.

La presencia de `firmware.bin`, `firmware.elf`, `bootloader.bin` y `partitions.bin` demuestra generación de artefactos de construcción del entorno `panel_4848s040`. No obstante, la generación exitosa de estos archivos no constituye, por sí sola, evidencia suficiente de funcionamiento eléctrico del ST7701S, respuesta del GT911, comunicación USB, conectividad de red o estabilidad física del panel. Esas propiedades corresponden al nivel de validación física definido por el proyecto.

En consecuencia, la arquitectura de compilación del firmware 3C puede expresarse académicamente como una cadena de transformación:

```
cabeceras y configuración
        →
unidades de traducción
        →
código objeto
        →
enlace con bibliotecas
        →
ELF
        →
binarios de firmware
        →
carga sobre el dispositivo
```

Esta cadena permite explicar cómo la lógica del editor 3C, el teclado virtual, el `commandBuffer`, la comunicación con la Device API y los controladores del panel se convierten en una imagen de firmware para el ESP32-S3 sin confundir las etapas de construcción con las etapas de validación física.

### 3.2.5 Tactile interface design

Pending controlled documentation section. It is reserved for the tactile interaction design of the ESP32-S3-4848S040 panel, including the documented touch-controller interface, interaction regions, priority rules, cursor interaction, keyboard behavior, and touch-hit testing once the corresponding evidence is consolidated.

### 3.2.6 Tactile treatment

Pending controlled documentation section. It is reserved for the documented treatment of tactile events, debouncing or event filtering where evidenced, interaction priority, visual feedback, and state transitions.

### 3.2.7 Integrated evidence status

The integrated documentation distinguishes three evidence levels:

- **External WEB evidence:** technical background checked against the referenced external documentation.
- **Repository evidence:** source/configuration references observed in the repository.
- **Execution evidence:** compilation, automated test, physical display/touch operation, deployment, or other runtime verification.

V6 and V14.1 provide documentation and repository-level evidence. They must not be interpreted as execution evidence unless an explicit test record exists.

## 3.3 Implementation

### 3.3.1 Controlled implementation references

The V14.1 consolidation preserves these exact implementation evidence markers for traceability:

`st7701_type8_init_operations`

`server/deviceApi.ts`

`server/deviceCommands.ts`

`server/reviewControl.ts`

The references are documentary traceability markers. They are not instructions to modify source code.



Pending controlled documentation section. This section is reserved for implementation evidence after the current 3.2 design documentation has been completed and verified.

## 3.4 Validation

Pending controlled documentation section. Validation results must be reported from actual evidence and must not be inferred from compilation, source inspection, or documentation alone.

### 3.4.1 Evidence boundary

Validation in this document remains an evidence boundary. Repository inspection and external WEB references are not substitutes for execution, automated test, physical display/touch operation, deployment, or other runtime evidence.

## 3.5 Commissioning

Pending controlled documentation section. Commissioning remains outside the scope of this README consolidation unless separately authorized and supported by execution evidence.

### 3.5.1 Commissioning boundary

No commissioning result is claimed by the present documentation consolidation. Any future commissioning record must be added under this hierarchy with dated and reproducible evidence.

## 3.6 Final integrated documentation control

This README is one documentation set containing the V6 Benchmark baseline, the preserved V14.1 design/evidence record, and the V15 consolidation/control layer.

**WEB FIRST then REPOSITORY AFTER**

**No modification or reflashing of firmware**

The controlled sequence remains:

**WEB FIRST → VERIFY → REPOSITORY AFTER → BYTE-FOR-BYTE VALIDATION**

The consolidation is documentation-only. It does not modify firmware source, firmware configuration, hardware, GPIO assignments, display initialization, backend architecture, or unrelated repository files. V14.1 and V15 remain represented together in this single README.
