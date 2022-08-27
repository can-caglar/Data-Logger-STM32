#ifndef MY_RECEIVER_H_INCLUDED
#define MY_RECEIVER_H_INCLUDED

typedef enum
{
    RCVR_NOT_RECEIVED,
    RCVR_RECEIVED,
    RCVR_DONE
} ReceiverEcode_e;

#include "MyCommon.h"
#include "stm32f407xx.h"

ReceiverEcode_e MyReceiver_Receive(void);
void MyReceiver_Clear(void);
char* MyReceiver_GetBuffer(void);
void MyReceiver_Init(USART_TypeDef* usart,
                     GPIO_TypeDef* gpio, 
                     GPIO_ALTF_e altf,
                     GPIO_Pin_Mask_t rx,
                     GPIO_Pin_Mask_t tx);

#endif
