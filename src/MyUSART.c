// Module to read and write 1 byte on UART

#include <math.h>
#include "MyUSART.h"

#define USART_CR2_1STOPBIT    (USART_CR2_STOP_0 | USART_CR2_STOP_1)
#define MAX_FRACTION    16  // OVER8 = 0 always for this module

// Static declarations
static io_register calculateBRR(unsigned long fclk, USART_BR_e baud);
unsigned char isNullPtr(void* ptr);

// Public functions

Error_Code_e MyUSART_Init(USART_TypeDef* USART, const USART_BR_e baud)
{
    USART->CR1 |= USART_CR1_UE;    // enable USART
    USART->CR1 &= ~(USART_CR1_M);  // Word length = 8
    USART->CR2 &= ~(USART_CR2_1STOPBIT);
    USART->BRR = calculateBRR(SystemCoreClock, baud);
    USART->CR1 |= USART_CR1_TE;    // send idle frame (enable transmitter)
    USART->CR1 |= USART_CR1_RE;    // enable receiver
    return ECODE_OK;
}

Error_Code_e MyUSART_Write(USART_TypeDef* usart, const unsigned char byte)
{
    if (usart->ISR & USART_ISR_TXE)
    {
        usart->TDR = byte;
        return ECODE_OK;
    }
    return ECODE_NOT_READY;
}

// todo: MyUSART_Read shall just read (and pop) from a circular buffer
// we shall initialise an interrupt to fire whenever we have new data and
// push it to the circular buffer
Error_Code_e MyUSART_Read(USART_TypeDef* usart, unsigned char* outchar)
{
    if (isNullPtr(outchar))
    {
        return ECODE_BAD_PARAM;
    }
    if (usart->ISR & USART_ISR_RXNE)
    {
        *outchar = usart->RDR;
        return ECODE_OK;
    }
    return ECODE_NOT_READY;
}

/****************** Static functions ********************/

static io_register calculateBRR(unsigned long fclk, USART_BR_e baud)
{
    // equation from RM0090 page 981
    float usartdiv = (float)(fclk) / (16UL * (unsigned long)baud);
    io_register div_mantissa = floor(usartdiv);
    float fraction = usartdiv - (int)(usartdiv);
    io_register div_fraction = round(fraction * MAX_FRACTION);

    io_register ret = (div_mantissa << 4) | div_fraction;

    return ret;
}

unsigned char isNullPtr(void* ptr)
{
    return (ptr == 0);
}
