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

Las condiciones iniciales que se deberán tener en cuenta en el sistema para satisfacer los requerimientos de diseño, arquitectura, comunicación, validación y persistencia de la presente especificación técnica se listan a continuación:

a) **Condiciones iniciales de unificación de versiones y alcance documental**

• Cuando dos versiones de la documentación contengan declaraciones equivalentes, el documento unificado deberá conservar una única formulación técnicamente consistente.

• Cuando una versión incorpore información técnica adicional que no se encuentre en la otra, dicha información deberá conservarse en la sección correspondiente, evitando duplicaciones innecesarias.

• Cuando una declaración se refiera a pruebas físicas, programación del dispositivo, *flashing*, despliegue o resultados de ejecución, la redacción deberá limitarse al nivel de evidencia que pueda demostrarse mediante registros verificables.

• La documentación deberá distinguir entre condiciones de diseño, evidencia obtenida del repositorio y resultados de ejecución.

b) **Condiciones iniciales del sistema de desarrollo**

• El sistema T-030 deberá comprender como subsistemas obligatorios y coordinados el firmware del panel ESP32-S3-4848S040 y el backend Asistente 3C.

• Ambos subsistemas deberán cumplir funciones complementarias y deberán mantener una separación de responsabilidades entre interacción física, transporte de órdenes, interpretación mediante inteligencia artificial, validación determinista, revisión humana y persistencia de información.

• El firmware deberá concentrar la interacción física con el usuario, la representación visual, la captura de eventos táctiles, la edición local de comandos y la comunicación con el servicio 3C.

• El backend deberá concentrar la recepción y normalización de órdenes, la autenticación del dispositivo, la interpretación estructurada, la validación determinista y el control del ciclo de revisión.

• La interfaz web deberá coordinar la recuperación de órdenes pendientes, la lectura de la estructura real de la hoja, la generación de propuestas, la presentación de la vista previa, la confirmación o rechazo y la comunicación del resultado al dispositivo.

• Se deberá mantener una separación explícita entre la generación de una propuesta y la autorización efectiva de la modificación de los datos maestros.

c) **Condiciones iniciales del subsistema de firmware y plataforma electrónica**

• El firmware se deberá compilar mediante el entorno `panel_4848s040`, configurado en `platformio.ini` para la plataforma Espressif32, el framework Arduino y la placa ESP32-S3 utilizada por el panel.

• La configuración deberá establecer una memoria Flash de 16 MB y el uso de PSRAM OPI, manteniendo las opciones de memoria definidas para el objetivo `panel_4848s040`.

• La interfaz de usuario deberá operar con una resolución lógica de 480 × 480 píxeles y deberá emplear la biblioteca GFX Library for Arduino junto con la arquitectura RGB y la secuencia de inicialización ST7701 definida por el firmware.

• El sistema táctil deberá utilizar el controlador GT911 mediante I²C, respetando la dirección y las líneas de comunicación establecidas en la implementación vigente.

• El firmware deberá incorporar una máquina de estados que diferencie las condiciones de inicio, indisponibilidad, disponibilidad, procesamiento, espera de confirmación, aplicación, rechazo y error.

• La interfaz deberá disponer de un mecanismo de edición de comandos con búfer de capacidad definida, cursor independiente, inserción y eliminación de caracteres, desplazamiento de la ventana de texto y teclado virtual adaptado a la geometría del panel.

• La gestión de órdenes deberá utilizar un identificador de dispositivo y un identificador de petición para permitir la identificación de retransmisiones y el control de duplicados lógicos.

• El dispositivo no deberá ejecutar escritura directa sobre la fuente maestra y deberá limitar su función a capturar, editar, transmitir y consultar el estado de las órdenes.

d) **Condiciones iniciales del subsistema backend Asistente 3C**

• El backend deberá ejecutarse como una aplicación basada en Node.js, TypeScript y Express, complementada con una interfaz React ejecutada mediante Vite.

• La aplicación deberá utilizar la biblioteca `@google/genai` para realizar la interpretación estructurada de las instrucciones recibidas.

• La selección del modelo deberá poder controlarse mediante la variable de entorno correspondiente, manteniendo un modelo predeterminado definido por la implementación vigente.

• La API del dispositivo deberá disponer de recursos para comprobar la salud del servicio, recibir órdenes, consultar órdenes pendientes, consultar el estado de una orden y reportar el resultado de una operación.

• Las solicitudes provenientes del panel deberán normalizarse y deberán contener, como mínimo, el identificador del dispositivo y el texto de la orden.

• El almacenamiento temporal de órdenes deberá diferenciar los estados pendiente de confirmación, aplicado y rechazado, y deberá controlar duplicados mediante la identificación conjunta del dispositivo y de la petición.

• El almacenamiento deberá conservar las restricciones de tiempo de vida y capacidad establecidas por la implementación vigente para evitar la permanencia indefinida de órdenes.

• El acceso de los dispositivos a los recursos protegidos deberá estar condicionado por el mecanismo de autenticación configurado en el backend.

e) **Condiciones iniciales de interpretación, validación y control humano**

• La interpretación de las órdenes se deberá ejecutar mediante Google GenAI con una salida estructurada en formato JSON.

• El modelo deberá limitar su función a transformar el lenguaje natural en una representación estructurada de la tarea y de las operaciones solicitadas y no deberá poseer autoridad directa para modificar la fuente maestra.

• La salida deberá generarse mediante un esquema estructurado y deberá mantener el contrato definido para identificar la tarea, las operaciones propuestas y la necesidad de revisión.

• La validación posterior deberá ser determinista y deberá verificar los campos permitidos, la correspondencia entre columnas y encabezados, los tipos de datos, las unidades de tiempo y la pertenencia de los valores categóricos a los catálogos existentes.

• La frecuencia deberá representarse mediante un entero mayor o igual a uno y las unidades de tiempo deberán corresponder a los valores admitidos por el contrato de la aplicación.

• Los campos de texto largo no deberán aceptar expresiones incompletas que puedan introducir ambigüedad en la información maestra.

• Las operaciones modificables deberán permanecer restringidas a las columnas B, C, H, I, J, K, L, M, N y O.

• Cuando una condición de validación no pueda comprobarse, el proceso deberá detenerse sin convertir la salida del modelo en una modificación automática.

• La interfaz humana deberá presentar una vista previa de la fila objetivo y de los cambios propuestos antes de efectuar cualquier escritura.

• La persistencia deberá habilitarse únicamente después de una confirmación explícita y el rechazo deberá cerrar la propuesta sin aplicar los cambios.

f) **Condiciones iniciales de la fuente maestra y acceso a Google Sheets**

• La fuente maestra deberá utilizar Google Sheets como almacenamiento estructurado de la estrategia 3C.

• La interfaz web deberá autenticarse mediante OAuth de Google con permisos de edición sobre la hoja requerida y deberá utilizar HTTPS para comunicarse con los servicios de Google.

• Antes de preparar una modificación, la aplicación deberá obtener la configuración de la hoja y deberá verificar que la estructura real corresponda a la plantilla esperada.

• La estructura de búsqueda deberá conservar `TareaId` en la columna E y `Nombre` en la columna F; una discrepancia deberá detener el procesamiento antes de efectuar modificaciones.

• La aplicación deberá obtener los catálogos existentes de las columnas B, C, N y O y deberá utilizar dichos valores como contexto real para la interpretación.

• La localización de la tarea deberá realizarse mediante el nombre o mediante el identificador de tarea cuando este sea proporcionado expresamente.

• La coincidencia de la tarea deberá ser única; una búsqueda inexistente o ambigua deberá detener el proceso antes de modificar la fuente maestra.

• La lectura y escritura de la hoja deberá efectuarse desde la interfaz web autenticada y la escritura deberá ejecutarse únicamente después de la confirmación de la propuesta.

g) **Condiciones iniciales de conexiones y arquitectura lógica**

• La arquitectura electrónica deberá considerar la alimentación del panel, la interfaz de visualización RGB, el sistema táctil y los elementos auxiliares correspondientes a la variante física empleada.

• Los detalles de las asignaciones eléctricas deberán mantenerse en el documento técnico o anexo correspondiente y no deberán inferirse a partir de configuraciones de otras variantes del hardware.

• La interfaz lógica deberá mantener una resolución de 480 × 480 píxeles para la interacción y la presentación de estados.

• El panel deberá establecer conectividad Wi-Fi antes de iniciar la comunicación con el backend.

• El dispositivo deberá utilizar el endpoint dinámico obtenido mediante el mecanismo de descubrimiento del servicio y no una dirección fija definida como autoridad de ejecución.

• La comunicación local entre el panel y el backend deberá mantenerse diferenciada del acceso HTTPS que la interfaz web utiliza para los servicios de Google.

h) **Condiciones iniciales del entorno de software y configuración**

• El entorno de desarrollo deberá disponer de PlatformIO y del framework Arduino para la compilación del firmware, junto con las dependencias gráficas declaradas por el proyecto.

• El backend deberá disponer de Node.js, npm, TypeScript, Express, React, Vite, Google GenAI y las dependencias requeridas para la autenticación y operación de la interfaz.

• Los parámetros propios del entorno del firmware deberán mantenerse en el archivo de configuración local previsto y los parámetros sensibles del backend deberán mantenerse mediante variables de entorno.

• Las credenciales y demás valores privados no deberán incorporarse al código controlado por Git.

• La configuración de ambos subsistemas deberá permanecer coherente en cuanto a identificación del dispositivo, autenticación, comunicación, configuración de Google y estructura esperada de la hoja.

• Una incompatibilidad de configuración deberá detener el procesamiento normal antes de la aplicación de cambios.

i) **Condiciones iniciales de red e infraestructura**

• La infraestructura mínima deberá contar con una red Wi-Fi capaz de establecer comunicación LAN entre el panel y el host que expone el backend.

• El firmware no deberá depender de una dirección de bucle local ni de una dirección IPv4 privada fija como autoridad del endpoint en tiempo de ejecución.

• El backend deberá descubrirse mediante el servicio mDNS `_3c._tcp` y deberá asociarse al nombre lógico estable `3c-backend.local`.

• El proceso de descubrimiento deberá proporcionar la dirección IP resuelta y el puerto del servicio.

• El firmware deberá conservar en NVS, mediante `Preferences`, el host lógico, la dirección resuelta y el puerto del endpoint.

• El endpoint almacenado deberá cargarse antes de establecer la conexión Wi-Fi y deberá utilizarse como primera referencia cuando resulte válido.

• Cuando no exista un endpoint válido en caché, se deberá efectuar un nuevo descubrimiento mDNS antes de comprobar la salud del backend.

• Cuando falle `GET /api/device/v1/health`, se deberá realizar una nueva resolución del servicio y un único reintento de la comprobación.

• La infraestructura deberá permitir que el backend sea accesible desde el panel y deberá mantener la conectividad externa requerida por los servicios de Google.

j) **Condiciones iniciales de seguridad**

• Las credenciales de Wi-Fi, los tokens de autenticación, las claves de integración con Google GenAI y los demás parámetros sensibles deberán mantenerse fuera de los archivos controlados por Git.

• La autenticación del dispositivo deberá realizarse mediante el token configurado para el servicio y deberá utilizar el mecanismo de comparación segura definido por la implementación.

• Las solicitudes que requieran autorización y que no satisfagan la condición de autenticación deberán ser rechazadas antes de procesar la orden.

• La generación mediante inteligencia artificial deberá constituir una etapa probabilística y no una autoridad de escritura.

• La validación determinista deberá constituir la barrera de cumplimiento del contrato de columnas, encabezados, catálogos, tipos y unidades.

• La confirmación humana deberá mantenerse como condición previa a la persistencia.

• La comunicación entre firmware y backend deberá mantenerse mediante HTTP en la implementación local vigente y deberá considerarse una limitación de seguridad fuera de una red controlada.

• El acceso de la interfaz web hacia Google deberá realizarse mediante HTTPS y OAuth.

k) **Condiciones iniciales del flujo de datos del sistema completo**

• El flujo deberá iniciar con una interacción del usuario en el panel ESP32-S3-4848S040.

• El firmware deberá capturar la interacción, gestionar el comando mediante el búfer local y transmitir la orden al servicio 3C cuando corresponda.

• Antes de utilizar el backend, el firmware deberá disponer de un endpoint válido obtenido desde la caché NVS o mediante descubrimiento mDNS.

• El dispositivo deberá consultar el recurso de salud del backend y deberá reflejar las condiciones de conectividad en la máquina de estados del panel.

• La orden deberá enviarse al recurso de comandos con la identificación del dispositivo, la identificación de la petición y el texto correspondiente.

• El backend deberá autenticar la solicitud cuando corresponda, normalizar el contenido, detectar duplicados y mantener la orden en estado pendiente de confirmación.

• La interfaz web deberá recuperar la orden pendiente, leer los encabezados y catálogos reales, interpretar la instrucción, localizar la tarea y registrar la propuesta de revisión.

• La propuesta deberá presentarse mediante una vista previa antes de efectuar cualquier modificación.

• Después de una confirmación explícita, la interfaz deberá aplicar las operaciones autorizadas, aprobar la propuesta y comunicar al dispositivo el resultado correspondiente.

• Cuando la propuesta sea rechazada, no se deberán efectuar modificaciones y se deberá comunicar el resultado de rechazo cuando exista una orden externa asociada.

• Cuando se produzca un error de transporte, un error de protocolo, la ausencia de un identificador de comando o un estado no reconocido, el firmware deberá representar la condición mediante el estado de error.

l) **Componentes no establecidos como condición inicial**

• No se deberán considerar como componentes implementados del sistema T-030 los servicios de Google Cloud Storage, los buckets de almacenamiento, un backend Python basado en FastAPI o Flask ni cualquier otro componente que no esté sustentado por la implementación vigente.

• El firmware no deberá ejecutar escritura directa sobre Google Sheets.

• La persistencia deberá permanecer asociada a la interfaz web autenticada y deberá efectuarse únicamente después de la confirmación correspondiente.

• La configuración histórica destinada a una dirección fija del backend deberá considerarse únicamente como compatibilidad y no deberá utilizarse como autoridad del endpoint en tiempo de ejecución.

m) **Condiciones iniciales mínimas obligatorias y criterio editorial**

• El sistema T-030 deberá requerir la operación coordinada del firmware ESP32-S3-4848S040 y del backend Asistente 3C.

• El firmware deberá proporcionar la interacción física, la representación visual, el tratamiento táctil, la edición de comandos, la conectividad y el seguimiento de estados.

• El backend deberá proporcionar la API del dispositivo, la normalización y deduplicación de órdenes, la interpretación mediante Google GenAI, la validación determinista y el control de revisión.

• La interfaz web autenticada deberá proporcionar la conexión con Google Sheets, la lectura de la estructura real de la hoja, la localización de la tarea, la vista previa y la persistencia posterior a la confirmación.

• La línea base V6 Benchmark deberá conservarse como referencia técnica documentada y no deberá reinterpretarse como resultado de ejecución cuando no exista evidencia explícita.

• La documentación deberá redactarse en tercera persona, con lenguaje formal, técnico y prescriptivo, utilizando preferentemente los verbos “deberá”, “deberán”, “debe” y “deben” para establecer condiciones obligatorias.

• Cada condición deberá corresponder a una característica verificable en el código, la configuración, las pruebas o una fuente externa claramente identificada.

• Los detalles de bajo nivel que no constituyan una condición general, incluyendo credenciales, valores privados y procedimientos locales de diagnóstico, deberán reservarse para los documentos técnicos y anexos correspondientes.

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
