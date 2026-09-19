# ESP32-S3-4848S040 physical validation

The normal firmware workflow proves compilation and software contracts. A GitHub-hosted runner cannot prove that a real USB-connected panel boots, shows ST7701 pixels, responds to GT911, produces audible output, or remains electrically stable.

This repository therefore adds a separate physical workflow that runs only on a repository-owned self-hosted runner labeled esp32-panel.

## Artifact 10377040660

The exact image set is:
- bootloader.bin at 0x0000
- partitions.bin at 0x8000
- firmware.bin at 0x10000

The artifact manifest explicitly says compiled_not_physically_flashed.

Because CI rejects and does not build include/local_config.h, the exact artifact has no real Wi-Fi credentials. It can prove boot/peripheral initialization, but it cannot honestly prove Wi-Fi association or backend connectivity. A configured hardware image is required for that part.

## Automated physical checkpoints

The smoke test flashes the three binaries, captures the USB serial stream at 115200 baud, and checks:

SERIAL -> PSRAM -> ST7701 -> GT911 -> AUDIO -> GPIO MAP -> HTTP -> READY

It also watches reset markers and stability telemetry. A second boot/reset during the observation window fails the test.

The firmware exposes GET /api/diagnostics with PSRAM, heap, display, GT911, audio, Wi-Fi and backend state.

## Limits that remain physical

Audio initialization is automatic, but actual speaker audibility still needs human or measurement confirmation.

GPIO validation here is the firmware pin map/configuration; it is not an electrical continuity test.

Power stability is software-inferred from boot success and absence of reboot/brownout during the observation window. USB-C 5 V rail voltage needs an external measurement.

USB-C is validated as the physical USB serial path using USB CDC boot logging; connector electrical quality is not measured by CI.
