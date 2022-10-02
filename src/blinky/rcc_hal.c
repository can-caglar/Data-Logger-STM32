#include "rcc_hal.h"
#include "stm32f4xx_hal_rcc.h"

void rcc_gpiod_clk_enable(void)
{
    __HAL_RCC_GPIOD_CLK_ENABLE();
}

void rcc_gpioa_clk_enable(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
}
