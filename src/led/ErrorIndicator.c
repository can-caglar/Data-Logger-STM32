#include "ErrorIndicator.h"
#include "MyCommon.h"
#include "stm32f0xx_hal.h"

static uint8_t errorOccured = 0;

void ErrorIndicator_Indicate(void)
{
    if (!errorOccured)
    {
        errorOccured = 1;   // indicate error once
        LED_ERROR_CLK_ENABLE();
        GPIO_InitTypeDef my_gpio = 
        {
            .Pin = LED_ERROR_PIN,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL
        };
        HAL_GPIO_Init(LED_ERROR_PORT, &my_gpio);
        HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_SET);
    }
}
