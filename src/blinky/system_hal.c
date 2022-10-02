#include "system_hal.h"
#include "stm32f4xx_hal.h"

void system_init(void)
{
    HAL_Init();
}

uint32_t get_tick(void)
{
    return HAL_GetTick();
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}
