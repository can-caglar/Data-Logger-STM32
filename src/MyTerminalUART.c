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

char MyTerminalUART_Read(void)
{
    unsigned char val = 0;
    MyUSART_Read(MY_USART, &val);
    MyTerminalUART_Write(val);
    return val;
}

void MyTerminalUART_Write(char value)
{
    while (MyUSART_Write(MY_USART, value) != ECODE_OK);

    if (value == '\r')  // treat \r uniquely. write a bunch more stuff than \r
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
