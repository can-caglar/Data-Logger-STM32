#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <stdint.h>

typedef void(*irq_func)(void);

void gpio_register_interrupt_callback(uint16_t pin, irq_func func);

#endif
