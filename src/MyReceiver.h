#ifndef MY_RECEIVER_H_INCLUDED
#define MY_RECEIVER_H_INCLUDED

#define NOT_RECEIVED 0
#define RECEIVED 1
#define DONE 2

#include "MyCommon.h"
#include "stm32f407xx.h"

int MyReceiver_Receive(void);
void MyReceiver_Clear(void);
char* MyReceiver_GetBuffer(void);
void MyReceiver_Init(GPIO_TypeDef* gpio, 
                     GPIO_ALTF_e altf,
                     GPIO_Pin_Mask_t rx,
                     GPIO_Pin_Mask_t tx);

#endif