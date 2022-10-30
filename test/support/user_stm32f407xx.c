#ifdef ON_DEV_PC

#include "user_stm32f407xx.h"
#include <stdint.h>

GPIO_TypeDef FakeGPIOA;
GPIO_TypeDef FakeGPIOB;
RCC_TypeDef  FakeRCC;
USART_TypeDef FakeUSART1;
uint32_t SystemCoreClock;

void SystemCoreClockUpdate(void)
{
    SystemCoreClock = 16000000;
}

#endif