#include "LED.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_rcc.h"

/*
ORANGE  PD13
GREEN   PD12
*/

#define LED_GPIO_PORT   GPIOB
#define LED_GPIO_PIN    GPIO_PIN_3

void led_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef my_gpio = 
    {
        .Pin = GPIO_PIN_3,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL
    };

    HAL_GPIO_Init(LED_GPIO_PORT, &my_gpio);
}

void led_on(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, GPIO_PIN_3, GPIO_PIN_SET);
}

void led_off(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, GPIO_PIN_3, GPIO_PIN_RESET);
}

void led_toggle(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_PORT, GPIO_PIN_3);
}
