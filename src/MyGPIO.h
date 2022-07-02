#ifndef MYGPIO_H
#define MYGPIO_H

#include "stm32f407xx.h"
#include "MyCommon.h"

typedef enum
{
    GPIO_INPUT,
    GPIO_OUTPUT
} GPIO_Mode_e;

void* MyGPIO_Init(GPIO_TypeDef* gpio_reg, GPIO_Pin_e pin_mask, GPIO_Mode_e mode);
Error_Code_e MyGPIO_Write(GPIO_TypeDef* gpio_reg, GPIO_Pin_e pin_mask, int high);

#endif // MYGPIO_H
