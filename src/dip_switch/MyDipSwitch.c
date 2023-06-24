#include "MyDipSwitch.h"
#include "stm32f3xx_hal.h"
#include "MyCommon.h"

void MyDIP_Init()
{
    GPIO_InitTypeDef dipswitch =
    {
        .Pin = GPIO_PIN_DIP_1 | GPIO_PIN_DIP_2 | GPIO_PIN_DIP_3 | GPIO_PIN_DIP_4,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
    };
    CLK_ENABLE_DIP();
    HAL_GPIO_Init(GPIO_PORT_DIP, &dipswitch);
}

uint8_t MyDIP_Read(void)
{
    uint8_t ret = 0;
    ret |= HAL_GPIO_ReadPin(GPIO_PORT_DIP, GPIO_PIN_DIP_1) << 0;
    ret |= HAL_GPIO_ReadPin(GPIO_PORT_DIP, GPIO_PIN_DIP_2) << 1;
    ret |= HAL_GPIO_ReadPin(GPIO_PORT_DIP, GPIO_PIN_DIP_3) << 2;
    ret |= HAL_GPIO_ReadPin(GPIO_PORT_DIP, GPIO_PIN_DIP_4) << 3;
    return ret;
}
