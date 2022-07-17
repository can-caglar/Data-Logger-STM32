#ifndef MYGPIO_H
#define MYGPIO_H

#include "stm32f407xx.h"
#include "MyCommon.h"

typedef enum
{
    GPIO_INPUT = 0UL,
    GPIO_OUTPUT = 1UL,
    GPIO_MODE_MASK = 3UL
} GPIO_Mode_e;

typedef enum
{
    GPIO_LOW = 0,
    GPIO_HIGH = 1,
} GPIO_State_e;

void* MyGPIO_Init(GPIO_TypeDef* gpio_reg, GPIO_Pin_Mask_e pin_mask, GPIO_Mode_e mode);
Error_Code_e MyGPIO_Write(GPIO_TypeDef* gpio_reg, GPIO_Pin_Mask_e pin_mask, GPIO_State_e high);
GPIO_State_e MyGPIO_Read(GPIO_TypeDef* gpio_reg, GPIO_Pin_Number_e pin);

#endif // MYGPIO_H
