#include "MyReceiver.h"
#include "MyGPIO.h"

static MyGPIO uartGpio;

void MyReceiver_Init(GPIO_TypeDef* gpio, 
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
}