#ifndef MY_USART_H_INCLUDED
#define MY_USART_H_INCLUDED

#include "MyCommon.h"
#include <stm32f407xx.h>
#include <stddef.h>

typedef enum
{
    USART_BR_19200 = 19200,
} USART_BR_e;

Error_Code_e MyUSART_Init(USART_TypeDef* usart, const USART_BR_e baud);
Error_Code_e MyUSART_Write(USART_TypeDef* usart, const unsigned char bytes);
Error_Code_e MyUSART_Read(USART_TypeDef* usart, unsigned char* readData);

#endif
