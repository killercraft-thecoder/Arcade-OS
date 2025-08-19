# ARCADEOS

**ARCADEOS** is a modular operating system for the STM32F4 microcontroller family. It features a layered architecture with:

- **Program Interface Layer (PIL)**
- **File Management Layer (FML)**
- A basic named file system on flash storage
- A new **Kernel Layer** for core OS services

---

## Features

- Modular abstraction layers: `Kernel`, `PIL`, `FML`, etc.
- Named file system with buffer support via **FLASH API**
- Cooperative multitasking and task management (handled by the Kernel, which really is just like a realy relaying.)
- RAM management and garbage collection (Kernel + wrappers in `core API/controlgc` + implmeantion in `core API/gc.cpp`)
- Robust USB support for programming and recovery
- Dual-mode operation: Bootloader and OS (really purly becuase you dont wn
  at your bootloader erased , and it makes ti easier.)
- Extensible architecture for drivers, input, and peripherals
- **Screen API (complete):** Includes font5 from makecode arcade , and has makecode arcade style images (though using the screen is manual , so the software running on the os , will need to do the screen stuff, even though plenty of it is already done.)
- **GhostFAT USB programming:** Appears as a USB drive for drag-and-drop programming—no flashing tools required
- **CODAL API 2 integration:** Hardware abstraction, event/fiber management, and device support
- **SPI support:** From MakeCode Arcade. PWM and I2C drivers planned for future expansion (but wait there is already I2C.h and SPI.h and their cpp's from codla , including the ZSPI stuff so you can already use it.)

---

## Flash Layout

| Region        | Address Range   | Description                     |
| ------------- | --------------- | ------------------------------- |
| Bootloader    | `0x0000–0x2000` | Must never be overwritten       |
| Main Firmware | `0x2000–...`    | OS, user code, and system logic |

Flash Layout is the Exact Same as the makecode arcade flash layout (apart from the well code section is diffrent.)

---

---
