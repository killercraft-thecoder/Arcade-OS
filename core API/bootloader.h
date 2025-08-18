#pragma once
#include "stm32f4xx.h"

namespace core {
#if defined(__GNUC__)
  #define NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
  #define NORETURN __declspec(noreturn)
#else
  #define NORETURN [[noreturn]]
#endif

using entry_t = void (*)();

static inline void jump_to_vector_table(uint32_t vector_base)
{
    __disable_irq();

    // Stop SysTick
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    // Disable and clear all NVIC interrupts
    for (uint32_t i = 0; i < 8; ++i) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    // Optionally return clocks to a simple state (HSI on, PLL/HSE off)
    RCC->CR   |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0) {}
    RCC->CFGR &= ~RCC_CFGR_SW;            // select HSI
    while ((RCC->CFGR & RCC_CFGR_SWS) != 0) {}
    RCC->CR   &= ~(RCC_CR_PLLON | RCC_CR_HSEON);

    // Point vector table at the target image
    SCB->VTOR = vector_base;

    // Load target SP and entry PC from its vector table
    uint32_t const new_msp = *reinterpret_cast<uint32_t const *>(vector_base + 0);
    uint32_t const new_pc  = *reinterpret_cast<uint32_t const *>(vector_base + 4);

    // Sanity checks (optional in release)
    // - new_msp should be in SRAM range
    // - bit0 of new_pc must be 1 (Thumb)
    // if ((new_pc & 1u) == 0) while(1) {}

    __set_MSP(new_msp);
    reinterpret_cast<entry_t>(new_pc)();

    // no return
}
NORETURN static inline void go_to_bootloader()
{
    // For STM32F4, FLASH_BASE is typically 0x08000000 (defined by vendor headers).
    jump_to_vector_table(FLASH_BASE);
    __builtin_unreachable();
}

} // namespace core
