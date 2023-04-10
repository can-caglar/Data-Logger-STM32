#include "fake_stm32f0xx_hal.h"
#include <string.h>

static struct
{
    uint32_t fakeHalGetTickVal;
    uint32_t incrementVal;
} internalState;

void fake_halTick_reset(void)
{
    memset(&internalState, 0, sizeof(internalState));
}

// Will increment HAL_GetTick() value by this amount each time
// It is called. Default is 0, i.e. it will always return the same
// each call. This is to emulate the passage of time.
void fake_halTick_enableAutoIncrement(uint32_t valueToIncrement)
{
    internalState.incrementVal = valueToIncrement;
}

void fake_halTick_setTickValue(uint32_t tick)
{
    internalState.fakeHalGetTickVal = tick;
}

uint32_t HAL_GetTick(void)
{
    uint32_t temp = internalState.fakeHalGetTickVal;
    internalState.fakeHalGetTickVal += internalState.incrementVal;
    return temp;
}
