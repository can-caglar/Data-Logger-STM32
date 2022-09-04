#include "MySM.h"
#if 0 
#include "MyCommonTypes.h"
#endif
#include "MyReceiver.h"
#include "MyProcessor.h"

typedef enum MySMState
{
    IDLE,
    RECEIVING,
    PROCESSING,
} MySMState;

static MySMState SM;

void MySM_Run(void)
{
    switch (SM)
    {
        case IDLE:  // this feels a bit unnecessary now!
        {
            if (MyReceiver_Receive() == RCVR_RECEIVED)
            {
                SM = RECEIVING;
            }
        }
        break;
        case RECEIVING:
        {
            if (MyReceiver_Receive() == RCVR_DONE)
            {
                SM = PROCESSING;
            }
        }
        break;
        case PROCESSING:
        {
            char* rcvBuf = MyReceiver_GetBuffer();
            MyProcessor_HandleCommandWithString(rcvBuf);
            const char* xmitBuf = MyProcessor_GetResponseMessage();
            MyReceiver_Transmit(xmitBuf);
            MyReceiver_Clear(); // processing done, clear it
            SM = IDLE;
        }
        break;
    }
}

void MySM_Init(void)
{
    SM = IDLE;
    MyReceiver_Init();
}

