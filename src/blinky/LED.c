#include "LED.h"
#include "gpio_hal.h"
#include "rcc_hal.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

/*
ORANGE  PD13
GREEN   PD12
*/

void led_init(void)
{
    rcc_gpiod_clk_enable();

    GPIO_InitTypeDef my_gpio = 
    {
        .Pin = GPIO_PIN_13,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOD, &my_gpio);
}

void led_on(void)
{
    gpio_write(GH_PORT_D, GH_PIN_13, GH_STATE_SET);
}

void led_off(void)
{
    gpio_write(GH_PORT_D, GH_PIN_13, GH_STATE_RESET);
}

void led_toggle(void)
{
    gpio_toggle(GH_PORT_D, GH_PIN_13);
}
