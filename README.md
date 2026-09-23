T-030 System ESP32-S3-4848S040 plus Backend Asistente 3C

# V14.1 Controlled Documentation Consolidation

## 1. Initial conditions

The T-030 system is documented as two coordinated subsystems: the ESP32-S3-4848S040 embedded panel and the Asistente 3C software backend. This README is a controlled documentation record. It describes the documented architecture, the implementation references required by the V14.1 consolidation, and the boundary between technical evidence and actions that are explicitly outside the scope of documentation work.

The governing documentation sequence is:

**WEB FIRST then REPOSITORY AFTER**

This sequence means that external technical information is checked before repository statements are consolidated. Repository content is then used to verify implementation references. Documentation does not authorize a firmware change, hardware intervention, or reprogramming operation.

The V14.1 record is intentionally limited to the documentation stage currently reached. The consolidated index reaches Initial conditions, 3.2 Design of the system, 3.2.1 Electronic design, and 3.2.2 Software design. Later implementation, validation, and commissioning sections remain pending in the controlled index.

## 2. Documentation control boundary

This consolidation publishes documentation only. It does not authorize firmware modification, firmware reflashing, hardware changes, display reinitialization, GPIO changes, backend architecture changes, or unrelated repository edits.

**No modification or reflashing of firmware**

The repository README is not treated as an executable control channel. A documentation update must not be interpreted as approval to alter, rebuild for deployment, flash, or physically reconfigure the ESP32-S3-4848S040 or any related equipment.

The controlled sequence remains:

**WEB FIRST → VERIFY → REPOSITORY AFTER → BYTE-FOR-BYTE VALIDATION**

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

## 3.2.3 Tactile interface design

Pending controlled documentation section. It is reserved for the tactile interaction design of the ESP32-S3-4848S040 panel, including the documented touch-controller interface, interaction regions, priority rules, cursor interaction, keyboard behavior, and touch-hit testing once the corresponding evidence is consolidated.

## 3.2.4 Tactile treatment

Pending controlled documentation section. It is reserved for the documented treatment of tactile events, debouncing or event filtering where evidenced, interaction priority, visual feedback, and state transitions.

## 3.3 Implementation

Pending controlled documentation section. This section is reserved for implementation evidence after the current 3.2 design documentation has been completed and verified.

## 4. Validation

Pending controlled documentation section. Validation results must be reported from actual evidence and must not be inferred from compilation, source inspection, or documentation alone.

## 5. Commissioning

Pending controlled documentation section. Commissioning remains outside the scope of this README consolidation unless separately authorized and supported by execution evidence.

## Controlled implementation references

The V14.1 consolidation preserves these exact implementation evidence markers for traceability:

`st7701_type8_init_operations`

`server/deviceApi.ts`

`server/deviceCommands.ts`

`server/reviewControl.ts`

The references are documentary traceability markers. They are not instructions to modify source code.

## Final control principle

This V15 process publishes documentation only.

**WEB FIRST then REPOSITORY AFTER**

**No modification or reflashing of firmware**

The governing sequence remains:

**WEB FIRST → VERIFY → REPOSITORY AFTER → BYTE-FOR-BYTE VALIDATION**