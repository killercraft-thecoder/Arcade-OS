#include "F4Flash.h"
#include <stm32f4xx.h> // Or your CMSIS header for STM32F4

namespace codal {
    int F4Flash::pageSize(uintptr_t address) {
        address |= 0x08000000;
        if (address < 0x08010000)
            return 16 * 1024;
        if (address < 0x08020000)
            return 64 * 1024;
        if (address < 0x08100000)
            return 128 * 1024;
        // Optionally: error handling
        return 0;
    }

    int F4Flash::totalSize() {
        // Example: 1MB for STM32F407
        return 1024 * 1024;
    }

    int F4Flash::erasePage(uintptr_t address) {
        // Implement STM32F4-specific page erase logic here
        // ...
        return 0;
    }

    int F4Flash::writeBytes(uintptr_t dst, const void *src, uint32_t len) {
        // Implement STM32F4-specific write logic here
        // ...
        return 0;
    }
}
