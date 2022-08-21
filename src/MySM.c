#include "MySM.h"
#if 0 
#include "MyCommonTypes.h"
#endif
#include "MyReceiver.h"

typedef enum MySMState
{
    IDLE,
    RECEIVING
} MySMState;

static MySMState SM = IDLE;

void MySM_Run(void)
{
    switch (SM)
    {
        case IDLE:
            MyReceiver_Clear();
            if (MyReceiver_Receive() == RECEIVED)
            {
                SM = RECEIVING;
            }
        break;
        case RECEIVING:
            MyReceiver_Receive();
        break;
    }
}

