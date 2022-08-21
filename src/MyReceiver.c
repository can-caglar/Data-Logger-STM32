#include "MyReceiver.h"
#include "MyGPIO.h"
#include "MyUSART.h"

static MyGPIO uartGpio;
static USART_TypeDef* rcvrUsart;

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

    rcvrUsart = usart;
    MyUSART_Init(usart, USART_BR_19200);
}

ReceiverEcode_e MyReceiver_Receive(void)
{
    char data = 0;
    MyUSART_Read(rcvrUsart, &data);
    return data;
}