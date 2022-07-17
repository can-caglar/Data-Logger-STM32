#ifndef MYRCC_H
#define MYRCC_H

#include <stdint.h>
#include "MyCommon.h"

typedef volatile unsigned int rcc_register;

// Enables clocks
Error_Code_e MyRCC_GPIOClockEnable(rcc_register* reg, GPIO_Port_e gpio_port);
Error_Code_e MyRCC_USARTClockEnable(rcc_register* reg, USART_Mask_e usart_periph);

#endif // MYRCC_H
