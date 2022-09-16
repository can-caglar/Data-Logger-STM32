#include "MySM.h"
#include "MyReceiver.h"
#include "MyProcessor.h"

typedef enum MySMState
{
    RECEIVING,
    PROCESSING,
} MySMState;

void MySM_Run(void)
{
    if (MyReceiver_Receive() == RCVR_DONE)
    {
        char* rcvBuf = MyReceiver_GetBuffer();
        MyProcessor_HandleCommandWithString(rcvBuf);
        const char* xmitBuf = MyProcessor_GetResponseMessage();
        MyReceiver_Transmit(xmitBuf);
        MyReceiver_Clear(); // processing done, clear it
    }
}

void MySM_Init(void)
{
    MyReceiver_Init();
}

