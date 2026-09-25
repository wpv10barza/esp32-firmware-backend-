T-030 System ESP32-S3-4848S040 plus Backend Asistente 3C

# V15 Integrated Documentation — V14.1 + V15 (V6 Benchmark retained)

This README is the single consolidated documentation record for the T-030 system. It **retains the V6 Benchmark baseline** and **integrates the V14.1 design/evidence expansion**. Neither version is treated as a replacement for the other.

The governing documentation sequence is:

**WEB FIRST then REPOSITORY AFTER**

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

## 3.1. Condiciones iniciales del sistema T-030

Las condiciones iniciales que se deberán considerar para satisfacer los requerimientos de diseño, arquitectura, comunicación, validación y persistencia del sistema T-030 se establecen a continuación. Cada condición se formula de manera prescriptiva y deberá mantenerse coherente con la implementación vigente del repositorio del firmware y con la versión del repositorio complementario `wpv10barza/asistente-3c` utilizada por el flujo de integración.

### a) Condiciones iniciales de unificación de versiones y alcance documental

• Cuando dos versiones de la documentación contengan declaraciones equivalentes, el documento consolidado deberá mantener una única declaración técnicamente consistente.

• Cuando una declaración de la versión V14.1 añada detalles que no estén presentes en la V15, dicha información se deberá conservar dentro de la sección técnica que corresponda.

• Cuando la versión V15 incorpore información adicional de línea base, trazabilidad o control documental, dicha información se deberá mantener sin duplicar innecesariamente el contenido técnico.

• Cuando una declaración implique una prueba física, programación del dispositivo, *flashing*, despliegue o resultado de ejecución, la documentación deberá utilizar únicamente el nivel de evidencia que pueda demostrarse mediante registros reproducibles.

• La documentación deberá diferenciar explícitamente entre evidencia externa, evidencia de repositorio y evidencia de ejecución.

### b) Condiciones iniciales del sistema de desarrollo

• El sistema T-030 deberá comprender dos subsistemas coordinados: el firmware del panel ESP32-S3-4848S040 y el backend Asistente 3C implementado en el repositorio complementario `wpv10barza/asistente-3c`.

• El firmware deberá concentrar la interacción física con el usuario, la representación visual, la captura táctil, la edición local de comandos, la comunicación Wi-Fi y el transporte de órdenes hacia el servicio 3C.

• El backend deberá concentrar la API del dispositivo, la normalización de órdenes, el almacenamiento temporal de comandos, la interpretación estructurada mediante inteligencia artificial, la validación determinista y el control de revisión humana.

• La interfaz web deberá coordinar el acceso autenticado a Google Sheets, la localización de la tarea, la generación de la vista previa, la confirmación o rechazo de la propuesta y la comunicación del resultado final hacia el dispositivo.

• Se deberá mantener la separación entre interacción embebida, interpretación probabilística, validación determinista, revisión humana y persistencia.

### c) Condiciones iniciales del subsistema de firmware y plataforma electrónica

• El firmware deberá utilizar el entorno de compilación `panel_4848s040` definido en `platformio.ini`, basado en la plataforma `espressif32@6.8.1`, framework Arduino y placa `esp32-s3-devkitm-1`.

• La configuración deberá establecer 16 MB de Flash y PSRAM OPI, con `BOARD_HAS_PSRAM=1`.

• La interfaz gráfica deberá operar a 480 × 480 píxeles mediante Arduino-GFX y una arquitectura RGB con inicialización `st7701_type8_init_operations`.

• El sistema táctil deberá operar mediante el controlador GT911 sobre I²C, utilizando las líneas definidas por el firmware para SDA y SCL y la dirección I²C `0x5D`.

• El firmware deberá mantener una máquina de estados con los estados `Booting`, `Offline`, `Ready`, `Busy`, `Pending`, `Applied`, `Rejected` y `Error`, de manera que la recepción de una orden no se confunda con su aplicación efectiva.

• La interfaz de edición deberá utilizar un búfer de comandos con capacidad máxima de 240 caracteres, cursor independiente, ventana de visualización, teclado virtual, inserción, eliminación, desplazamiento y cambio entre los modos alfabético y numérico.

• El firmware deberá generar las solicitudes del dispositivo utilizando `device_id`, `request_id` y el texto del comando, permitiendo identificar retransmisiones y evitar duplicados lógicos.

• El firmware deberá soportar, como mínimo, los recursos de la API `/api/device/v1/health`, `/api/device/v1/commands`, `/api/device/v1/commands/pending`, `/api/device/v1/commands/:id` y `/api/device/v1/commands/:id/result`.

• El dispositivo no deberá ejecutar escritura directa sobre Google Sheets. Su responsabilidad deberá limitarse a capturar, editar, transportar y consultar el estado de la orden, además de presentar el resultado al usuario.

• La funcionalidad de audio deberá mantenerse opcional mediante `PANEL_AUDIO_ENABLED_VALUE`; cuando se encuentre habilitada, el firmware deberá utilizar la configuración I²S definida en el código y, cuando se encuentre deshabilitada, los GPIO asociados podrán reservarse para la variante física correspondiente.

### d) Condiciones iniciales de descubrimiento del backend y comunicación de red

• El firmware deberá utilizar descubrimiento dinámico del backend mediante mDNS y no deberá depender de una dirección IPv4 privada fija como autoridad de configuración en tiempo de ejecución.

• El servicio deberá descubrirse mediante la consulta mDNS `_3c._tcp` y deberá asociarse al nombre lógico estable `3c-backend.local`.

• El resultado del descubrimiento deberá incluir la dirección IP resuelta y el puerto del servicio.

• El firmware deberá conservar en NVS, dentro del espacio `backend`, el host lógico, la dirección resuelta y el puerto mediante `Preferences`.

• El endpoint almacenado en NVS deberá cargarse durante `setup()` antes del establecimiento de la conexión Wi-Fi, permitiendo utilizar una dirección previamente conocida como primera referencia.

• Cuando no exista un endpoint válido en caché, se deberá efectuar descubrimiento mDNS antes de intentar la comprobación de salud del backend.

• Cuando la comprobación `GET /api/device/v1/health` falle, el firmware deberá realizar una nueva consulta mDNS y efectuar un único reintento de comprobación.

• Los comandos y el sondeo de estado deberán utilizar el endpoint descubierto en tiempo de ejecución y no una dirección codificada en el firmware.

• La comunicación ESP32 → backend deberá mantenerse mediante HTTP en la implementación local vigente. El acceso del backend hacia Google deberá realizarse mediante HTTPS.

### e) Condiciones iniciales del subsistema backend Asistente 3C

• El backend deberá ejecutarse como aplicación Node.js con TypeScript y Express y deberá integrar una interfaz React/Vite.

• La configuración vigente deberá utilizar `@google/genai` para la interpretación estructurada de órdenes y deberá permitir seleccionar el modelo mediante la variable `GEMINI_MODEL`, utilizando `gemini-2.5-flash` como valor predeterminado.

• La API del dispositivo deberá exponer un recurso de salud, un recurso de recepción de comandos, un recurso de comandos pendientes, un recurso de consulta de estado y un recurso de reporte del resultado.

• La normalización del comando deberá exigir `device_id` y `text`, aceptar `request_id` explícito o generarlo automáticamente y rechazar identificadores con caracteres no permitidos.

• El almacenamiento de órdenes deberá mantener los estados `pending_confirmation`, `applied` y `rejected`, con un tiempo de vida de 15 minutos y una capacidad máxima de 50 órdenes residentes.

• La recepción de comandos deberá controlar duplicados mediante la combinación de `device_id` y `request_id`.

• La autenticación del dispositivo deberá utilizar `ESP32_API_TOKEN`; cuando el token no se encuentre configurado, la API de comandos y estado deberá permanecer bloqueada salvo que `ALLOW_INSECURE_DEVICE_API=true` se habilite expresamente para desarrollo.

• La integración CI deberá utilizar el repositorio `wpv10barza/asistente-3c` fijado al commit configurado en el workflow del firmware, evitando que el proceso de prueba dependa implícitamente de una versión móvil del backend.

### f) Condiciones iniciales de interpretación, validación determinista y control humano

• La interpretación mediante Google GenAI deberá producir una salida JSON estructurada y no deberá ejecutar directamente cambios sobre la fuente maestra.

• El contrato de salida deberá identificar, como mínimo, `tarea_buscada`, `tarea_id`, `operaciones`, `requiere_revision` y `motivo_revision`, utilizando un esquema explícito.

• Las operaciones permitidas deberán estar restringidas a los campos definidos en `FIELD_RULES`: ItemMantenible, ModoDeFalla, Restriccion, LimitesAceptables, ComentariosCondicionales, Origen, Frecuencia, UnidadTiempo, Especialidad y Labour1.

• Las columnas modificables deberán permanecer restringidas a B, C, H, I, J, K, L, M, N y O.

• La aplicación deberá verificar que cada columna real coincida con el encabezado esperado antes de aceptar una operación.

• Los campos de catálogo deberán coincidir con valores existentes en los catálogos detectados y no deberán introducir valores inventados.

• La frecuencia deberá representarse como entero mayor o igual a uno y las unidades de tiempo deberán normalizarse hacia los valores admitidos por el contrato.

• Los campos de texto largo no deberán aceptar expresiones incompletas como `...`, `…` o `etc.`.

• La tarea deberá localizarse mediante la columna F (`Nombre`) o mediante la columna E (`TareaId`) cuando el usuario lo indique explícitamente.

• Cuando el modelo no produzca operaciones válidas o declare que se requiere revisión, el proceso no deberá continuar hacia la aplicación automática de cambios.

• La interfaz deberá presentar una vista previa con la fila objetivo, el valor identificado y cada operación propuesta antes de habilitar la escritura.

• La confirmación humana deberá mantenerse como condición previa a la aplicación de los valores en Google Sheets.

• El rechazo deberá cerrar la propuesta sin aplicar los cambios y deberá reportar el estado `rejected` al comando del dispositivo cuando exista un comando externo asociado.

### g) Condiciones iniciales de la fuente maestra y acceso a Google Sheets

• La fuente maestra deberá utilizar Google Sheets como almacenamiento estructurado de la estrategia 3C.

• La interfaz web deberá autenticarse mediante OAuth de Google utilizando el alcance `https://www.googleapis.com/auth/spreadsheets`.

• La configuración deberá obtener desde `/api/config` el identificador de la hoja, el nombre de la pestaña, la fila de encabezados y la columna de búsqueda.

• Antes de preparar una modificación, la interfaz deberá leer los encabezados reales de la hoja y verificar que E corresponda a `TareaId` y F corresponda a `Nombre`.

• La interfaz deberá obtener los catálogos existentes desde las columnas B, C, N y O para proporcionar contexto real a la etapa de interpretación.

• La localización de la fila deberá exigir una coincidencia única. Cuando existan varias coincidencias, el sistema deberá detener el procesamiento y solicitar una identificación más precisa.

• La escritura deberá realizarse mediante solicitudes autenticadas contra la API de Google Sheets y únicamente después de la confirmación explícita de la vista previa.

• No se deberá atribuir al firmware, al endpoint `/api/extract` ni al API de comandos la autoridad directa de escritura sobre Google Sheets, ya que la aplicación web autenticada ejecuta esa operación.

• No se deberá considerar Google Cloud Storage como mecanismo de persistencia de la estrategia 3C mientras no exista una ruta de implementación que lo utilice efectivamente.

### h) Condiciones iniciales del ciclo de revisión y bloqueo temporal

• Cada propuesta deberá registrar fila, coincidencia encontrada, operaciones, identificador externo opcional, estado, fecha de creación, fecha de actualización y fecha de expiración.

• Las propuestas deberán utilizar los estados `proposed`, `approved` y `rejected`.

• El sistema deberá mantener un bloqueo temporal por fila para impedir que dos propuestas activas modifiquen simultáneamente la misma posición lógica.

• El tiempo de vida predeterminado del bloqueo y de la propuesta deberá ser de cinco minutos.

• Una propuesta expirada deberá impedir su aprobación y deberá liberar el bloqueo correspondiente.

• Una propuesta aprobada deberá liberar el bloqueo de la fila una vez que el flujo de revisión haya concluido.

### i) Condiciones iniciales de configuración y credenciales

• Los valores propios del entorno del dispositivo deberán mantenerse en `include/local_config.h`, archivo que deberá permanecer fuera del control de versiones.

• El firmware no deberá incorporar credenciales reales de Wi-Fi ni tokens de dispositivo dentro de `app_config.h` o `main.cpp`.

• El backend deberá utilizar variables de entorno para `GEMINI_API_KEY`, `GEMINI_MODEL`, `ESP32_API_TOKEN`, `PORT`, `SPREADSHEET_ID`, `SHEET_NAME` y `HEADER_ROW`, además de los parámetros de CI que correspondan.

• La variable `ASSISTANT_BASE_URL_VALUE` se deberá conservar únicamente como compatibilidad de configuración y no deberá utilizarse como autoridad de endpoint en tiempo de ejecución.

• La configuración del sistema deberá separar código reproducible, credenciales, parámetros locales y artefactos de prueba.

### j) Condiciones iniciales de seguridad y límites de confianza

• La recepción de comandos del dispositivo deberá utilizar autenticación mediante token y comparación segura del valor configurado.

• La generación mediante IA deberá considerarse una etapa probabilística y no una autoridad de escritura.

• La validación determinista deberá constituir la barrera de cumplimiento del contrato de columnas, encabezados, catálogos, tipos y unidades.

• La confirmación humana deberá permanecer como barrera previa a la persistencia.

• La comunicación local ESP32 → backend mediante HTTP deberá declararse como una limitación de seguridad del estado vigente.

• Los valores sensibles no deberán imprimirse en los diagnósticos seriales ni almacenarse dentro de archivos controlados por Git.

### k) Condiciones iniciales del flujo de datos del sistema completo

• El flujo deberá iniciar con una interacción táctil o con una orden previamente estructurada en el panel.

• El firmware deberá establecer conectividad Wi-Fi, resolver el backend mediante mDNS cuando corresponda, recuperar o actualizar el endpoint en NVS y comprobar la disponibilidad del servicio.

• La orden deberá transmitirse al endpoint de comandos con identificación de dispositivo y de petición.

• El backend deberá normalizar la orden, detectar duplicados y mantenerla en estado `pending_confirmation`.

• La interfaz web deberá recuperar la orden pendiente, leer encabezados y catálogos reales, invocar `/api/extract`, localizar la tarea y registrar la propuesta para revisión.

• La interfaz deberá mostrar la vista previa y requerir confirmación antes de modificar la hoja.

• Una confirmación deberá aplicar los valores autorizados, cerrar la propuesta como aprobada y reportar `applied` al dispositivo.

• Un rechazo deberá cerrar la propuesta como rechazada y reportar `rejected` al dispositivo.

• Una respuesta HTTP inválida, un estado de protocolo desconocido, la ausencia de `command_id`, un error de transporte o un resultado no reconocido deberán conducir al estado visible de error del panel y no deberán permanecer silenciosamente en estado pendiente.

### l) Condiciones iniciales de pruebas y evidencia de ejecución

• El repositorio deberá mantener pruebas de contrato para Wi-Fi, descubrimiento del backend y máquina de estados del panel.

• La integración continua deberá ejecutar las pruebas de contrato antes de compilar el firmware y deberá comprobar además la aplicación real Asistente 3C mediante pruebas de TypeScript, API, E2E de navegador y E2E entre monitor y API del dispositivo.

• El firmware deberá compilarse en CI para el entorno `panel_4848s040` y deberá generarse un paquete reproducible con `firmware.bin`, `bootloader.bin`, `partitions.bin`, `firmware.elf`, disposición de *flash* y sumas SHA-256.

• El manifiesto de CI deberá diferenciar una compilación validada de una programación física del dispositivo y no deberá presentar la compilación como evidencia de *flashing*.

### m) Condiciones iniciales mínimas obligatorias y criterio editorial

• El sistema T-030 deberá mantener coordinados el firmware ESP32-S3-4848S040 y el backend Asistente 3C como subsistemas complementarios.

• La documentación deberá redactarse en tercera persona, con lenguaje técnico, prescriptivo y verificable, utilizando `deberá`, `debe` y `deberán` para establecer condiciones obligatorias.

• Cada condición deberá corresponder a una característica comprobable en el código, la configuración, las pruebas o una fuente externa claramente identificada.

• No se deberán incorporar como condiciones implementadas componentes o tecnologías que no estén sustentados por el código vigente.

• Los detalles de bajo nivel, comandos de instalación, credenciales y configuraciones específicas del entorno deberán permanecer fuera del cuerpo normativo cuando su inclusión reduzca la trazabilidad o exponga información sensible.

### 3.1.14. Fuentes de implementación verificadas

La redacción de esta sección deberá mantenerse sincronizada, como mínimo, con las siguientes fuentes de implementación:

• `platformio.ini`: entorno de compilación, plataforma, memoria, PSRAM, dependencias y objetivo `panel_4848s040`.

• `include/app_config.h`: configuración local, identificador de dispositivo, parámetros temporales, brillo, audio y compatibilidad de endpoint.

• `src/main.cpp`: inicialización del display, touch, audio, Wi-Fi, mDNS, NVS, API HTTP, máquina de estados, editor de comandos, sondeo y tratamiento de errores.

• `wpv10barza/asistente-3c/server.ts`: servidor Node/Express, integración GenAI, contrato de extracción, configuración de Google Sheets, propuestas y revisión.

• `wpv10barza/asistente-3c/server/deviceApi.ts`: contrato HTTP del dispositivo y autorización mediante token.

• `wpv10barza/asistente-3c/server/deviceCommands.ts`: normalización, deduplicación, estados, TTL y límite de comandos.

• `wpv10barza/asistente-3c/server/reviewControl.ts`: propuestas, estados, bloqueo de filas y expiración.

• `wpv10barza/asistente-3c/src/App.tsx`: OAuth de Google, lectura de encabezados y catálogos, localización de tareas, vista previa, escritura autenticada y reporte del resultado al dispositivo.

• `.github/workflows/firmware-and-pages.yml` y los archivos de pruebas del repositorio: evidencia automatizada de contratos, E2E, compilación y empaquetado.

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

### 3.2.2 Software design

The software architecture is documented as a coordinated web and device-communication system. The application layer uses Node.js-oriented server components, an Express-compatible HTTP API pattern, a React-based interface, and generative-AI-assisted command interpretation. The embedded panel communicates with the service through a controlled device contract rather than writing directly to a master data source.

#### Node.js

The software design records Node.js as the server/runtime family used by the Asistente 3C application layer. The purpose of the server is to receive and coordinate structured device requests, preserve request identity, and support the controlled command lifecycle.

#### Express

Express is documented as the HTTP service layer used to expose the device-facing contract. The architecture separates transport from the decision to apply a change. An accepted HTTP request therefore represents reception or progression through the workflow, not automatic authorization to write master data.

#### React

React is documented as the user-interface layer for the controlled review workflow. The interface can present a proposed operation, expose the resulting fields or state, and keep a human confirmation step between interpretation and persistence.

#### GenAI

Generative AI is documented as an interpretation component. Its function is to convert natural-language instructions into a structured representation suitable for deterministic checks. The AI component is not treated as the direct authority for persistence.

#### Repository verification

The required software evidence markers for the V14.1 record are preserved explicitly:

- `server/deviceApi.ts`
- `server/deviceCommands.ts`
- `server/reviewControl.ts`

`server/deviceApi.ts` is retained as the device API evidence reference; `server/deviceCommands.ts` as the command normalization/state reference; and `server/reviewControl.ts` as the human-review control reference. The README records these as implementation evidence markers and does not modify their contents as part of this documentation consolidation.

### 3.2.3 Diseño del agente de IA - Modelo de inteligencia artificial y procesamiento controlado

El agente de inteligencia artificial constituye la capa de interpretación semántica del sistema Asistente 3C. Su función es transformar una instrucción expresada en lenguaje natural en una representación estructurada de la tarea y de las operaciones solicitadas, la cual posteriormente es sometida a validaciones deterministas y al flujo de revisión humana. En consecuencia, la generación del modelo no se considera una autorización autónoma para modificar la fuente maestra.

La implementación vigente utiliza la biblioteca `@google/genai`. El modelo configurado por defecto es `gemini-2.5-flash`, aunque su selección puede sustituirse mediante la variable de entorno `GEMINI_MODEL`. Esta configuración mantiene separado el comportamiento del software respecto del identificador concreto del modelo utilizado durante una ejecución determinada.

#### 3.2.3.1 Función del modelo

La llamada al modelo se realiza mediante `ai.models.generateContent`. La configuración establece `temperature: 0`, orientando la generación hacia un comportamiento controlado y reduciendo la variabilidad en la interpretación de comandos equivalentes.

La salida se solicita con `responseMimeType: "application/json"` y mediante un `responseSchema` definido explícitamente. El esquema establece campos para la tarea buscada, el identificador de tarea cuando corresponda, las operaciones propuestas y la indicación de si se requiere revisión. El resultado del modelo se procesa como una estructura verificable y no como texto libre destinado a ejecutar cambios.

La función del modelo se limita, por tanto, a la interpretación semántica. El flujo lógico posterior conserva la separación entre modelo, salida estructurada, validación determinista, localización de tarea, propuesta, revisión humana y persistencia autorizada.

#### 3.2.3.2 Entrada contextual y grounding con información real

La implementación recibe `detectedHeaders` y `detectedCatalogs` como contexto para la etapa de interpretación. Los encabezados permiten contrastar la estructura real de la hoja, mientras que los catálogos proporcionan los valores existentes que pueden utilizarse en los campos categóricos controlados.

La identificación de la tarea se mantiene vinculada a la estructura de la estrategia. La implementación establece la búsqueda por `Nombre` en la columna F y permite utilizar `TareaId` en la columna E cuando el usuario lo especifica explícitamente. Esta distinción evita que el modelo invente identificadores o interprete como identidad una columna diferente de la establecida por el contrato.

Los catálogos utilizados como contexto corresponden a `ItemMantenible`, `ModoDeFalla`, `Especialidad` y `Labour1`. La finalidad de este mecanismo es restringir la interpretación a valores que realmente existen en la fuente contextualizada.

En esta implementación específica no se evidencia una recuperación vectorial para la etapa de `/api/extract`. El grounding documentado para este componente es tabular y estructural: encabezados, catálogos y reglas de operación. No se atribuye aquí una arquitectura RAG vectorial que no aparece implementada en este flujo.

#### 3.2.3.3 Contrato de salida estructurada

El `responseSchema` define una estructura de respuesta que contiene, como mínimo, `tarea_buscada`, `operaciones` y `requiere_revision`, además de `tarea_id` y `motivo_revision` cuando corresponda.

Cada operación identifica un campo permitido, su valor propuesto y opcionalmente una razón asociada. Los campos permitidos se encuentran definidos previamente en `FIELD_RULES`; por tanto, el modelo no determina libremente qué columnas del sistema pueden modificarse.

La lista blanca vigente comprende los campos asociados a las columnas B, C, H, I, J, K, L, M, N y O. Las columnas de identidad, búsqueda o cualquier columna fuera de la lista autorizada permanecen fuera del dominio de modificación.

El contrato estructurado establece así una frontera entre generación y ejecución: la inteligencia artificial propone una estructura y la aplicación determina si dicha estructura es aceptable.

#### 3.2.3.4 Validación determinista posterior al modelo

La respuesta generada se procesa mediante una segunda etapa de validación programática. Antes de aceptar cada operación, el sistema verifica que el campo recibido pertenezca a `FIELD_RULES`.

Posteriormente se comprueba que la columna asociada coincida con el encabezado esperado. Cuando existe una discrepancia entre la estructura detectada y la definición de una columna, la auditoría se detiene en lugar de continuar con una operación potencialmente incorrecta.

Los valores de catálogo se normalizan para comparación, pero el valor finalmente utilizado debe corresponder a un elemento existente del catálogo. Esto evita convertir una variación de mayúsculas, minúsculas o acentuación en un valor nuevo no autorizado.

Las frecuencias se convierten en valores enteros y deben ser mayores o iguales a uno. Las unidades de tiempo se normalizan hacia representaciones canónicas como `Mes`, `año`, `Semana`, `Dia` y `Hora`.

Los campos de texto largo rechazan expresiones incompletas como `...`, `…` o `etc.`, debido a que la información destinada a la fuente maestra debe conservar el contenido descriptivo completo.

#### 3.2.3.5 Límites de autoridad del agente de IA

El agente no posee autoridad directa para modificar la fuente maestra. El endpoint `/api/extract` interpreta la instrucción y devuelve una estructura de operaciones validada, pero no ejecuta por sí mismo una escritura sobre Google Sheets.

La recepción de una orden y su interpretación tampoco equivalen a su aplicación. El resultado del modelo se incorpora al proceso de propuesta y revisión, manteniendo separadas la interpretación, la validación y la persistencia.

La autoridad de la inteligencia artificial se limita a interpretar la intención expresada por el usuario dentro del contrato de campos, encabezados, catálogos y reglas proporcionado como contexto.

#### 3.2.3.6 Integración con revisión humana

Cuando la estructura resultante requiere revisión o no contiene operaciones válidas, el sistema establece `requiere_revision`. La propuesta puede registrarse posteriormente mediante `reviewStore`.

La propuesta conserva la fila objetivo, la coincidencia localizada, las operaciones solicitadas y, cuando corresponde, el identificador de la orden externa que originó el proceso. La revisión humana se mantiene como condición entre la propuesta generada y la persistencia.

Este diseño impide interpretar una respuesta correcta del modelo como una escritura automática. La decisión final permanece separada de la generación probabilística y se ejecuta mediante el flujo de aprobación o rechazo.

#### 3.2.3.7 Secuencia completa de procesamiento de una instrucción

El procesamiento se estructura en las siguientes etapas técnicas:

1. Recepción de la instrucción en lenguaje natural.
2. Incorporación de encabezados, estructura y catálogos disponibles.
3. Envío al modelo Gemini mediante `@google/genai`.
4. Generación de respuesta JSON con esquema definido.
5. Parseo de la respuesta.
6. Verificación determinista del campo y de la columna asociada.
7. Validación de catálogos, frecuencias, unidades y contenido textual.
8. Determinación de la tarea objetivo por `Nombre` o `TareaId`.
9. Generación de una propuesta controlada.
10. Revisión humana.
11. Persistencia después de la autorización correspondiente.

La secuencia mantiene separadas las responsabilidades de interpretación semántica y ejecución determinista. Un error de formato, una discrepancia de encabezado, un valor de catálogo inexistente o una condición no verificable interrumpe el avance normal del proceso en lugar de convertir la salida del modelo en una modificación directa.

### 3.2.4 Tactile interface design

Pending controlled documentation section. It is reserved for the tactile interaction design of the ESP32-S3-4848S040 panel, including the documented touch-controller interface, interaction regions, priority rules, cursor interaction, keyboard behavior, and touch-hit testing once the corresponding evidence is consolidated.

### 3.2.5 Tactile treatment

Pending controlled documentation section. It is reserved for the documented treatment of tactile events, debouncing or event filtering where evidenced, interaction priority, visual feedback, and state transitions.

### 3.2.6 Integrated evidence status

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
