#include "neopixel_patterns.h"
#include "delay.h" // or HAL_Delay

void Pattern_Rainbow(uint16_t wait_ms) {
    for (uint16_t j = 0; j < 256; j++) {
        for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i++) {
            uint8_t r = (i + j) & 255;
            uint8_t g = (i * 2 + j) & 255;
            uint8_t b = (i * 3 + j) & 255;
            NeoPixel_SetPixel(i, r, g, b);
        }
        NeoPixel_Show();
        HAL_Delay(wait_ms);
    }
}

void Pattern_TheaterChase(uint8_t r, uint8_t g, uint8_t b, uint16_t wait_ms) {
    for (int q = 0; q < 3; q++) {
        for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i += 3) {
            NeoPixel_SetPixel(i + q, r, g, b);
        }
        NeoPixel_Show();
        HAL_Delay(wait_ms);
        for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i += 3) {
            NeoPixel_SetPixel(i + q, 0, 0, 0);
        }
    }
}

void Pattern_ColorWipe(uint8_t r, uint8_t g, uint8_t b, uint16_t wait_ms) {
    for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i++) {
        NeoPixel_SetPixel(i, r, g, b);
        NeoPixel_Show();
        HAL_Delay(wait_ms);
    }
}

void Pattern_Custom(void (*pixelFunc)(uint16_t index)) {
    for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i++) {
        pixelFunc(i);
    }
    NeoPixel_Show();
}

void Pattern_Rainbow(uint16_t wait_ms) {
    for (uint16_t j = 0; j < 256; j++) {
        for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i++) {
            uint8_t r = (i + j) & 255;
            uint8_t g = (i * 2 + j) & 255;
            uint8_t b = (i * 3 + j) & 255;
            NeoPixel_SetPixel(i, r, g, b);
        }
        NeoPixel_Show();
        HAL_Delay(wait_ms);
    }
}

void Pattern_TheaterChase(uint8_t r, uint8_t g, uint8_t b, uint16_t wait_ms) {
    for (int q = 0; q < 3; q++) {
        for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i += 3) {
            NeoPixel_SetPixel(i + q, r, g, b);
        }
        NeoPixel_Show();
        HAL_Delay(wait_ms);
        for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i += 3) {
            NeoPixel_SetPixel(i + q, 0, 0, 0);
        }
    }
}

void Pattern_ColorWipe(uint8_t r, uint8_t g, uint8_t b, uint16_t wait_ms) {
    for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i++) {
        NeoPixel_SetPixel(i, r, g, b);
        NeoPixel_Show();
        HAL_Delay(wait_ms);
    }
}

void Pattern_Run(const PatternStep *steps, uint16_t count) {
    uint8_t currentR = 0, currentG = 0, currentB = 0;

    for (uint16_t i = 0; i < count; i++) {
        switch (steps[i].cmd) {
            case PATTERN_CMD_SET_PIXEL:
                NeoPixel_SetPixel(steps[i].a, steps[i].b, steps[i].c, steps[i].param);
                break;

            case PATTERN_CMD_FILL:
                for (uint16_t p = 0; p < NEOPIXEL_NUM_LEDS; p++)
                    NeoPixel_SetPixel(p, steps[i].a, steps[i].b, steps[i].c);
                currentR = steps[i].a;
                currentG = steps[i].b;
                currentB = steps[i].c;
                break;

            case PATTERN_CMD_SHOW:
                NeoPixel_Show();
                break;

            case PATTERN_CMD_DELAY:
                HAL_Delay(steps[i].param);
                break;

            case PATTERN_CMD_FADE: {
                static const uint16_t fadeTimes[] = {100, 200, 300, 400, 500};
                uint16_t totalTime = fadeTimes[steps[i].a % 5];
                uint16_t stepDelay = totalTime / 50;

                int16_t dr = (int16_t)steps[i].b - currentR;
                int16_t dg = (int16_t)steps[i].c - currentG;
                int16_t db = (int16_t)steps[i].param - currentB;

                for (uint8_t s = 1; s <= 50; s++) {
                    uint8_t r = currentR + (dr * s) / 50;
                    uint8_t g = currentG + (dg * s) / 50;
                    uint8_t b = currentB + (db * s) / 50;

                    for (uint16_t p = 0; p < NEOPIXEL_NUM_LEDS; p++)
                        NeoPixel_SetPixel(p, r, g, b);

                    NeoPixel_Show();
                    HAL_Delay(stepDelay);
                }

                currentR = steps[i].b;
                currentG = steps[i].c;
                currentB = steps[i].param;
                break;
            }
        }
    }
}