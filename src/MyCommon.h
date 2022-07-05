#ifndef MY_COMMON_H_INCLUDED
#define MY_COMMON_H_INCLUDED

#include <stdint.h>

// Error codes
typedef enum
{
    ECODE_OK,
    ECODE_NOT_OUTPUT
} Error_Code_e;

// GPIO ports and pins
typedef enum
{
    GPIO_PORT_A_e,
    GPIO_PORT_B_e,
    GPIO_PORT_C_e,
    GPIO_PORT_D_e,
    GPIO_PORT_E_e,
    GPIO_PORT_F_e,
    GPIO_PORT_G_e,
    GPIO_PORT_H_e,
    GPIO_PORT_I_e,
    GPIO_PORT_COUNT_e,
} GPIO_Port_e;

typedef enum
{
    GPIO_PIN_0_e = (0x1UL << 0),
    GPIO_PIN_1_e = (0x1UL << 1),
    GPIO_PIN_2_e = (0x1UL << 2),
    GPIO_PIN_3_e = (0x1UL << 3),
    GPIO_PIN_4_e = (0x1UL << 4),
    GPIO_PIN_5_e = (0x1UL << 5),
    GPIO_PIN_6_e = (0x1UL << 6),
    GPIO_PIN_7_e = (0x1UL << 7),
    GPIO_PIN_8_e = (0x1UL << 8),
    GPIO_PIN_9_e = (0x1UL << 9),
    GPIO_PIN_10_e = (0x1UL << 10),
    GPIO_PIN_11_e = (0x1UL << 11),
    GPIO_PIN_12_e = (0x1UL << 12),
    GPIO_PIN_13_e = (0x1UL << 13),
    GPIO_PIN_14_e = (0x1UL << 14),
    GPIO_PIN_15_e = (0x1UL << 15),
    GPIO_PIN_ALL_e = (0xFFFFUL),
} GPIO_Pin_e;

#define MAX_GPIO_PINS 16

typedef volatile uint32_t io_register;

#endif