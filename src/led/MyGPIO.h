#ifndef MYGPIO_H
#define MYGPIO_H

#include <stm32f303x8.h>
#include "MyCommon.h"

// TODO, move to common?
typedef enum
{
    GPIO_INPUT = 0UL,
    GPIO_OUTPUT = 1UL,
    GPIO_ALT = 2UL,
    GPIO_MODE_MASK = 3UL
} GPIO_Mode_e;

// TODO, move to common?
typedef enum
{
    GPIO_LOW = 0,
    GPIO_HIGH = 1,
} GPIO_State_e;

typedef enum
{
    GPIO_PUSH_PULL,
    GPIO_OPEN_DRAIN,
} GPIO_OutputType_e;

typedef enum
{
    GPIO_PUPD_NONE = 0x0,
    GPIO_PUPD_UP   = 0x1,
    GPIO_PUPD_DOWN = 0x2,
    GPIO_PUPD_MASK = 0x3
} GPIO_PullUpDown_e;

typedef struct MyGPIO
{
    GPIO_TypeDef* gpio_register;
    GPIO_Pin_Mask_t pin_mask;
    GPIO_Mode_e mode;
    GPIO_ALTF_e alt_func;
    GPIO_OutputType_e output_type;
    GPIO_PullUpDown_e pupd;
} MyGPIO;

Error_Code_e MyGPIO_Init(const MyGPIO* gpio);
Error_Code_e MyGPIO_Write(GPIO_TypeDef* gpio_reg, GPIO_Pin_Mask_t pin_mask, GPIO_State_e high);
GPIO_State_e MyGPIO_Read(GPIO_TypeDef* gpio_reg, GPIO_Pin_Number_e pin);

#endif // MYGPIO_H
