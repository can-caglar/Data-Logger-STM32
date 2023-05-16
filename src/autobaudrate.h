#ifndef AUTO_BAUDRATE_INCLUDED_H
#define AUTO_BAUDRATE_INCLUDED_H

#include <stdint.h>

#define UNDEFINED_BR ((uint32_t)(~0))

void autobaudrate_init(void);
uint32_t autobaudrate_getBr(void);

#endif
