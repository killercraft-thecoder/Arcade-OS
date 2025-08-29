#include "neopixel.h"

// WS2812 timing in nanoseconds
#define T0H  350
#define T1H  900
#define PERIOD 1250

// LED data
static uint16_t pwmBuffer[NEOPIXEL_NUM_LEDS * 24 + 50];
static NeoPixel_Color pixels[NEOPIXEL_NUM_LEDS];

static TIM_HandleTypeDef htim;
static DMA_HandleTypeDef hdma_tim;

static void NeoPixel_UpdateBuffer(void) {
    uint32_t indx = 0;
    for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i++) {
        uint8_t colors[3] = { pixels[i].green, pixels[i].red, pixels[i].blue };
        for (uint8_t c = 0; c < 3; c++) {
            for (int8_t bit = 7; bit >= 0; bit--) {
                if (colors[c] & (1 << bit)) {
                    pwmBuffer[indx++] = (T1H * htim.Init.Period) / PERIOD;
                } else {
                    pwmBuffer[indx++] = (T0H * htim.Init.Period) / PERIOD;
                }
            }
        }
    }
    for (uint8_t i = 0; i < 50; i++) {
        pwmBuffer[indx++] = 0;
    }
}

static void NeoPixel_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_8; // TIM1_CH1 on PA8
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void NeoPixel_TIM_Init(void) {
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    htim.Instance = TIM1;
    htim.Init.Prescaler = (SystemCoreClock / 800000) - 1; // 800kHz
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = 100 - 1; // Adjust for duty cycle resolution
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim);

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_1);

    // DMA config
    hdma_tim.Instance = DMA2_Stream5;
    hdma_tim.Init.Channel = DMA_CHANNEL_6;
    hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_tim.Init.Mode = DMA_NORMAL;
    hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_tim.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_tim);

    __HAL_LINKDMA(&htim, hdma[TIM_DMA_ID_CC1], hdma_tim);
}

void NeoPixel_Init(void) {
    NeoPixel_GPIO_Init();
    NeoPixel_TIM_Init();
    NeoPixel_Clear();
    NeoPixel_UpdateBuffer();
}

int NeoPixel_SetPixel(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    if (n >= NEOPIXEL_NUM_LEDS) {
        return -1; // invalid LED index
    }
    pixels[n].red = r;
    pixels[n].green = g;
    pixels[n].blue = b;
    return 0;
}

void NeoPixel_Show(void) {
    NeoPixel_UpdateBuffer();
    HAL_TIM_PWM_Start_DMA(&htim, TIM_CHANNEL_1, (uint32_t *)pwmBuffer, sizeof(pwmBuffer)/sizeof(uint16_t));
    while (HAL_DMA_GetState(&hdma_tim) != HAL_DMA_STATE_READY) {}
}

void NeoPixel_Clear(void) {
    for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS; i++) {
        pixels[i].red = 0;
        pixels[i].green = 0;
        pixels[i].blue = 0;
    }
}