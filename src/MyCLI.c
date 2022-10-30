#include "MyCLI.h"
#include "MyReceiver.h"
#include "MyProcessor.h"
#include "MyCommon.h"

#define INTRO_STR \
    "\r"\
    "********* SERIAL SNOOPER V1.0 *********\r"\
    "Enter \"seeAll\" to see list of commands"

void MyCLI_Run(void)
{
    // Receive characters
    if (MyReceiver_Receive() == RCVR_DONE)
    {
        // finishing receiving a complete line
        char* rcvBuf = MyReceiver_GetBuffer();
        // invoke this line
        MyProcessor_HandleCommandWithString(rcvBuf);
        // get response message
        const char* xmitBuf = MyProcessor_GetResponseMessage();
        // transmit the response message
        MyReceiver_Transmit(xmitBuf);
        // clear internal string buffer, ready for next time
        MyReceiver_Clear();
    }
}

void MyCLI_Init(void)
{
    MyReceiver_Init();
    MyReceiver_Transmit(INTRO_STR);
}

