#ifndef __PXT_PLATFORM_H
#define __PXT_PLATFORM_H

#include "Image.h"// Found
#include "MultiButton.h"// Found
#include "ZPin.h" /// Found
#include "Timer.h" // Found
#include "ZSPI.h" // Found
#include "ZI2C.h" // Found
#include "ZSingleWireSerial.h" // Proalby in codal-stm32 again.

#include "pinmap.h" // even more inseting.

#define BOOTLOADER_START 0x08000000
#define BOOTLOADER_END 0x08008000

#define SETTINGS_MAGIC_0 0x10476643
#define SETTINGS_MAGIC_1 0x2e9a5026

struct F4_Settings {
    uint32_t magic0;
    uint32_t magic1;
    int *configValues;
    uint32_t hseValue;
    const char *info_uf2;
    const char *manufacturer;
    const char *device;
    uint32_t reserved[9];
};

#define UF2_BINFO ((F4_Settings *)(BOOTLOADER_END - sizeof(F4_Settings)))
#define UF2_INFO_TXT UF2_BINFO->info_uf2
#define USB_HANDOVER 0

#define BOOT_RTC_SIGNATURE 0x71a21877
#define APP_RTC_SIGNATURE 0x24a22d12
#define HF2_RTC_SIGNATURE 0x39a63a78
#define QUICK_BOOT(v)                                                                              \
    do {                                                                                           \
        RTC->BKP0R = v ? APP_RTC_SIGNATURE : HF2_RTC_SIGNATURE;                                    \
    } while (0)

#define PAGE_SIZE 1024 // not really

#define DEV_NUM_PINS 64

#ifdef STM32F1 // never defined.
#define DEV_PWM_PINS 0b111100000011101100001110111000111111001110LL // secret support dectected!
#else
#define DEV_PWM_PINS 0b111100000011100111111110111000111111101111LL
#endif

//               CCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAA
//               fedcba9876543210fedcba9876543210fedcba9876543210
#define DEV_AIN_PINS 0b000011111100000000000000110000000011111111LL

// Codal doesn't yet distinguish between PWM and AIN
#define DEV_ANALOG_PINS (DEV_PWM_PINS | DEV_AIN_PINS)

#define CODAL_PIN ZPin
#define CODAL_TIMER Timer
#define CODAL_SPI ZSPI
#define CODAL_I2C ZI2C
#define CODAL_JACDAC_WIRE_SERIAL codal::ZSingleWireSerial

#define PERF_NOW() (TIM5->CNT)

#define IMAGE_BITS 4

#endif