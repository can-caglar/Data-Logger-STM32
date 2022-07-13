#include "src/MyCommon.h"
#include "test/support/fake_stm32f407xx.h"
#include "src/MyRCC.h"
#include "C:/Ruby27-x64/lib/ruby/gems/2.7.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"














void setUp(void)

{

   (&FakeRCC)->AHB1ENR = 0;

}



void tearDown(void)

{

   (&FakeRCC)->AHB1ENR = 0;

}



void test_MyRCC_GpioClockEnableWorksForAllPorts(void)

{

   for (int i = GPIO_PORT_A_e; i < GPIO_PORT_COUNT_e; i++)

   {

       UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyRCC_GPIOClockEnable(&((&FakeRCC)->AHB1ENR), i))), (

      ((void *)0)

      ), (UNITY_UINT)(26), UNITY_DISPLAY_STYLE_INT);

       UnityAssertBits((UNITY_INT)(((UNITY_UINT)1 << (i))), (UNITY_INT)((UNITY_UINT)(-1)), (UNITY_INT)(((&FakeRCC)->AHB1ENR)), (

      ((void *)0)

      ), (UNITY_UINT)(27));

   }

}
