#include "unity.h"
#include "MySM.h"
#include <stdio.h>
#include "mock_MyReceiver.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_SMWhenFirstStartedWillJustReceive(void)
{
    MyReceiver_Clear_Expect();
    MyReceiver_Receive_ExpectAndReturn(NOT_RECEIVED);
    MySM_Run();
}

void test_SMWhenReceiverReceivesStopClearingItButContinueReceiving(void)
{
    MyReceiver_Clear_Expect();
    MyReceiver_Receive_ExpectAndReturn(RECEIVED);
    MySM_Run();

    MyReceiver_Receive_ExpectAndReturn(NOT_RECEIVED);
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