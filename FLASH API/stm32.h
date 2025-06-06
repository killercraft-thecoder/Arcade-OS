#ifndef STM32_FLASH_H
#define STM32_FLASH_H

#include "Flash.h"
#include <cstdint>

namespace codal {
    /**
     * Abstract base class for STM32 flash memory operations.
     * Implementations should provide chip-specific logic for page size, erase, and write.
     */
    class STM32Flash : public Flash {
    public:
        virtual int pageSize(uintptr_t address) override = 0;
        virtual int totalSize() override = 0;
        virtual int erasePage(uintptr_t address) override = 0;
        virtual int writeBytes(uintptr_t dst, const void *src, uint32_t len) override = 0;
        virtual ~STM32Flash() {}
    };
}

#endif
