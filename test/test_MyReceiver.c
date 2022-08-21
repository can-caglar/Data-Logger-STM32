#include "unity.h"
#include "MyReceiver.h"
#include "MyCommon.h"
#include "mock_MyUSART.h"
#include "mock_MyGPIO.h"
#include "unity_helper.h"
#include <stdio.h>

void setUp(void)
{

}

void tearDown(void)
{

}

void test_MyReceiver_Init_WillInitialiseUSARTAndGPIOProperly(void)
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
    MyUSART_Init_ExpectAndReturn(MY_USART, USART_BR_19200, ECODE_OK);

    MyReceiver_Init(MY_USART, MY_USART_GPIO, MY_USART_ALT, MY_USART_RX, MY_USART_TX);
}

void test_MyReceiver_Receive_WillReadFromUSART(void)
{
    char readData = 0;
    char retData = 'a';
    MyUSART_Read_ExpectAndReturn(MY_USART, &readData, ECODE_OK);
    MyUSART_Read_ReturnThruPtr_readData(&retData);
    
    char actualData = MyReceiver_Receive();

    TEST_ASSERT_EQUAL_CHAR(retData, actualData);

}