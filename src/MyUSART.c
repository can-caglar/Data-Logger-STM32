#include <math.h>
#include "MyUSART.h"

#define USART_CR2_1STOPBIT    (USART_CR2_STOP_0 | USART_CR2_STOP_1)
#define MAX_FRACTION    16  // OVER8 = 0 always for this module

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

Error_Code_e MyUSART_Init(USART_TypeDef* USART, USART_BR_e baud)
{
    USART->CR1 |= USART_CR1_UE;    // enable USART
    USART->CR1 &= ~(USART_CR1_M);  // Word length = 8
    USART->CR2 &= ~(USART_CR2_1STOPBIT);
    USART->BRR = calculateBRR(SystemCoreClock, baud);
    USART->CR1 |= USART_CR1_TE;    // send idle frame
    return ECODE_OK;
}
