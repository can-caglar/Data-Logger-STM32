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
        MyRCC_GPIOClockEnable(&(RCC->AHB1ENR), GPIO_PORT_C_e);
        MyRCC_USARTClockEnable(&(RCC->APB2ENR), USART6_Mask);

        MyGPIO_Init(&terminalUartGpio);
        MyUSART_Init(MY_USART, USART_BR_19200);
    }
}


// TODO: Writing needs to keep trying until it is OK to write
// TODO: maybe make the \r a global?
// TODO: say command doesn't seem to be working
// TODO: help seeAll doesn't work, it's too short but tests are passing?
// TODO: don't make input send a newline if buffer full. just cap the message instead. it's annoying to do something on terminal I didn't intend!
// TODO: do a new line after writing a string to terminal
char MyTerminalUART_Read(void)
{
    unsigned char val = 0;
    MyUSART_Read(MY_USART, &val);
    MyUSART_Write(MY_USART, val);
    return val;
}

void MyTerminalUART_Write(char value)
{
    MyUSART_Write(MY_USART, value);
    if (value == '\r')
    {
        MyTerminalUART_WriteString(NEW_LINE_DENOTER);
    }
}

void MyTerminalUART_WriteString(char* str)
{
    while (*str)
    {
        MyTerminalUART_Write(*str);
        str++;
    }
}
