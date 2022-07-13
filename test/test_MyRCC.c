#ifdef TEST

#include "unity.h"

#include "MyRCC.h"
#include "fake_stm32f407xx.h"
#include "MyCommon.h"

#include <stdio.h>
#include <string.h>

void setUp(void)
{
   RCC->AHB1ENR = 0;
}

void tearDown(void)
{
   RCC->AHB1ENR = 0;
}

void test_MyRCC_GpioClockEnableWorksForAllPorts(void)
{
   for (int i = GPIO_PORT_A_e; i < GPIO_PORT_COUNT_e; i++)
   {
       TEST_ASSERT_EQUAL_INT(ECODE_OK, MyRCC_GPIOClockEnable(&(RCC->AHB1ENR), i));
       TEST_ASSERT_BIT_HIGH(i, RCC->AHB1ENR);
   }
}

#endif // TEST

/*
- [x] MyRCC will be responsible for enabling the clocks for GPIO peripherals
- [ ] Eventually, will also be responsible for resetting peripheral (clocks?)
- [ ] For setting system clock
- [ ] And other peripherals

*/
