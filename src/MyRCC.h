#ifndef MYRCC_H
#define MYRCC_H

#include <stdint.h>
#include "MyCommon.h"

typedef volatile unsigned int rcc_register;

// Enables clocks
Error_Code_e MyRCC_GPIOClockEnable(rcc_register* reg, int gpio_port);

#endif // MYRCC_H
