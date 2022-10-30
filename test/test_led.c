#include "unity.h"
#include "LED.h"
#include "mock_gpio_interrupts.h"
#include "unity_helper.h"  // TODO: update me?
#include "mock_stm32f0xx_hal_gpio.h"
#include "mock_stm32f0xx_hal_rcc.h"

void test_LedInitWillInitialiseCorrectPort(void)
{
    MY_HAL_RCC_GPIOB_CLK_ENABLE_Expect();

    GPIO_InitTypeDef my_gpio = 
    {
        .Pin = GPIO_PIN_3,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL
    };

    HAL_GPIO_Init_Expect(GPIOB, &my_gpio);

    led_init();
}

void test_LedOnTest(void)
{
    HAL_GPIO_WritePin_Expect(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
    led_on();
}

void test_LedOffTest(void)
{
    HAL_GPIO_WritePin_Expect(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
    led_off();
}

void test_LedToggle(void)
{
    HAL_GPIO_TogglePin_Expect(GPIOB, GPIO_PIN_3);
    led_toggle();
}

/*
- [x] Led can be turned on
- [x] Led can be turned off
- [ ] Make remove the RCC adapter layer.
*/