#include "unity.h"
#include "MySM.h"
#include <stdio.h>
#include "mock_MyReceiver.h"
#include "mock_MyProcessor.h"
#include "MyCommon.h"

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
    // init shall initialise MyReceiver
    MyReceiver_Init_Expect();
    MySM_Init();    // starts off at IDLE state
}

void tearDown(void)
{
}

#if 0
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

void test_RECV(void)
{
    moveFromIdleToReceivingOnceReceived();

    remainsInReceivingStateWHILSTReceiving();
    remainsInReceivingStateWHILSTReceiving();
}
#endif

void test_RCV_to_PROCESSING_then_IDLE(void)
{
    // The expectations essentially tell us which state we
    // are in.

    // receiving
    MyReceiver_Receive_ExpectAndReturn(RCVR_RECEIVED);
    MySM_Run();

    MyReceiver_Receive_ExpectAndReturn(RCVR_DONE);
    MySM_Run();

    // processing
    char* fakeStr = "doBadCmd";
    char* fakeResponse = "doBadCmd Failed";
    MyReceiver_GetBuffer_ExpectAndReturn(fakeStr);
    MyProcessor_HandleCommandWithString_Expect(fakeStr);
    MyProcessor_GetResponseMessage_ExpectAndReturn(fakeResponse);
    MyReceiver_Transmit_Expect(fakeResponse);
    MyReceiver_Clear_Expect();
    MySM_Run();

    // receiving
    MyReceiver_Receive_ExpectAndReturn(RCVR_NOT_RECEIVED);
    MySM_Run();
}


/******************************* Helper functions ****************************/

static void expectToDoProcessing(void)
{
    char* fakeStr = "doBadCmd";
    char* fakeResponse = "doBadCmd Failed";
    MyReceiver_GetBuffer_ExpectAndReturn(fakeStr);
    MyProcessor_HandleCommandWithString_Expect(fakeStr);
    MyProcessor_GetResponseMessage_ExpectAndReturn(fakeResponse);
    MyReceiver_Transmit_Expect(fakeResponse);
    MyReceiver_Clear_Expect();
    MySM_Run();
}
