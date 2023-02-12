#ifndef FAKE_STM32F4XX_H_INCLUDED
#define FAKE_STM32F4XX_H_INCLUDED

#include <stm32f303x8.h>

// Target shall use actual registers. Dev machine will use fake ones in RAM.
// To use actual registers, peripheral clocks have to be enabled.
#ifdef ON_TARGET

// nothing, go as normal
#elif defined(ON_DEV_PC)

#include <stdint.h>

extern GPIO_TypeDef FakeGPIOA;
extern GPIO_TypeDef FakeGPIOB;
extern RCC_TypeDef  FakeRCC;
extern USART_TypeDef FakeUSART1;

#define GPIOA (&FakeGPIOA)
#define GPIOB (&FakeGPIOB)
#define RCC   (&FakeRCC)
#define USART1 (&FakeUSART1)

extern uint32_t SystemCoreClock;
void SystemCoreClockUpdate(void);

#else
    #error Either ON_TARGET or ON_DEV_PC must be defined. Your build config seems to be wrong!
#endif

// Common to all
#define RCC_GPIO_EN_REG (&(RCC->AHBENR))
#define RCC_USART1_EN_REG (&(RCC->APB2ENR))

#endif
