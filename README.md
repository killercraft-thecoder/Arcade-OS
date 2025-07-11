# ARCADEOS

ARCADEOS is a modular operating system designed for the STM32F4 microcontroller family. It features a layered architecture with a Program Interface Layer (PIL), a File Management Layer (FML), a basic named file system implemented on flash storage, and a new kernel layer for core OS services.

## Features
- Modular abstraction layers (Kernel, PIL, FML, etc.)
- Named file system with buffer support (FLASH API)
- Task management and cooperative multitasking (now handled by the Kernel)
- RAM management and garbage collection (Kernel, with wrappers in core API/controlgc)
- Robust USB support for programming and recovery
- Bootloader/OS dual-mode operation
- Extensible for drivers, input, and more
- **Screen API (partial):** Provides a font (`font5` from MakeCode Arcade) and some frame/image buffer structures and stubs (see `frame.cpp`, `frame.h`, `screenimage.cpp`, etc.), but does not support actual screen updates or hardware rendering yet. The API is a foundation for future display support, but not functional for real screen output.
- **User-friendly programming:** Thanks to CODAL's GhostFAT, the device appears as a USB drive. Users can drag-and-drop files or programs onto the device, making programming and file management simple and accessible—no need for low-level flashing tools.
- **CODAL API 2 integration:** ARCADEOS uses CODAL for hardware abstraction, event/fiber management, and device support, making it easy to port to other STM32 boards and leverage high-level APIs.
- **SPI support:** SPI bus support is included (from MakeCode Arcade). PWM and I2C drivers are planned for future hardware features (e.g., speakers, sensors). (Also the Low-levle parts implment I2c drivers.)

## Flash Layout
- **0x0000–0x2000:** Bootloader (must never be overwritten by OS or updates)
- **0x2000–...:** Main firmware (OS, user code, etc.)

## Modes
- **Bootloader Mode:** Menu for firmware/bootloader updates and recovery. USB always available. Can be entered by software (`jump_to_bootloader()`) or hardware (button/pin).
- **OS Mode:** Normal operation. USB stack always initialized. OS can request a jump to bootloader for safe updates.

## Project Structure
- `boot/` — Bootloader and main entry point
- `os/` — Core OS logic, Kernel, PIL, FML, and declarations
  - `kernel/` — Kernel implementation for task, memory, and system management
- `FLASH API/` — Flash storage and file system implementation (with bootloader protection)
- `APIs/`, `SPI/`, `screen API/` — Hardware and peripheral APIs
  - `SPI/` — SPI bus support (from MakeCode Arcade)
  - `screen API/` — Provides a font and frame/image buffer stubs, but not screen update logic yet
  - *(Planned: PWM and I2C drivers for speakers, sensors, etc.)*
- `Codal API 2/` — Provides hardware abstraction, GhostFAT, event/fiber/device management, and USB drive support
- `core API/` — Core utilities and platform support (including USB, GC wrappers in controlgc, and bootloader helpers)
  - `configkeys.h`, `pxtconfig.h` — Board and pin configuration files. Update these for new hardware variants.

## Building
1. Ensure you have a C++ toolchain (e.g., GCC for ARM or MinGW for Windows).
2. Build the project using your preferred method or the provided VS Code tasks.

## Boot Process
- The system boots from `boot/main.cpp`, which initializes CODAL device/fiber/event systems, then the OS, mounts the filesystem, and starts the main loop.
- The Kernel is responsible for task and memory management.
- USB is always initialized for programming and recovery.
- GhostFAT enables drag-and-drop programming and file management.


## Note

Before Building for Hardware modify the `configkeys.h` and `pxtconfig.h` files to modify the configuration of the OS.
