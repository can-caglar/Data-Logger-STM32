// Module to read and write 1 byte on UART

#include <math.h>
#include "MyUSART.h"
#include "stm32f0xx_hal.h"
#include "main.h"

extern UART_HandleTypeDef huart1;

// Public functions

Error_Code_e MyUSART_Init(USART_TypeDef* USART, const USART_BR_e baud)
{
    CubeMX_SystemInit(CMX_UART_POLL);
    return ECODE_OK;
}

Error_Code_e MyUSART_Write(USART_TypeDef* usart, const unsigned char byte)
{
    if (HAL_UART_Transmit(&huart1, (uint8_t*)&byte, 1, 500) == HAL_OK)
    {
        return ECODE_OK;
    }
    else
    {
        return ECODE_NOT_READY;
    }
}

// todo: MyUSART_Read shall just read (and pop) from a circular buffer
// we shall initialise an interrupt to fire whenever we have new data and
// push it to the circular buffer
Error_Code_e MyUSART_Read(USART_TypeDef* usart, unsigned char* outchar)
{
    if (HAL_UART_Receive(&huart1, outchar, 1, 500) == HAL_OK)
    {
        return ECODE_OK;
    }
    else
    {
        return ECODE_NOT_READY;
    }
}
