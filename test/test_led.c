#include "unity.h"
#include "LED.h"
#include "mock_MyInterrupts.h" // rm me
#include "mock_rcc_hal.h"  // rm me
#include "unity_helper.h"  // update me?
#include "mock_stm32f4xx_hal_gpio.h"
#include "mock_stm32f4xx_hal_rcc_ex.h"

void test_LedInitWillInitialiseCorrectPort(void)
{
    MY_HAL_RCC_GPIOD_CLK_ENABLE_Expect();

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
    HAL_GPIO_WritePin_Expect(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
    led_on();
}

void test_LedOffTest(void)
{
    HAL_GPIO_WritePin_Expect(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    led_off();
}

void test_LedToggle(void)
{
    HAL_GPIO_TogglePin_Expect(GPIOD, GPIO_PIN_13);
    led_toggle();
}

/*
- [x] Led can be turned on
- [x] Led can be turned off
- [ ] Make remove the RCC adapter layer.
*/