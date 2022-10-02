#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <stdint.h>

typedef enum GPIOPort_e
{
    GH_PORT_A,
    GH_PORT_B,
    GH_PORT_C,
    GH_PORT_D,
    GH_PORT_E,
    GH_PORT_F,
    GH_PORT_COUNT,
} GPIOPort_e;

typedef enum GPIOPin_e
{
    GH_PIN_0,
    GH_PIN_1,
    GH_PIN_2,
    GH_PIN_3,
    GH_PIN_4,
    GH_PIN_5,
    GH_PIN_6,
    GH_PIN_7,
    GH_PIN_8,
    GH_PIN_9,
    GH_PIN_10,
    GH_PIN_11,
    GH_PIN_12,
    GH_PIN_13,
    GH_PIN_14,
    GH_PIN_15,
    GH_PIN_COUNT
} GPIOPin_e;

typedef enum GPIOMode_e
{
    GH_MODE_INPUT,
    GH_MODE_OUTPUT_PP,
    GH_MODE_OUTPUT_OD,
    GH_MODE_IT_RISING_FALLING,
    GH_MODE_IT_RISING,
    #if 0
    GH_MODE_AF_PP,
    GH_MODE_AF_OD,
    GH_MODE_ANALOG,
    GH_MODE_IT_FALLING,
    GH_MODE_EVT_RISING,
    GH_MODE_EVT_FALLING,
    GH_MODE_EVT_RISING_FALLING,
    #endif
    GH_MODE_COUNT,
} GPIOMode_e;

typedef enum GPIOPull_e
{
    GH_PULL_NONE,
    GH_PULL_UP,
    GH_PULL_DOWN,
    GH_PULL_COUNT,
} GPIOPull_e;

typedef enum GPIOState_e
{
    GH_STATE_RESET,
    GH_STATE_SET,
    GH_STATE_COUNT
} GPIOState_e;

typedef struct GH_Init_s
{
    GPIOPin_e pin;
    GPIOMode_e mode;
    GPIOPull_e pull;
} GH_Init_s;

typedef void(*irq_func)(void);

void gpio_init(GPIOPort_e port, GH_Init_s* gpio);
void gpio_write(GPIOPort_e port, GPIOPin_e pin, GPIOState_e state);
void gpio_toggle(GPIOPort_e port, GPIOPin_e pin);
GPIOState_e gpio_read(GPIOPort_e port, GPIOPin_e pin);
void gpio_register_interrupt_callback(GPIOPin_e pin, irq_func func);

#endif
