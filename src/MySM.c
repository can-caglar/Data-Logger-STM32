#include "MySM.h"
#if 0 
#include "MyCommonTypes.h"
#endif
#include "MyReceiver.h"
#include "MyProcessor.h"
#include "MyTransmitter.h"

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
        case IDLE:
        {
            MyReceiver_Clear();
            if (MyReceiver_Receive() == RECEIVED)
            {
                SM = RECEIVING;
            }
        }
        break;
        case RECEIVING:
        {
            if (MyReceiver_Receive() == DONE)
            {
                SM = PROCESSING;
            }
        }
        break;
        case PROCESSING:
        {
            char* rcvBuf = MyReceiver_GetBuffer();
            MyProcessor_HandleCommandWithString(rcvBuf);
            char* xmitBuf = MyProcessor_GetResponseMessage();
            MyTransmitter_Transmit(xmitBuf);
            SM = IDLE;
        }
        break;
    }
}

void MySM_Init(void)
{
    SM = IDLE;
}

