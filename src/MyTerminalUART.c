#include "MyTerminalUART.h"
#include "MyUSART.h"
#include "MyCommon.h"
#include "MyGPIO.h"
#include "MyRCC.h"

#define NEW_LINE_DENOTER "> "

static char initialised = 0;

void MyTerminalUART_Init(void)
{
    if (!initialised)
    {
        initialised = 1;
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
