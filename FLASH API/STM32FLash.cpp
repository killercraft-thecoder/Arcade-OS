#include "C:\Users\Lando\ARCADE OS\core API\pxt.h"
#include "Flash.h"
#include "stm32.h"
#include "F4Flash.h"

// #define LOG DMESG
#define LOG NOLOG

namespace codal
{
    static void waitForLast()
    {
        while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY)
            ;
    }

    static void unlock()
    {
        FLASH->CR |= FLASH_CR_LOCK;
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }

    static void lock()
    {
        FLASH->CR |= FLASH_CR_LOCK;
    }

    int ZFlash::pageSize(uintptr_t address)
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

    int ZFlash::totalSize()
    {
        return *((uint16_t *)0x1FFF7A22) * 1024;
    }

    int8 ZFlash::erasePage(uintptr_t address)
    {
        waitForLast();
        unlock();

        address |= 0x08000000;
        uintptr_t ptr = 0x08000000;
        int sectNum = 0;
        while (1)
        {
            ptr += pageSize(ptr);
            if (ptr > address)
                break;
            sectNum++;
        }

        FLASH->CR = FLASH_CR_PSIZE_1 | (sectNum << FLASH_CR_SNB_Pos) | FLASH_CR_SER;
        FLASH->CR |= FLASH_CR_STRT;

        waitForLast();

        FLASH->CR = FLASH_CR_PSIZE_1;
        lock();

        // cache flushing only required after erase, not programming (3.5.4)
        __HAL_FLASH_DATA_CACHE_DISABLE();
        __HAL_FLASH_DATA_CACHE_RESET();
        __HAL_FLASH_DATA_CACHE_ENABLE();

        // we skip instruction cache, as we're not expecting to erase that

        return 0;
    }

    int8 ZFlash::writeBytes(uintptr_t dst, const void *src, uint32_t len)
    {
        LOG("WR flash at %p len=%d", (void *)dst, len);

        if ((dst & 3) || ((uintptr_t)src & 3) || (len & 3))
            return -1;

        for (unsigned i = 0; i < len; ++i)
            if (((uint8_t *)dst)[i] != 0xff && ((uint8_t *)src)[i] != 0xff)
                return -3;

        waitForLast();
        unlock();

        dst |= 0x08000000;

        FLASH->CR = FLASH_CR_PSIZE_1 | FLASH_CR_PG;

        volatile uint32_t *sp = (uint32_t *)src;
        volatile uint32_t *dp = (uint32_t *)dst;
        len >>= 2;

        while (len-- > 0)
        {
            uint32_t v = *sp++;
            if (v != 0xffffffff)
            {
                *dp++ = v;
                waitForLast();
            }
            else
            {
                dp++;
            }
        }

        FLASH->CR = FLASH_CR_PSIZE_1;
        lock();

        LOG("WR flash OK");

        return 0;
    }

    bool is_bootloader_region(uintptr_t address, size_t size) {
        // Protect 0x0000–0x2000 (8KB)
        return (address < 0x2000) || (address + size > 0x0000 && address + size <= 0x2000);
    }

    // Before any flash write/erase, check:
    // if (is_bootloader_region(address, size)) return FLASH_PROTECT_ERROR;
}
