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
    GPIOPort_e port = GH_PORT_D;
    GH_Init_s gpio;

    gpio.mode = GH_MODE_OUTPUT_PP;
    gpio.pin = GH_PIN_13;
    gpio.pull = GH_PULL_NONE;

    rcc_gpiod_clk_enable();
    gpio_init(port, &gpio);
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
