#include "unity.h"
#include "MyReceiver.h"
#include "MyCommon.h"
#include "mock_MyUSART.h"
#include "mock_MyGPIO.h"
#include "unity_helper.h"

void setUp(void)
{

}

void tearDown(void)
{

}

void test_MyReceiverInitWillInitialiseUSARTViaGPIO(void)
{
    MyGPIO expectedStruct = 
    {
        .gpio_register = MY_USART_GPIO,
        .pin_mask = (MY_USART_RX | MY_USART_TX),
        .mode = GPIO_ALT,
        .alt_func = MY_USART_ALT,
        .output_type = GPIO_PUSH_PULL,
        .pupd = GPIO_PUPD_UP
    };
    MyGPIO_Init_ExpectAndReturn(&expectedStruct, ECODE_OK);
    MyReceiver_Init(MY_USART_GPIO, MY_USART_ALT, MY_USART_RX, MY_USART_TX);
}
