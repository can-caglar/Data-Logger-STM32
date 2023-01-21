#include "unity.h"
#include "mock_stm32f0xx_hal_gpio.h"
#include "mock_stm32f0xx_hal_rcc.h"
#include "ErrorIndicator.h"

void test_1(void)
{
    GPIO_InitTypeDef my_gpio = 
    {
        .Pin = GPIO_PIN_11,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL
    };

    MY_HAL_RCC_GPIOA_CLK_ENABLE_Expect();
    HAL_GPIO_Init_Expect(GPIOA, &my_gpio);
    HAL_GPIO_WritePin_Expect(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

    ErrorIndicator_Indicate();

    // Indicated error, won't do anything if called again
    ErrorIndicator_Indicate();
    ErrorIndicator_Indicate();
    ErrorIndicator_Indicate();
}