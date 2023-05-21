#ifndef MY_FAKE_FAKE_STM32F4XX_HAL_H
#define MY_FAKE_FAKE_STM32F4XX_HAL_H

// TODO: rename this module it's no longer for stm32f0

#include "stm32f3xx_hal.h"

void fake_halTick_reset(void);
void fake_halTick_setTickValue(uint32_t tick);
void fake_halTick_enableAutoIncrement(uint32_t valueToIncrement);

#endif
