#ifdef TEST

#include <stdint.h>
#include <string.h>
#include "unity.h"
#include "MyGPIO.h"
#include "MyRCC.h"
#include "MyCommon.h"
#include "fake_stm32f407xx.h"

// Prettier names
#define pin0_mask        GPIO_PIN_0_e
#define pin1_mask        GPIO_PIN_1_e
#define pin2_mask        GPIO_PIN_2_e
#define pin3_mask        GPIO_PIN_3_e
#define pin4_mask        GPIO_PIN_4_e
#define pin5_mask        GPIO_PIN_5_e
#define pin6_mask        GPIO_PIN_6_e
#define pin7_mask        GPIO_PIN_7_e
#define pin8_mask        GPIO_PIN_8_e
#define pin9_mask        GPIO_PIN_9_e
#define pin10_mask       GPIO_PIN_10_e
#define pin11_mask       GPIO_PIN_11_e
#define pin12_mask       GPIO_PIN_12_e
#define pin13_mask       GPIO_PIN_13_e
#define pin14_mask       GPIO_PIN_14_e
#define pin15_mask       GPIO_PIN_15_e
#define pins_all_mask    GPIO_PIN_ALL_e

// Helper macros
#define MY_SIZE_OF_ARR(x) ((sizeof(x) / sizeof(x[0])))

// Forward declarations for test helper functions
static io_register pinToMODER_Out(GPIO_Pin_Number_e pin);
static io_register pinToMODER(GPIO_Pin_Number_e pin, GPIO_Mode_e mode);
static io_register posToBits(GPIO_Pin_Number_e pin);
static void assertOnlyTheseBitsHigh(const io_register mask, const io_register reg);
static void assertBitsAreLOW(const io_register mask, const io_register reg);
static io_register pinToMODER_OutAllPins(void);
static void enablePeripheralClocks(void);

void setUp(void)
{
    enablePeripheralClocks();
    GPIOD->MODER = 0;
    GPIOD->ODR = 0;
    GPIOD->IDR = 0;

    GPIOC->MODER = 0;
    GPIOC->ODR = 0;
    GPIOD->IDR = 0;
}

void tearDown(void)
{
}

void test_MyGPIO_ConstantsAreCorrect(void)
{
    TEST_ASSERT_EQUAL_HEX(0x0, GPIO_INPUT);
    TEST_ASSERT_EQUAL_HEX(0x1, GPIO_OUTPUT);

    TEST_ASSERT_EQUAL_INT(16, MAX_GPIO_PINS);

    GPIO_Pin_Mask_e pins[MAX_GPIO_PINS] = 
    {
        GPIO_PIN_0_e, GPIO_PIN_1_e, GPIO_PIN_2_e, GPIO_PIN_3_e,
        GPIO_PIN_4_e, GPIO_PIN_5_e, GPIO_PIN_6_e, GPIO_PIN_7_e,
        GPIO_PIN_8_e, GPIO_PIN_9_e, GPIO_PIN_10_e, GPIO_PIN_11_e,
        GPIO_PIN_12_e, GPIO_PIN_13_e, GPIO_PIN_14_e, GPIO_PIN_15_e,
    };

    for (int i = 0; i < MAX_GPIO_PINS; i++)
    {
        TEST_ASSERT_EQUAL_HEX(1UL << i, pins[i]);
    }
}

void test_MyGPIO_OutputIsInitialisedCorrectlyWhen1PinPassedIn(void)
{
    MyGPIO_Init(GPIOC, pin1_mask, GPIO_OUTPUT);

    assertOnlyTheseBitsHigh(pinToMODER_Out(pin_num_1), GPIOC->MODER);
}

void test_MyGPIO_OutputIsInitialisedCorrectlyWhenMultiplePinsPassedIn(void)
{
    MyGPIO_Init(GPIOC, (GPIO_Pin_Mask_e)(pin2_mask | pin3_mask | pin4_mask), GPIO_OUTPUT);
    
    io_register expected = pinToMODER_Out(pin_num_2) | pinToMODER_Out(pin_num_3) | pinToMODER_Out(pin_num_4);

    assertOnlyTheseBitsHigh(expected, GPIOC->MODER);
}

void test_MyGPIO_OutputIsInitialisedCorrectlyWhenAllPinsPassedInSameTime(void)
{
    MyGPIO_Init(GPIOC, pins_all_mask, GPIO_OUTPUT);
    
    io_register expected = pinToMODER_OutAllPins();

    assertOnlyTheseBitsHigh(expected, GPIOC->MODER);
}

void test_MyGPIO_WritingToUninitialisedPortDoesNothing(void)
{
    TEST_ASSERT_EQUAL_INT(ECODE_NOT_OUTPUT, MyGPIO_Write(GPIOC, pin4_mask, GPIO_HIGH));
    assertOnlyTheseBitsHigh(0, GPIOC->ODR);
}

void test_MyGPIO_SinglePinCanBeSetHigh(void)
{
    MyGPIO_Init(GPIOC, pin4_mask, GPIO_OUTPUT);

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pin4_mask, GPIO_HIGH));

    assertOnlyTheseBitsHigh(posToBits(pin_num_4), GPIOC->ODR);
}

void test_MyGPIO_OutputMultiplePinsPassedInCanBeSetHigh(void)
{
    MyGPIO_Init(GPIOC, (GPIO_Pin_Mask_e)(pin6_mask | pin7_mask), GPIO_OUTPUT);

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, (GPIO_Pin_Mask_e)(pin6_mask | pin7_mask), GPIO_HIGH));

    io_register expected = posToBits(pin_num_6) | posToBits(pin_num_7);
    assertOnlyTheseBitsHigh(expected, GPIOC->ODR);
}

void test_MyGPIO_OutputCanBeSetHighWhenCalledMultipleTimes(void)
{
    MyGPIO_Init(GPIOC, (GPIO_Pin_Mask_e)(pin6_mask | pin7_mask | pin8_mask), GPIO_OUTPUT);

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pin6_mask, GPIO_HIGH));
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pin7_mask, GPIO_HIGH));
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pin8_mask, GPIO_HIGH));

    io_register expected = posToBits(pin_num_6) | posToBits(pin_num_7) |  posToBits(pin_num_8);
    assertOnlyTheseBitsHigh(expected, GPIOC->ODR);
}

void test_MyGPIO_OutputCanBeSetLow_1Pin(void)
{
    MyGPIO_Init(GPIOC, pins_all_mask, GPIO_OUTPUT);
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pins_all_mask, GPIO_HIGH));

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pin8_mask, GPIO_LOW));

    assertBitsAreLOW(posToBits(pin_num_8), GPIOC->ODR);
}

void test_MyGPIO_InitPinOutsideRangeReturnsNullPtr(void)
{
    io_register mask = posToBits(MAX_GPIO_PINS);
    TEST_ASSERT_EQUAL_PTR(0, MyGPIO_Init(GPIOC, (GPIO_Pin_Mask_e)mask, GPIO_OUTPUT));
    assertOnlyTheseBitsHigh(0, GPIOC->MODER);

    mask = posToBits((GPIO_Pin_Number_e)31);
    TEST_ASSERT_EQUAL_PTR(0, MyGPIO_Init(GPIOC,(GPIO_Pin_Mask_e)mask, GPIO_OUTPUT));
    assertOnlyTheseBitsHigh(0, GPIOC->MODER);
}

void test_MyGPIO_OutputCanBeSetLowWhenCalledMultipleTimes(void)
{
    MyGPIO_Init(GPIOC, pins_all_mask, GPIO_OUTPUT);
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pins_all_mask, GPIO_HIGH));

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pin6_mask, GPIO_LOW));
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, pin8_mask, GPIO_LOW));

    io_register expected = 0xFFFF & ~(posToBits(pin_num_6) | posToBits(pin_num_8));
    assertOnlyTheseBitsHigh(expected, GPIOC->ODR);
}

void test_MyGPIO_MultiplePortsMayBeInitialisedAsOutput(void)
{
    MyGPIO_Init(GPIOC, pins_all_mask, GPIO_OUTPUT);
    MyGPIO_Init(GPIOD, pins_all_mask, GPIO_OUTPUT);

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOC, (GPIO_Pin_Mask_e)(pin1_mask | pin2_mask), GPIO_HIGH));
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOD, (GPIO_Pin_Mask_e)(pin1_mask | pin3_mask), GPIO_HIGH));

    assertOnlyTheseBitsHigh(posToBits(pin_num_1) | posToBits(pin_num_2), GPIOC->ODR);
    assertOnlyTheseBitsHigh(posToBits(pin_num_1) | posToBits(pin_num_3), GPIOD->ODR);
}

void test_MyGPIO_InitialiseAsInputMakesRegisterValue00(void)
{
    MyGPIO_Init(GPIOC, pins_all_mask, GPIO_OUTPUT);

    MyGPIO_Init(GPIOC, pin1_mask, GPIO_INPUT);
    
    io_register expected = pinToMODER_OutAllPins() & ~(pinToMODER(pin_num_1, GPIO_MODE_MASK));
    assertOnlyTheseBitsHigh(expected, GPIOC->MODER);
}

void test_MyGPIO_InputIsReadStraightFromIDRRegisterForHighSignal(void)
{
    MyGPIO_Init(GPIOC, pin3_mask, GPIO_INPUT);
    GPIOC->IDR = (0x1U << pin_num_3);

    TEST_ASSERT_EQUAL(GPIO_HIGH, MyGPIO_Read(GPIOC, pin_num_3));
}

void test_MyGPIO_InputIsReadStraightFromIDRRegisterForLowSignal(void)
{
    GPIOC->IDR = ~0U;
    MyGPIO_Init(GPIOC, pin1_mask, GPIO_INPUT);
    GPIOC->IDR &= ~(posToBits(pin_num_1));
    TEST_ASSERT_EQUAL(GPIO_LOW, MyGPIO_Read(GPIOC, pin_num_1));
}

void test_MyGPIO_ReadingFromPortNotInitialisedAsInputReturns0(void)
{
    MyGPIO_Init(GPIOC, pin2_mask, GPIO_OUTPUT);
    GPIOC->IDR = ~0U;

    TEST_ASSERT_EQUAL(GPIO_LOW, MyGPIO_Read(GPIOC, pin_num_2));
}

/************************* Private functions *********************/

static io_register pinToMODER_Out(GPIO_Pin_Number_e pin)
{
    return pinToMODER(pin, GPIO_OUTPUT);
}

static io_register pinToMODER(GPIO_Pin_Number_e pin, GPIO_Mode_e mode)
{
    return (mode << (pin * 2));
}

static io_register pinToMODER_OutAllPins(void)
{
    io_register ret = 0;
    for (GPIO_Pin_Number_e i = pin_num_0; i < MAX_GPIO_PINS; i++)
    {
        ret |= pinToMODER_Out(i);
    }
    return ret;
}

static io_register posToBits(GPIO_Pin_Number_e pin)
{
    return (0x1U << pin);
}

static void assertOnlyTheseBitsHigh(const io_register mask, const io_register reg)
{
    TEST_ASSERT_BITS_HIGH(mask, reg);
    assertBitsAreLOW(~mask, reg);
}

static void assertBitsAreLOW(const io_register mask, const io_register reg)
{
    TEST_ASSERT_BITS_LOW(mask, reg);
}

static void enablePeripheralClocks(void)
{
    static int enabled = 0;
    if (!enabled)
    {
        enabled = 1;
        MyRCC_GPIOClockEnable(&(RCC->AHB1ENR), GPIO_PORT_C_e);
        MyRCC_GPIOClockEnable(&(RCC->AHB1ENR), GPIO_PORT_D_e);
    }
}

#endif // TEST

/*

Functional requirements

// Output
- [x] Output mode configured MODER correctly
- [x] Init can take in mask for 1 or more pins
- [x] Init with mask outside pin max returns null pointer
- [x] Init can be called multiple times and retains integrity of MODER
- [ ] (later) push/pull or open drain may be enabled (out type)
- [ ] Multiple ports can be initialised as output and behave correctly
- [x] Writing to an uninitialized pin returns uninitialised error and does nothing to ODR

- [x] Test inits clock
- [ ] GPIO has 4 modes, in, out, alternate and analog

// Input
- [ ] In, when configured as pull/up, means IDR values are 1 at default or 0 if configured as pull/down
- [ ] Input can be queried
- [ ] If in, will support IRQs too

// Generic
- [ ] GPIO close will reset all its configured GPIO settings for that port
- [x] Initialisation takes GPIOx register struct, GPIO pin mask and direction mask.
- [ ] Newly initilialised output pins are low to begin with
- [ ] Port mode can be changed which changes MODER correctly
- [ ] All ports can be configured (port A - F?)
- [ ] All pins may be configured (pin 0 - 15)
- [ ] Is able to support further GPIO modes
- [ ] Multiple pins may be initialised for that one port
- [ ] Generic error codes shall be returned

*/
