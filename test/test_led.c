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
    rcc_gpiod_clk_enable_Expect();

    GPIO_InitTypeDef my_gpio = 
    {
        .Pin = GPIO_PIN_13,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL
    };
    HAL_GPIO_Init_Expect(GPIOD, &my_gpio);

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