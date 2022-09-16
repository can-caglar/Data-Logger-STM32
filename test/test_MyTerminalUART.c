#include "unity.h"
#include "MyCommon.h"
#include "mock_MyGPIO.h"
#include "mock_MyUSART.h"
#include "mock_MyRCC.h"
#include "unity_helper.h"
#include "MyTerminalUART.h"

void setUp(void)
{

}

void tearDown(void)
{

}

void test_MyTerminalUART_Init_InitialisationInitsKnownPins(void)
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
    
    // TODO, find out why user_stm32f407xx can't be included instead
    MyRCC_GPIOClockEnable_ExpectAndReturn(&(RCC->AHB1ENR), GPIO_PORT_C_e, ECODE_OK);
    MyRCC_USARTClockEnable_ExpectAndReturn(&(RCC->APB2ENR), USART6_Mask, ECODE_OK);

    MyGPIO_Init_ExpectAndReturn(&expectedStruct, ECODE_OK);
    MyUSART_Init_ExpectAndReturn(MY_USART, USART_BR_19200, ECODE_OK);

    MyTerminalUART_Init();

    // 2nd init shall do nothing, so expect nothing
    MyTerminalUART_Init();
}

void test_MyTerminalUART_Read_CallsUARTReadAlsoEchoesAndReturnsChar(void)
{
    char readData = 0;  // should pass in a 0
    char receivedByte = 'a';
    MyUSART_Read_ExpectAndReturn(MY_USART, &readData, ECODE_OK);
    MyUSART_Read_ReturnThruPtr_readData(&receivedByte);

    MyUSART_Write_ExpectAndReturn(MY_USART, receivedByte, ECODE_NOT_READY);
    MyUSART_Write_ExpectAndReturn(MY_USART, receivedByte, ECODE_OK);

    char value = MyTerminalUART_Read();

    TEST_ASSERT_EQUAL_INT('a', value);
}

void test_MyTerminalUART_Read_WhatHappensWhenWeReadCarriageReturn(void)
{
    char readData = 0;  // should pass in a 0
    char receivedByte = '\r';
    MyUSART_Read_ExpectAndReturn(MY_USART, &readData, ECODE_OK);
    MyUSART_Read_ReturnThruPtr_readData(&receivedByte);

    MyUSART_Write_ExpectAndReturn(MY_USART, receivedByte, ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, '\n', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, '>', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, ' ', ECODE_OK);

    char value = MyTerminalUART_Read();
}


void test_MyTerminalUART_Write_SingleByteCallsUARTWriteUntilReady(void)
{
    char byteToWrite = 'z';
    MyUSART_Write_ExpectAndReturn(MY_USART, byteToWrite, ECODE_NOT_READY);
    MyUSART_Write_ExpectAndReturn(MY_USART, byteToWrite, ECODE_NOT_READY);
    MyUSART_Write_ExpectAndReturn(MY_USART, byteToWrite, ECODE_OK);

    MyTerminalUART_Write(byteToWrite);
}

void test_MyTerminalUART_WriteStringRespectsNotReady(void)
{
    char* stringToWrite = "hi";
    MyUSART_Write_ExpectAndReturn(MY_USART, 'h', ECODE_NOT_READY);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'h', ECODE_NOT_READY);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'h', ECODE_NOT_READY);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'h', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'i', ECODE_OK);

    MyTerminalUART_WriteString("hi");
}

void test_MyTerminalUART_WriteStringWillAppendNewline(void)
{
    char* stringToWrite = "hello";

    MyUSART_Write_ExpectAndReturn(MY_USART, 'h', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'e', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'l', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'l', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'o', ECODE_OK);

    MyTerminalUART_WriteString("hello");
}

void test_MyTerminalUART_Write_EndlineWillAutoWriteNewlineDenoter(void)
{
    // newline denoter here could be something like "> "
    MyUSART_Write_ExpectAndReturn(MY_USART, '\r', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, '\n', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, '>', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, ' ', ECODE_OK);

    MyTerminalUART_Write('\r');
}

void test_MyTerminalUART_WriteString_EndlineWillAutoWriteNewlineDenoter(void)
{
    // newline denoter here could be something like "> "
    MyUSART_Write_ExpectAndReturn(MY_USART, 'h', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'i', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, '\r', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, '\n', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, '>', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, ' ', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'h', ECODE_OK);
    MyUSART_Write_ExpectAndReturn(MY_USART, 'o', ECODE_OK);

    MyTerminalUART_WriteString("hi\rho");
}

/*
This is testing the MyTerminalUART module, which is an
adapter to the UART.

- [x] It initialises UART6,
- [x] It can read from UART.
- [x] It writes 1 byte or multiple bytes to UART
- [x] When writing, it will append a > to whenever it sees \r
- [x] It essentially decorates the terminal for us
- [ ] TODO how to handle initialising multiple times?
*/
