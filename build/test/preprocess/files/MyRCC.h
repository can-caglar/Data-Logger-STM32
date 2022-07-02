#include "src/MyCommon.h"








typedef volatile unsigned int rcc_register;





Error_Code_e MyRCC_GPIOClockEnable(rcc_register* reg, int gpio_port);
