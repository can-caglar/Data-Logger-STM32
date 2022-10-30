// This module shall be removed in favour of using CMSIS RCC functions.
// Made it for exercise, and refactored it down to essentially not needing it anymore.
#ifndef MYRCC_H
#define MYRCC_H

#include <stdint.h>
#include "MyCommon.h"

typedef volatile unsigned int rcc_register;

// Enables clocks
Error_Code_e MyRCC_ClockEnable(rcc_register* reg, unsigned int position);

#endif // MYRCC_H
