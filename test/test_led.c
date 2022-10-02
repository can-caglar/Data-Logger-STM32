#include "unity.h"
#include "LED.h"
#include "mock_gpio_hal.h"
#include "mock_rcc_hal.h"
#include "unity_helper.h"
#include "mock_stm32f4xx_hal_gpio.h"

#if 0
void setUp(void)
{

}

void tearDown(void)
{

}
#endif

void test_LedInitWillInitialiseCorrectPort(void)
{
    GPIOPort_e port = GH_PORT_D;
    GH_Init_s gpio;

    gpio.mode = GH_MODE_OUTPUT_PP;
    gpio.pull = GH_PULL_NONE;
    gpio.pin = GH_PIN_13;

    rcc_gpiod_clk_enable_Expect();

    gpio_init_Expect(GH_PORT_D, &gpio);

    led_init();
}

void test_LedOnTest(void)
{
    gpio_write_Expect(GH_PORT_D, GH_PIN_13, GH_STATE_SET);
    led_on();
}

void test_LedOffTest(void)
{
    gpio_write_Expect(GH_PORT_D, GH_PIN_13, GH_STATE_RESET);
    led_off();
}

void test_LedToggle(void)
{
    gpio_toggle_Expect(GH_PORT_D, GH_PIN_13);
    led_toggle();
}

/*
- [x] Led can be turned on
- [x] Led can be turned off
*/