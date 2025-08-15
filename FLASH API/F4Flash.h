#ifndef STM32F4_FLASH_H
#define STM32F4_FLASH_H

#include "stm32flash.h"

namespace codal {
    /**
     * STM32F4-specific flash implementation.
     */
    class F4Flash : public STM32Flash {
    public:
        int pageSize(uintptr_t address) override;
        int totalSize() override;
        int erasePage(uintptr_t address) override;
        int writeBytes(uintptr_t dst, const void *src, uint32_t len) override;
    };
}

#endif
