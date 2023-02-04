#ifndef MY_FAKE_FAKE_STM32F4XX_HAL_H
#define MY_FAKE_FAKE_STM32F4XX_HAL_H

#include "stm32f0xx_hal.h"

static uint32_t fakeHalGetTickVal;

void setHalGetTickReturnValue(uint32_t tick)
{
    fakeHalGetTickVal = tick;
}

uint32_t HAL_GetTick(void)
{
    return fakeHalGetTickVal;
}

#endif
