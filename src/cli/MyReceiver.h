#ifndef MY_RECEIVER_H_INCLUDED
#define MY_RECEIVER_H_INCLUDED

typedef enum
{
    RCVR_NOT_RECEIVED,
    RCVR_RECEIVED,
    RCVR_DONE
} ReceiverEcode_e;

#include "MyCommon.h"
#include "stm32f031x6.h"

ReceiverEcode_e MyReceiver_Receive(void);
void MyReceiver_Transmit(const char* buf);
void MyReceiver_Clear(void);
char* MyReceiver_GetBuffer(void);
void MyReceiver_Init(void);

#endif