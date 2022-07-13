//#ifdef TEST

//#include "unity.h"

//#include "MyRCC.h"
//#include "Fake_stm32f407xx.h"
//#include "MyCommon.h"

//#include <stdio.h>
//#include <string.h>

//RCC_TypeDef FakeRCC;

//void setUp(void)
//{
//    memset(&FakeRCC, 0, sizeof FakeRCC);
//}

//void tearDown(void)
//{
//}

//void test_MyRCC_GpioClockEnableWorksForAllPorts(void)
//{
//    // need to then write tests for the clock enable module (make rcc module and test it)
//    // rcc module can just make use of the RCC_TypeDef struct as they contain all ports
//    for (int i = GPIO_PORT_A_e; i < GPIO_PORT_COUNT_e; i++)
//    {
//        TEST_ASSERT_EQUAL_INT(ECODE_OK, MyRCC_GPIOClockEnable(&FakeRCC.AHB1ENR, i));
//        TEST_ASSERT_BIT_HIGH(i, FakeRCC.AHB1ENR);
//    }
//}

//// When back, read through test and structure of code again to familarise.
//// I think we have done the important part of MyRCC, so we can continue working on the GPIO driver
//// TDD helped identify that it's best to leave the gpio clock enable outside of gpio driver. how cool.

//#endif // TEST

///*
//- [ ] MyRCC will be responsible for enabling the clocks for GPIO peripherals
//- [ ] Eventually, will also be responsible for resetting peripheral (clocks?)
//- [ ] For setting system clock
//- [ ] And other peripherals


//*/
