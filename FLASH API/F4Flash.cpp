#include "F4Flash.h"
#include <stm32f4xx.h> // Or your CMSIS header for STM32F4

namespace codal
{
    inline int F4Flash::pageSize(uintptr_t address)
    {
        address |= 0x08000000;
        if (address < 0x08010000)
            return 16 * 1024;
        if (address < 0x08020000)
            return 64 * 1024;
        if (address < 0x08100000)
            return 128 * 1024;
        target_panic(DEVICE_FLASH_ERROR);
        return 0;
    }

    inline int F4Flash::totalSize()
    {
        return *((uint16_t *)0x1FFF7A22) * 1024;
    }

    inline int F4Flash::erasePage(uintptr_t address)
    {
        return ZFlash::erasePage(address);
    }

    inline int F4Flash::writeBytes(uintptr_t dst, const void *src, uint32_t len)
    {
        return ZFlash::writeBytes(dst, src, len)
    }
}
