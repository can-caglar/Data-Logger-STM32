#include "unity.h"
#include "MyCLI.h"
#include <stdio.h>
#include "mock_MyReceiver.h"
#include "mock_MyProcessor.h"
#include "MyCommon.h"

#define INTRO_MESSAGE \
    "\r"\
    "********* SERIAL SNOOPER V2.0 *********\r"\
    "Enter \"seeAll\" to see list of commands"

void setUp(void)
{
    // init shall initialise MyReceiver
    MyReceiver_Init_Expect();
    MyReceiver_Transmit_Expect(INTRO_MESSAGE);
    MyCLI_Init();    // starts off at IDLE state
}

void tearDown(void)
{
}

void test_RCV_to_PROCESSING_then_IDLE(void)
{
    // receiving
    MyReceiver_Receive_ExpectAndReturn(RCVR_RECEIVED);
    MyCLI_Run();

    // processing
    MyReceiver_Receive_ExpectAndReturn(RCVR_DONE);
    char* fakeStr = "doBadCmd";
    char* fakeResponse = "doBadCmd Failed";
    MyReceiver_GetBuffer_ExpectAndReturn(fakeStr);
    MyProcessor_HandleCommandWithString_Expect(fakeStr);
    MyProcessor_GetResponseMessage_ExpectAndReturn(fakeResponse);
    MyReceiver_Transmit_Expect(fakeResponse);
    MyReceiver_Clear_Expect();
    MyCLI_Run();

    // receiving
    MyReceiver_Receive_ExpectAndReturn(RCVR_NOT_RECEIVED);
    MyCLI_Run();
}
