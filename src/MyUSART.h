#ifndef MY_USART_H_INCLUDED
#define MY_USART_H_INCLUDED

#include "MyCommon.h"
#include <stm32f407xx.h>

typedef enum
{
    USART_BR_19200 = 19200,
} USART_BR_e;

Error_Code_e MyUSART_Init(USART_TypeDef* usart, USART_BR_e baud);

#endif
