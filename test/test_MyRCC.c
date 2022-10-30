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
   for (int i = 0; i < 32; i++)
   {
       TEST_ASSERT_EQUAL_INT(ECODE_OK, MyRCC_ClockEnable(RCC_GPIO_EN_REG, i));
       TEST_ASSERT_BIT_HIGH(i, RCC->AHBENR);
   }
}

#endif // TEST

/*
- [x] MyRCC will be responsible for enabling the clocks for GPIO peripherals
- [ ] Eventually, will also be responsible for resetting peripheral (clocks?)
- [ ] For setting system clock
- [ ] And other peripherals

*/
