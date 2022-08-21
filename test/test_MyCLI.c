#include "unity.h"
#include "MySM.h"
#include <stdio.h>
#include "mock_MyReceiver.h"
#include "mock_MyProcessor.h"
#include "mock_MyTransmitter.h"

static void remainsInIdleStateWhenNotReceiving(void);
static void remainsInReceivingStateWhilstNOTReceiving(void);
static void remainsInReceivingStateWHILSTReceiving(void);
static void moveFromIdleToReceivingOnceReceived(void);
static void moveFromReceivingToProcessingOnceReceiveDone(void);
static void expectToDoProcessing(void);
static void expectMyReceiver_NotReceive(void);
static void expectToDoTransmit(void);

void setUp(void)
{
    MySM_Init();    // starts off at IDLE state
}

void tearDown(void)
{
}

void test_IDLE(void)
{
    remainsInIdleStateWhenNotReceiving();
    remainsInIdleStateWhenNotReceiving();
}

void test_IDLE_to_RECV(void)
{
    moveFromIdleToReceivingOnceReceived();

    remainsInReceivingStateWhilstNOTReceiving();
    remainsInReceivingStateWhilstNOTReceiving();
}

void test_IDLE_to_PROCESSING_shouldntHappen(void)
{
    // Currently in IDLE
    MyReceiver_Clear_Expect();
    MyReceiver_Receive_ExpectAndReturn(DONE);
    MySM_Run();

    remainsInIdleStateWhenNotReceiving();
    remainsInIdleStateWhenNotReceiving();
}

void test_RECV(void)
{
    moveFromIdleToReceivingOnceReceived();

    remainsInReceivingStateWHILSTReceiving();
    remainsInReceivingStateWHILSTReceiving();
}

void test_RCV_to_PROCESSING(void)
{
    moveFromIdleToReceivingOnceReceived();

    moveFromReceivingToProcessingOnceReceiveDone();
    expectToDoProcessing();

    // should auto go to IDLE when done
    remainsInIdleStateWhenNotReceiving();
}

/******************************* Helper functions ****************************/

static void remainsInIdleStateWhenNotReceiving(void)
{
    MyReceiver_Clear_Expect();
    MyReceiver_Receive_ExpectAndReturn(NOT_RECEIVED);
    MySM_Run();
}

static void moveFromIdleToReceivingOnceReceived(void)
{
    MyReceiver_Clear_Expect();
    MyReceiver_Receive_ExpectAndReturn(RECEIVED);
    MySM_Run();
}

static void expectMyReceiver_NotReceive(void)
{
    MyReceiver_Receive_ExpectAndReturn(NOT_RECEIVED);
    MySM_Run();
}

static void remainsInReceivingStateWhilstNOTReceiving(void)
{
    MyReceiver_Receive_ExpectAndReturn(NOT_RECEIVED);
    MySM_Run();
}

static void remainsInReceivingStateWHILSTReceiving(void)
{
    MyReceiver_Receive_ExpectAndReturn(RECEIVED);
    MySM_Run();
}

static void moveFromReceivingToProcessingOnceReceiveDone(void)
{
    MyReceiver_Receive_ExpectAndReturn(DONE);
    MySM_Run();
}

static void expectToDoProcessing(void)
{
    char* fakeStr = "doBadCmd";
    char* fakeResponse = "doBadCmd Failed";
    MyReceiver_GetBuffer_ExpectAndReturn(fakeStr);
    MyProcessor_HandleCommandWithString_Expect(fakeStr);
    MyProcessor_GetResponseMessage_ExpectAndReturn(fakeResponse);
    MyTransmitter_Transmit_Expect(fakeResponse);
    MySM_Run();
}


/*

[ ] Write to terminal and get board to do stuff, i.e. turn on an LED
[ ] The board writes back whetever we sent, to terminal
[ ] The board returns original cmd + "success" or "fail" to terminal

[ ] State machine:
[ ] Initial state is: IDLE (where the buffer is cleared and waiting to receive)
[ ] If start receiving, go to: RECEIVING (where buf size is checked etc)
[ ] If received or buffer full, go to: PROCESSING
[ ] If found function, go to: INVOKING, if not found go to TRANSMIT
[ ] Invoke function in INVOKING, then go to TRANSMIT
[ ] TRANSMIT a message depending on if invoke was called or not
*/