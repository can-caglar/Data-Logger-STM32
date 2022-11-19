#include "unity.h"
#include "MyReceiver.h"
#include "MyCommon.h"
#include "mock_MyUSART.h"
#include "mock_MyTerminalUART.h"
#include "unity_helper.h"
#include <stdio.h>

void initMyReceiver(void);
void initMyReceiverNoExpectations(void);
ReceiverEcode_e receiveFromUsart(char byte);
ReceiverEcode_e receiveFromUsart_String(char* string);
ReceiverEcode_e receiveFromUsartButUsartNotReady(void);
ReceiverEcode_e initAndReceive(char* string);
ReceiverEcode_e expectReceiveAndEcho(char* receivedByte, Error_Code_e willReturn);

#define FULL_STRING     "01234567890123456789"

void setUp(void)
{

}

void tearDown(void)
{

}

void test_MyReceiver_Init_WillInitialiseUSARTAndGPIOProperly(void)
{
    MyTerminalUART_Init_Expect();

    initMyReceiver();
}

void test_MyReceiver_GetBuffer_ReceivedCharsAndEmptiedAfterInit(void)
{
    initMyReceiverNoExpectations();

    // empty after init
    char* buf = MyReceiver_GetBuffer();
    TEST_ASSERT_EQUAL_STRING("", buf);

    // not empty after receiving
    MyTerminalUART_Read_ExpectAndReturn('a');
    ReceiverEcode_e err = MyReceiver_Receive();
    TEST_ASSERT_EQUAL_INT(RCVR_RECEIVED, err);

    buf = MyReceiver_GetBuffer();
    TEST_ASSERT_EQUAL_STRING("a", buf);

    // empty again after init!
    initMyReceiverNoExpectations();
    buf = MyReceiver_GetBuffer();
    TEST_ASSERT_EQUAL_STRING("", buf);
}

void test_MyReceiver_GetBuffer_FillsUpAfter20Characters(void)
{
    initMyReceiverNoExpectations();

    // receive a full string
    ReceiverEcode_e err = receiveFromUsart_String(FULL_STRING);

    TEST_ASSERT_EQUAL_INT(RCVR_RECEIVED, err);

    char* buf = MyReceiver_GetBuffer();

    TEST_ASSERT_EQUAL_STRING(FULL_STRING, buf);

    // receive 1 more character.
    MyTerminalUART_Read_ExpectAndReturn('c');

    err = MyReceiver_Receive();
    TEST_ASSERT_EQUAL_INT(RCVR_RECEIVED, err);

    // Buffer shall not have expanded
    buf = MyReceiver_GetBuffer();
    TEST_ASSERT_EQUAL_STRING(FULL_STRING, buf);

    // receive \r character.
    MyTerminalUART_Read_ExpectAndReturn('\r');

    // shall be done
    err = MyReceiver_Receive();
    TEST_ASSERT_EQUAL_INT(RCVR_DONE, err);
}

void test_MyReceiver_Returns_RECEIVED_EvenIfErrorAsLongAsReceivedOnce(void)
{
    initMyReceiverNoExpectations();

    ReceiverEcode_e err = receiveFromUsartButUsartNotReady();

    TEST_ASSERT_EQUAL_INT(RCVR_NOT_RECEIVED, err);

    MyTerminalUART_Read_ExpectAndReturn('a');
    err = MyReceiver_Receive();

    TEST_ASSERT_EQUAL_INT(RCVR_RECEIVED, err);

    MyTerminalUART_Read_ExpectAndReturn(0);
    err = MyReceiver_Receive();

    TEST_ASSERT_EQUAL_INT(RCVR_RECEIVED, err);  // still "rcvr_received"
}

void test_MyReceiver_GetBuffer_FillsUpAfterReceivingReturn(void)
{
    initMyReceiverNoExpectations();

    ReceiverEcode_e err = receiveFromUsart_String("012\r345");
    TEST_ASSERT_EQUAL_INT(RCVR_DONE, err);

    char* buf = MyReceiver_GetBuffer();
    TEST_ASSERT_EQUAL_STRING("012", buf);
}

void test_MyReceiver_Clear_WillEmptyTheInbufAndSetStateToNOTRECEIVED(void)
{
    initMyReceiverNoExpectations();
    receiveFromUsart_String("test");

    MyReceiver_Clear();
    char* buf = MyReceiver_GetBuffer();
    TEST_ASSERT_EQUAL_STRING("", buf);

    ReceiverEcode_e err = receiveFromUsartButUsartNotReady();

    TEST_ASSERT_EQUAL_INT(RCVR_NOT_RECEIVED, err);
}

void test_MyReceiver_AbleToContinueReceivingAfterClearing(void)
{
    initMyReceiverNoExpectations();

    receiveFromUsart_String("DryRun");
    TEST_ASSERT_EQUAL_STRING("DryRun", MyReceiver_GetBuffer());

    MyReceiver_Clear();

    receiveFromUsart_String("NextWord");
    TEST_ASSERT_EQUAL_STRING("NextWord", MyReceiver_GetBuffer());
}

void test_MyReceiver_Transmit_DoesntTransferIfNullptr(void)
{
    MyReceiver_Transmit(0);
}

void test_MyReceiver_Transmit_WillSendStringsViaUART(void)
{
    MyTerminalUART_WriteString_Expect("hello!");
    MyTerminalUART_Write_Expect('\r');

    MyReceiver_Transmit("hello!");
}

/******************************** Helper Functions *****************/

void initMyReceiver(void)
{
    MyReceiver_Init();
}

void initMyReceiverNoExpectations(void)
{
    MyTerminalUART_Init_Ignore();
    MyReceiver_Init();
}

ReceiverEcode_e receiveFromUsart(char byte)
{
    return expectReceiveAndEcho(&byte, ECODE_OK);
}

// Useful as it shouldn't change what MyReceiver_Receive() will return
ReceiverEcode_e receiveFromUsartButUsartNotReady(void)
{
    MyTerminalUART_Read_ExpectAndReturn(0);
    ReceiverEcode_e err = MyReceiver_Receive();
}

// TODO, simplify this?
ReceiverEcode_e receiveFromUsart_String(char* string)
{
    ReceiverEcode_e err = RCVR_NOT_RECEIVED;
    while (*string)
    {
        MyTerminalUART_Read_ExpectAndReturn(*string);
        err = MyReceiver_Receive();
        if (err != RCVR_RECEIVED)
        {
            break;
        }
        string++;
    }
    return err;
}

ReceiverEcode_e expectReceiveAndEcho(char* receivedByte, Error_Code_e willReturn)
{
    char readData = 0;  // should pass in a 0
    MyUSART_Read_ExpectAndReturn(MY_USART, &readData, willReturn);
    MyUSART_Read_ReturnThruPtr_readData(receivedByte);

    MyUSART_Write_ExpectAndReturn(MY_USART, *receivedByte, ECODE_OK);

    return MyReceiver_Receive();
}

/*
- [x] MyReceiver_Receive will return 0 if received 0 and buffer is empty
- [x] MyReceiver_Receive will continue to return RECEIVED even if uart fails
- [x] MyReceiver clear shall clear the input buffer and reset state to NOT_RECEIVED
- [x] Shall echo back to terminal 
- [x] Also shall write \n automatically when full

- [ ] TODO, no longer use UART. Use the a "singleton" for USART6. Have the transmission
and reception go through that module instead. Let it have unique transmit ability
*/