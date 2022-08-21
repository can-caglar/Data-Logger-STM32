#include "MyReceiver.h"
#include "MyGPIO.h"
#include "MyUSART.h"
#include <string.h>

static MyGPIO uartGpio;
static USART_TypeDef* rcvrUsart;

#define MAX_CHARACTERS 11

static struct
{
    char character[MAX_CHARACTERS];
    unsigned int index;
} inbuf;

void MyReceiver_Init(USART_TypeDef* usart,
                     GPIO_TypeDef* gpio, 
                     GPIO_ALTF_e altf,
                     GPIO_Pin_Mask_t rx,
                     GPIO_Pin_Mask_t tx)
{
    uartGpio.gpio_register = gpio;
    uartGpio.pin_mask = (rx | tx);
    uartGpio.mode = GPIO_ALT;
    uartGpio.alt_func = altf;
    uartGpio.output_type = GPIO_PUSH_PULL;
    uartGpio.pupd = GPIO_PUPD_UP;
    MyGPIO_Init(&uartGpio);
    MyUSART_Init(usart, USART_BR_19200);

    rcvrUsart = usart;
    memset(&inbuf, 0, sizeof(inbuf));
}

ReceiverEcode_e MyReceiver_Receive(void)
{
    char data = 0;
    if (MyUSART_Read(rcvrUsart, &data) == ECODE_OK)
    {
        if (inbuf.index < (MAX_CHARACTERS - 1))
        {
            if (data != '\n')
            {
                inbuf.character[inbuf.index++] = data;
                return RCVR_RECEIVED;
            }
        }
        return RCVR_DONE;
        
    }
    return RCVR_NOT_RECEIVED;
}

char* MyReceiver_GetBuffer(void)
{
    return inbuf.character;
}