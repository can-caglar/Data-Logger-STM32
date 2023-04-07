#ifndef FAKE_LED_H
#define FAKE_LED_H

#include "LED.h"
#include <stdint.h>

uint8_t fakeLed_isOn(void);
uint8_t fakeLed_reset(void);

#endif