#ifndef MY_COMMON_H_INCLUDED
#define MY_COMMON_H_INCLUDED

#include <stdint.h>

#define FIRST_16_BITS  0x0000FFFF
#define LAST_16_BITS   0xFFFF0000
#define ALL_32_BITS    0xFFFFFFFF

// Error codes
typedef enum
{
    ECODE_OK,
    ECODE_NOT_OUTPUT,
    ECODE_BAD_PARAM,
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

typedef uint16_t GPIO_Pin_Mask_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_0_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_1_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_2_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_3_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_4_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_5_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_6_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_7_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_8_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_9_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_10_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_11_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_12_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_13_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_14_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_15_e;
extern const GPIO_Pin_Mask_e GPIO_PIN_ALL_e;

typedef enum
{
    pin_num_0,
    pin_num_1,
    pin_num_2,
    pin_num_3,
    pin_num_4,
    pin_num_5,
    pin_num_6,
    pin_num_7,
    pin_num_8,
    pin_num_9,
    pin_num_10,
    pin_num_11,
    pin_num_12,
    pin_num_13,
    pin_num_14,
    pin_num_15,
    MAX_GPIO_PINS
} GPIO_Pin_Number_e;

typedef enum
{
    USART1_Mask = (0x1U << 4),
    USART6_Mask = (0x1U << 5),
} USART_Mask_e;

typedef enum
{
    GPIO_ALTF_0 = 0,
    GPIO_ALTF_1,
    GPIO_ALTF_2,
    GPIO_ALTF_3,
    GPIO_ALTF_4,
    GPIO_ALTF_5,
    GPIO_ALTF_6,
    GPIO_ALTF_7,
    GPIO_ALTF_8,
    GPIO_ALTF_9,
    GPIO_ALTF_10,
    GPIO_ALTF_11,
    GPIO_ALTF_12,
    GPIO_ALTF_13,
    GPIO_ALTF_14,
    GPIO_ALTF_15,
    GPIO_MAX_ALT_FUNCTIONS,
} GPIO_ALTF_e;

#define AFR_LOW 0
#define AFR_HIGH 1

typedef volatile uint32_t io_register;

#endif
