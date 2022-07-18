#ifdef ON_DEV_PC

#include "user_stm32f407xx.h"
#include <stdint.h>

GPIO_TypeDef FakeGPIOD;
GPIO_TypeDef FakeGPIOC;
RCC_TypeDef  FakeRCC;
USART_TypeDef FakeUSART6;
uint32_t SystemCoreClock;

void SystemCoreClockUpdate(void)
{
    SystemCoreClock = 16000000;
}

#endif