/**
 * @file rng_utils.h
 * @brief Simple RNG utility functions for STM32F4xx devices with hardware RNG
 *
 * Make sure your MCU has the RNG peripheral.
 */

#ifndef RNG_UTILS_H_
#define RNG_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdint.h>
#include "codal_target_hal.h"

/** 
 * @brief Wheter the RNG unit is ready.
*/
static inline volatile bool ready = false;

/** 
 * @brief RNG State.
*/
enum class RNGState {
    RNG_OK,
    RNG_NOT_READY
}

/**
 * @brief Seed Codal's PRNG
 * @return RNG_OK if sucess RNG_NOT_READY if not Initalized.
 */
static inline int RNG_SeedPRNG() {
   if (ready == false) {
    return RNGState::RNG_NOT_READY;
   }
   target_seed_random(RNG_GetRandomBlocking());
   return RNGState::RNG_OK;
}

/**
 * @brief Initialize the hardware RNG peripheral.
 *
 * Enables clocking to the RNG and starts the generator.
 */
static inline void RNG_Init(void)
{
    ready = true;
    /* Enable RNG clock (on AHB2) */
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;

    /* Optional: reset status flags */
    RNG->SR = 0;

    /* Enable RNG */
    RNG->CR |= RNG_CR_RNGEN;
}

/**
 * @brief Check if a random number is ready.
 * @return 1 if ready, 0 otherwise
 */
static inline uint8_t RNG_IsReady(void)
{
    return (RNG->SR & RNG_SR_DRDY) ? 1U : 0U;
}

/**
 * @brief Get a 32-bit random number (blocking until ready).
 * @return 32-bit random value
 */
static inline uint32_t RNG_GetRandomBlocking(void)
{
    while (!(RNG->SR & RNG_SR_DRDY)) {
        /* wait */
    }
    return RNG->DR;
}

/**
 * @brief Try to get a random number without blocking.
 * @param out Pointer to where the random number will be stored
 * @return 1 if a value was read, 0 if not ready
 */
static inline uint8_t RNG_GetRandomNonBlocking(uint32_t *out)
{
    if (RNG->SR & RNG_SR_DRDY) {
        *out = RNG->DR;
        return 1U;
    }
    return 0U;
}

/**
 * @brief Disable the hardware RNG to save power.
 */
static inline void RNG_DeInit(void)
{
    ready = false;
    RNG->CR &= ~RNG_CR_RNGEN;
    RCC->AHB2ENR &= ~RCC_AHB2ENR_RNGEN;
}

#ifdef __cplusplus
}
#endif

#endif /* RNG_UTILS_H_ */