#include "LED.h"
#include "MyCommon.h"
#include "stm32f3xx_hal.h"

void led_init(void)
{
    LED_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef my_gpio = 
    {
        .Pin = LED_GPIO_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL
    };

    HAL_GPIO_Init(LED_GPIO_PORT, &my_gpio);
}

void led_on(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

void led_off(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
}

void led_toggle(void)
{
    #if 0   // for now, let's not toggle
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    #endif
}
