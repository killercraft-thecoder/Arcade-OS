#ifndef NEOPIXEL_PATTERNS_H
#define NEOPIXEL_PATTERNS_H

#include <stdint.h>
#include "neopixel.h"

#ifdef __cplusplus
extern "C" {
#endif

// Example built‑in patterns
void Pattern_Rainbow(uint16_t wait_ms);
void Pattern_TheaterChase(uint8_t r, uint8_t g, uint8_t b, uint16_t wait_ms);
void Pattern_ColorWipe(uint8_t r, uint8_t g, uint8_t b, uint16_t wait_ms);

typedef enum {
    PATTERN_CMD_SET_PIXEL, // set one pixel to a color
    PATTERN_CMD_FILL,      // fill all pixels with a color
    PATTERN_CMD_SHOW,      // push data to LEDs
    PATTERN_CMD_DELAY,     // wait some ms
    PATTERN_CMD_FADE       // fade smoothly over 50 steps to a new color
} PatternCommandType;

typedef struct {
    PatternCommandType cmd; // command type
    uint8_t a;              // arg1 (meaning depends on cmd)
    uint8_t b;              // arg2
    uint8_t c;              // arg3
    uint16_t param;         // extra parameter
} PatternStep;

// Run a pattern from an array of steps
void Pattern_Run(const PatternStep *steps, uint16_t count);




#ifdef __cplusplus
}
#endif

#endif // NEOPIXEL_PATTERNS_H