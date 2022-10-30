#ifdef TEST

#include "unity.h"

#include "MyRCC.h"
#include "user_stm32f407xx.h"
#include "MyCommon.h"

#include <stdio.h>
#include <string.h>

#define ALL_BITS (~0U)

void setUp(void)
{
   RCC->AHBENR = 0;
   RCC->APB2ENR = 0;
}

void tearDown(void)
{
   RCC->AHBENR = 0;
   RCC->APB2ENR = 0;
}

void test_MyRCC_GpioClockEnableWorksForAllPorts(void)
{
   for (int i = GPIO_PORT_A_e; i < GPIO_PORT_COUNT_e; i++)
   {
       TEST_ASSERT_EQUAL_INT(ECODE_OK, MyRCC_GPIOClockEnable(RCC_GPIO_EN_REG, i));
       TEST_ASSERT_BIT_HIGH(i, RCC->AHBENR);
   }
}

void test_MyRCC_USARTClockEnable(void)
{
   TEST_ASSERT_EQUAL_INT(ECODE_OK, MyRCC_USARTClockEnable(RCC_USART1_EN_REG, MY_USART_UART_RCC_MASK));
   TEST_ASSERT_BIT_HIGH(14, RCC->APB2ENR);
}

#endif // TEST

/*
- [x] MyRCC will be responsible for enabling the clocks for GPIO peripherals
- [ ] Eventually, will also be responsible for resetting peripheral (clocks?)
- [ ] For setting system clock
- [ ] And other peripherals

*/
