#include "MyTerminalUART.h"
#include "MyUSART.h"
#include "MyCommon.h"
#include "MyGPIO.h"
#include "MyRCC.h"

#define NEW_LINE_DENOTER "> "

static char initialised = 0;
static MyGPIO terminalUartGpio =
{
    .gpio_register = MY_USART_GPIO,
    .mode = GPIO_ALT,
    .alt_func = MY_USART_ALT,
    .output_type = GPIO_PUSH_PULL,
    .pupd = GPIO_PUPD_UP
};

void MyTerminalUART_Init(void)
{
    if (!initialised)
    {
        initialised = 1;
        terminalUartGpio.pin_mask = (MY_USART_RX | MY_USART_TX);

        // enable clocks
        MyRCC_ClockEnable(&(RCC->AHBENR), MY_USART_GPIO_RCC_POS);
        MyRCC_ClockEnable(&(RCC->APB2ENR), MY_USART_UART_RCC_POS);

        MyGPIO_Init(&terminalUartGpio);
        MyUSART_Init(MY_USART, USART_BR_19200);
    }
}

char MyTerminalUART_Read(void)
{
    unsigned char val = 0;
    MyUSART_Read(MY_USART, &val);
    MyTerminalUART_Write(val);
    return val;
}

// Writing carriage return will write newline automatically
// as well as the newline denoter
void MyTerminalUART_Write(char value)
{
    while (MyUSART_Write(MY_USART, value) != ECODE_OK);

    if (value == '\r')  // treat \r uniquely and write more things to terminal.
    {
        MyTerminalUART_Write('\n');
        MyTerminalUART_WriteString(NEW_LINE_DENOTER);
    }
}

void MyTerminalUART_WriteString(const char* str)
{
    while (*str)
    {
        MyTerminalUART_Write(*str);
        str++;
    }
}
