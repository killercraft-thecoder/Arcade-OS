#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include "stm32f4xx_hal.h"

#define NEOPIXEL_NUM_LEDS 16  // change to your strip length

typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} NeoPixel_Color;

void NeoPixel_Init(TIM_HandleTypeDef *htim);
void NeoPixel_SetPixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void NeoPixel_Show(void);
void NeoPixel_Clear(void);

#endif