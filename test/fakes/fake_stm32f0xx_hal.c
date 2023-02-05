#ifndef MY_FAKE_FAKE_STM32F4XX_HAL_H
#define MY_FAKE_FAKE_STM32F4XX_HAL_H

#include "stm32f0xx_hal.h"

static uint32_t fakeHalGetTickVal;
static uint32_t incrementVal;

// Will increment HAL_GetTick() value by this amount each time
// It is called. Default is 0, i.e. it will always return the same
// each call. This is to emulate the passage of time.
void fake_halTick_enableAutoIncrement(uint32_t valueToIncrement)
{
    incrementVal = valueToIncrement;
}

void fake_halTick_setTickValue(uint32_t tick)
{
    fakeHalGetTickVal = tick;
}

uint32_t HAL_GetTick(void)
{
    uint32_t temp = fakeHalGetTickVal;
    fakeHalGetTickVal += incrementVal;
    return temp;
}

#endif
