/* MyReceiver.c
 *
 * Responsible for storing, updating and clearing
 * a command received from USART.
 * 
 * It will echo the command back on the USART.
*/
#include "MyReceiver.h"
#include "MyGPIO.h"
#include "MyUSART.h"
#include <string.h>

static MyGPIO uartGpio;
static USART_TypeDef* rcvrUsart;
static Error_Code_e receiveState = RCVR_NOT_RECEIVED;
static void resetGlobals(void);

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
    resetGlobals();

    uartGpio.gpio_register = gpio;
    uartGpio.pin_mask = (rx | tx);
    uartGpio.mode = GPIO_ALT;
    uartGpio.alt_func = altf;
    uartGpio.output_type = GPIO_PUSH_PULL;
    uartGpio.pupd = GPIO_PUPD_UP;
    MyGPIO_Init(&uartGpio);
    MyUSART_Init(usart, USART_BR_19200);

    rcvrUsart = usart;
}

ReceiverEcode_e MyReceiver_Receive(void)
{
    char data = 0;
    if (MyUSART_Read(rcvrUsart, &data) == ECODE_OK)
    {
        if (inbuf.index < (MAX_CHARACTERS - 1)
            && data != '\n')
        {
            inbuf.character[inbuf.index++] = data;
            receiveState = RCVR_RECEIVED;
        }
        else
        {
            receiveState = RCVR_DONE;
        }
    }
    MyUSART_Write(rcvrUsart, data);
    return receiveState;
}

char* MyReceiver_GetBuffer(void)
{
    return inbuf.character;
}

void MyReceiver_Clear(void)
{
    resetGlobals();
}

/***************************** Private *******************/

static void resetGlobals(void)
{
    receiveState = RCVR_NOT_RECEIVED;
    memset(&inbuf, 0, sizeof(inbuf));
}