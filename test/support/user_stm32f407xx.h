#ifndef FAKE_STM32F4XX_H_INCLUDED
#define FAKE_STM32F4XX_H_INCLUDED

#include <stm32f031x6.h>

// Target shall use actual registers. Dev machine will use fake ones in RAM.
// To use actual registers, peripheral clocks have to be enabled.
#ifdef ON_TARGET

// nothing, go as normal
#elif defined(ON_DEV_PC)

#include <stdint.h>

// GPIOA and GPIOB uses pins used for debugging/jtag, so skipping those
// GPIOA for SWDIO, and GPIOB for jtag (used in printf)
extern GPIO_TypeDef FakeGPIOD;
extern GPIO_TypeDef FakeGPIOC;
extern RCC_TypeDef  FakeRCC;
extern USART_TypeDef FakeUSART6;

#define GPIOD (&FakeGPIOD)
#define GPIOC (&FakeGPIOC)
#define RCC   (&FakeRCC)
#define USART6 (&FakeUSART6)

extern uint32_t SystemCoreClock;
void SystemCoreClockUpdate(void);

#else
    #error Either ON_TARGET or ON_DEV_PC must be defined. Your build config seems to be wrong!
#endif

// Common to all
#define RCC_GPIO_EN_REG (&(RCC->AHB1ENR))
#define RCC_USART1_EN_REG (&(RCC->APB2ENR))
#define RCC_USART6_EN_REG (&(RCC->APB2ENR))

#endif
