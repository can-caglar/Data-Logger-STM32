#include "src/MyCommon.h"
#include "src/MyRCC.h"
#include "test/support/Fake_stm32f407xx.h"
#include "src/MyGPIO.h"
#include "C:/Ruby27-x64/lib/ruby/gems/2.7.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"




static io_register pinToMODER_Out(int pin);

static io_register posToBits(int pin);

static void assertOnlyTheseBitsHigh(const io_register mask, const io_register reg);

static void assertBitsAreLOW(const io_register mask, const io_register reg);

static io_register pinToMODER_OutAllPins(void);

static void enablePeripheralClocks(void);











    GPIO_TypeDef FakeGPIOA;

    GPIO_TypeDef FakeGPIOB;

    RCC_TypeDef FakeRCC;













void setUp(void)

{



    enablePeripheralClocks();

    memset((&FakeGPIOA), 0, sizeof *(&FakeGPIOA));

    memset(&FakeGPIOB, 0, sizeof FakeGPIOB);

}



void tearDown(void)

{

}



void test_MyGPIO_ConstantsAreCorrect(void)

{

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((0x0)), (UNITY_INT)(UNITY_INT32)((GPIO_INPUT)), (

   ((void *)0)

   ), (UNITY_UINT)(68), UNITY_DISPLAY_STYLE_HEX32);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((0x1)), (UNITY_INT)(UNITY_INT32)((GPIO_OUTPUT)), (

   ((void *)0)

   ), (UNITY_UINT)(69), UNITY_DISPLAY_STYLE_HEX32);



    UnityAssertEqualNumber((UNITY_INT)((16)), (UNITY_INT)((16)), (

   ((void *)0)

   ), (UNITY_UINT)(71), UNITY_DISPLAY_STYLE_INT);



    GPIO_Pin_e pins[16] =

    {

        GPIO_PIN_0_e, GPIO_PIN_1_e, GPIO_PIN_2_e, GPIO_PIN_3_e,

        GPIO_PIN_4_e, GPIO_PIN_5_e, GPIO_PIN_6_e, GPIO_PIN_7_e,

        GPIO_PIN_8_e, GPIO_PIN_9_e, GPIO_PIN_10_e, GPIO_PIN_11_e,

        GPIO_PIN_12_e, GPIO_PIN_13_e, GPIO_PIN_14_e, GPIO_PIN_15_e,

    };



    for (int i = 0; i < 16; i++)

    {

        UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)((1UL << i)), (UNITY_INT)(UNITY_INT32)((pins[i])), (

       ((void *)0)

       ), (UNITY_UINT)(83), UNITY_DISPLAY_STYLE_HEX32);

    }

}



void test_MyGPIO_OutputIsInitialisedCorrectlyWhen1PinPassedIn(void)

{

    MyGPIO_Init((&FakeGPIOA), GPIO_PIN_1_e, GPIO_OUTPUT);



    assertOnlyTheseBitsHigh(pinToMODER_Out(1), (&FakeGPIOA)->MODER);

}



void test_MyGPIO_OutputIsInitialisedCorrectlyWhenMultiplePinsPassedIn(void)

{

    MyGPIO_Init((&FakeGPIOA), (GPIO_Pin_e)(GPIO_PIN_2_e | GPIO_PIN_3_e | GPIO_PIN_4_e), GPIO_OUTPUT);



    io_register expected = pinToMODER_Out(2) | pinToMODER_Out(3) | pinToMODER_Out(4);



    assertOnlyTheseBitsHigh(expected, (&FakeGPIOA)->MODER);

}



void test_MyGPIO_OutputIsInitialisedCorrectlyWhenAllPinsPassedInSameTime(void)

{

    MyGPIO_Init((&FakeGPIOA), GPIO_PIN_ALL_e, GPIO_OUTPUT);



    io_register expected = pinToMODER_OutAllPins();



    assertOnlyTheseBitsHigh(expected, (&FakeGPIOA)->MODER);

}



void test_MyGPIO_WritingToUninitialisedPortDoesNothing(void)

{

    UnityAssertEqualNumber((UNITY_INT)((ECODE_NOT_OUTPUT)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_4_e, 1))), (

   ((void *)0)

   ), (UNITY_UINT)(114), UNITY_DISPLAY_STYLE_INT);

    assertOnlyTheseBitsHigh(0, (&FakeGPIOA)->ODR);

}



void test_MyGPIO_SinglePinCanBeSetHigh(void)

{

    MyGPIO_Init((&FakeGPIOA), GPIO_PIN_4_e, GPIO_OUTPUT);



    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_4_e, 1))), (

   ((void *)0)

   ), (UNITY_UINT)(122), UNITY_DISPLAY_STYLE_INT);



    assertOnlyTheseBitsHigh(posToBits(4), (&FakeGPIOA)->ODR);

}



void test_MyGPIO_OutputMultiplePinsPassedInCanBeSetHigh(void)

{

    MyGPIO_Init((&FakeGPIOA), (GPIO_Pin_e)(GPIO_PIN_6_e | GPIO_PIN_7_e), GPIO_OUTPUT);



    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), (GPIO_Pin_e)(GPIO_PIN_6_e | GPIO_PIN_7_e), 1))), (

   ((void *)0)

   ), (UNITY_UINT)(131), UNITY_DISPLAY_STYLE_INT);



    io_register expected = posToBits(6) | posToBits(7);

    assertOnlyTheseBitsHigh(expected, (&FakeGPIOA)->ODR);

}



void test_MyGPIO_OutputCanBeSetHighWhenCalledMultipleTimes(void)

{

    MyGPIO_Init((&FakeGPIOA), (GPIO_Pin_e)(GPIO_PIN_6_e | GPIO_PIN_7_e | GPIO_PIN_8_e), GPIO_OUTPUT);



    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_6_e, 1))), (

   ((void *)0)

   ), (UNITY_UINT)(141), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_7_e, 1))), (

   ((void *)0)

   ), (UNITY_UINT)(142), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_8_e, 1))), (

   ((void *)0)

   ), (UNITY_UINT)(143), UNITY_DISPLAY_STYLE_INT);



    io_register expected = posToBits(6) | posToBits(7) | posToBits(8);

    assertOnlyTheseBitsHigh(expected, (&FakeGPIOA)->ODR);

}



void test_MyGPIO_OutputCanBeSetLow_1Pin(void)

{

    MyGPIO_Init((&FakeGPIOA), GPIO_PIN_ALL_e, GPIO_OUTPUT);

    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_ALL_e, 1))), (

   ((void *)0)

   ), (UNITY_UINT)(152), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_8_e, 0))), (

   ((void *)0)

   ), (UNITY_UINT)(154), UNITY_DISPLAY_STYLE_INT);



    assertBitsAreLOW(posToBits(8), (&FakeGPIOA)->ODR);

}



void test_MyGPIO_InitPinOutsideRangeReturnsNullPtr(void)

{

    io_register mask = posToBits(16);

    UnityAssertEqualNumber((UNITY_INT32)((0)), (UNITY_INT32)((MyGPIO_Init((&FakeGPIOA), (GPIO_Pin_e)mask, GPIO_OUTPUT))), (

   ((void *)0)

   ), (UNITY_UINT)(162), UNITY_DISPLAY_STYLE_HEX32);

    assertOnlyTheseBitsHigh(0, (&FakeGPIOA)->MODER);



    mask = posToBits(31);

    UnityAssertEqualNumber((UNITY_INT32)((0)), (UNITY_INT32)((MyGPIO_Init((&FakeGPIOA),(GPIO_Pin_e)mask, GPIO_OUTPUT))), (

   ((void *)0)

   ), (UNITY_UINT)(166), UNITY_DISPLAY_STYLE_HEX32);

    assertOnlyTheseBitsHigh(0, (&FakeGPIOA)->MODER);

}



void test_MyGPIO_OutputCanBeSetLowWhenCalledMultipleTimes(void)

{

    MyGPIO_Init((&FakeGPIOA), GPIO_PIN_ALL_e, GPIO_OUTPUT);

    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_ALL_e, 1))), (

   ((void *)0)

   ), (UNITY_UINT)(173), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_6_e, 0))), (

   ((void *)0)

   ), (UNITY_UINT)(175), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), GPIO_PIN_8_e, 0))), (

   ((void *)0)

   ), (UNITY_UINT)(176), UNITY_DISPLAY_STYLE_INT);



    io_register expected = 0xFFFF & ~(posToBits(6) | posToBits(8));

    assertOnlyTheseBitsHigh(expected, (&FakeGPIOA)->ODR);

}



void test_MyGPIO_MultiplePortsMayBeInitialisedAsOutput(void)

{

    MyGPIO_Init((&FakeGPIOA), GPIO_PIN_ALL_e, GPIO_OUTPUT);

    MyGPIO_Init(&FakeGPIOB, GPIO_PIN_ALL_e, GPIO_OUTPUT);



    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write((&FakeGPIOA), (GPIO_Pin_e)(GPIO_PIN_1_e | GPIO_PIN_2_e), 1))), (

   ((void *)0)

   ), (UNITY_UINT)(187), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((ECODE_OK)), (UNITY_INT)((MyGPIO_Write(&FakeGPIOB, (GPIO_Pin_e)(GPIO_PIN_1_e | GPIO_PIN_3_e), 1))), (

   ((void *)0)

   ), (UNITY_UINT)(188), UNITY_DISPLAY_STYLE_INT);



    assertOnlyTheseBitsHigh(posToBits(1) | posToBits(2), (&FakeGPIOA)->ODR);

    assertOnlyTheseBitsHigh(posToBits(1) | posToBits(3), FakeGPIOB.ODR);

}









static io_register pinToMODER_Out(int pin)

{

    return (0x1U << (pin * 2));

}



static io_register pinToMODER_OutAllPins(void)

{

    io_register ret = 0;

    for (uint8_t i = 0; i < 16; i++)

    {

        ret |= pinToMODER_Out(i);

    }

    return ret;

}



static io_register posToBits(int pin)

{

    return (0x1U << pin);

}



static void assertOnlyTheseBitsHigh(const io_register mask, const io_register reg)

{

    UnityAssertBits((UNITY_INT)((mask)), (UNITY_INT)((UNITY_UINT)(-1)), (UNITY_INT)((reg)), (

   ((void *)0)

   ), (UNITY_UINT)(219));

    assertBitsAreLOW(~mask, reg);

}



static void assertBitsAreLOW(const io_register mask, const io_register reg)

{

    UnityAssertBits((UNITY_INT)((mask)), (UNITY_INT)((UNITY_UINT)(0)), (UNITY_INT)((reg)), (

   ((void *)0)

   ), (UNITY_UINT)(225));

}



static void enablePeripheralClocks(void)

{

    MyRCC_GPIOClockEnable(&((&FakeRCC)->AHB1ENR), GPIO_PORT_A_e);

    MyRCC_GPIOClockEnable(&((&FakeRCC)->AHB1ENR), GPIO_PORT_B_e);

}
