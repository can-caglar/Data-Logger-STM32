#ifdef TEST

#include <stdint.h>
#include <string.h>
#include "unity.h"
#include "MyGPIO.h"
#include "MyRCC.h"
#include "MyCommon.h"
#include "MyBitStuff.h"
#include "user_stm32f407xx.h"

// Helper macros
#define MY_SIZE_OF_ARR(x) ((sizeof(x) / sizeof(x[0])))

// Forward declarations for test helper functions
static io_register MODERfromPinAndMode_Out(GPIO_Pin_Number_e pin);
static io_register MODERfromPinAndMode(GPIO_Pin_Number_e pin, GPIO_Mode_e mode);
static io_register posToBits(GPIO_Pin_Number_e pin);
static void assertOnlyTheseBitsHigh(const io_register mask, const io_register reg);
static void assertBitsAreLOW(const io_register mask, const io_register reg);
static io_register MODERfromPinAndMode_OutAllPins(void);

MyGPIO testGPIO;

void setUp(void)
{
    memset(GPIOA, 0, sizeof *GPIOB);
    memset(GPIOB, 0, sizeof *GPIOA);
    memset(&testGPIO, 0, sizeof(testGPIO));
}

void tearDown(void)
{
}

void test_MyGPIO_ConstantsAreCorrect(void)
{
    TEST_ASSERT_EQUAL_HEX(0x0, GPIO_INPUT);
    TEST_ASSERT_EQUAL_HEX(0x1, GPIO_OUTPUT);

    TEST_ASSERT_EQUAL_INT(16, MAX_GPIO_PINS);
    TEST_ASSERT_EQUAL_INT(16, MAX_ALT_FUNCTIONS);

    GPIO_Pin_Mask_t pins[MAX_GPIO_PINS] =
    {
        pin0_mask, pin1_mask, pin2_mask, pin3_mask,
        pin4_mask, pin5_mask, pin6_mask, pin7_mask,
        pin8_mask, pin9_mask, pin10_mask, pin11_mask,
        pin12_mask, pin13_mask, pin14_mask, pin15_mask,
    };

    for (int i = 0; i < MAX_GPIO_PINS; i++)
    {
        TEST_ASSERT_EQUAL_HEX(1UL << i, pins[i]);
    }

    TEST_ASSERT_EQUAL_HEX(pins_all_mask, 0xFFFF);

    TEST_ASSERT_EQUAL(0, GPIO_INPUT);
    TEST_ASSERT_EQUAL(1, GPIO_OUTPUT);
    TEST_ASSERT_EQUAL(2, GPIO_ALT);
    TEST_ASSERT_EQUAL(3, GPIO_MODE_MASK);

    /* GPIOx_OTYPER */
    TEST_ASSERT_EQUAL(0, GPIO_PUSH_PULL);
    TEST_ASSERT_EQUAL(1, GPIO_OPEN_DRAIN);

    /* GPIOx_PUPDR */
    TEST_ASSERT_EQUAL(0, GPIO_PUPD_NONE);
    TEST_ASSERT_EQUAL(1, GPIO_PUPD_UP);
    TEST_ASSERT_EQUAL(2, GPIO_PUPD_DOWN);
    TEST_ASSERT_EQUAL(3, GPIO_PUPD_MASK);

}

void test_MyGPIO_OutputIsInitialisedCorrectlyWhen1PinPassedIn(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin1_mask;
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO);

    assertOnlyTheseBitsHigh(MODERfromPinAndMode_Out(pin_num_1), GPIOB->MODER);
}

void test_MyGPIO_OutputIsInitialisedCorrectlyWhenMultiplePinsPassedIn(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = (GPIO_Pin_Mask_t)(pin2_mask | pin3_mask | pin4_mask);
    testGPIO.mode = GPIO_OUTPUT;

    MyGPIO_Init(&testGPIO);

    io_register expected = MODERfromPinAndMode_Out(pin_num_2)
        | MODERfromPinAndMode_Out(pin_num_3)
        | MODERfromPinAndMode_Out(pin_num_4);

    assertOnlyTheseBitsHigh(expected, GPIOB->MODER);
}

void test_MyGPIO_OutputIsInitialisedCorrectlyWhenAllPinsPassedInSameTime(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pins_all_mask;
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO);

    io_register expected = MODERfromPinAndMode_OutAllPins();

    assertOnlyTheseBitsHigh(expected, GPIOB->MODER);
}
void test_MyGPIO_WritingToUninitialisedPortDoesNothing(void)
{
    TEST_ASSERT_EQUAL_INT(ECODE_NOT_OUTPUT, MyGPIO_Write(GPIOB, pin4_mask, GPIO_HIGH));
    assertOnlyTheseBitsHigh(0, GPIOB->ODR);
}

void test_MyGPIO_SinglePinCanBeSetHigh(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin4_mask;
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO);

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pin4_mask, GPIO_HIGH));

    assertOnlyTheseBitsHigh(posToBits(pin_num_4), GPIOB->ODR);
}

void test_MyGPIO_OutputMultiplePinsPassedInCanBeSetHigh(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = (GPIO_Pin_Mask_t)(pin6_mask | pin7_mask);
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO);

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, (GPIO_Pin_Mask_t)(pin6_mask | pin7_mask), GPIO_HIGH));

    io_register expected = posToBits(pin_num_6) | posToBits(pin_num_7);
    assertOnlyTheseBitsHigh(expected, GPIOB->ODR);
}

void test_MyGPIO_OutputCanBeSetHighWhenCalledMultipleTimes(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = (GPIO_Pin_Mask_t)(pin6_mask | pin7_mask | pin8_mask);
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO);

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pin6_mask, GPIO_HIGH));
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pin7_mask, GPIO_HIGH));
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pin8_mask, GPIO_HIGH));

    io_register expected = posToBits(pin_num_6) | posToBits(pin_num_7) |  posToBits(pin_num_8);
    assertOnlyTheseBitsHigh(expected, GPIOB->ODR);
}

void test_MyGPIO_OutputCanBeSetLow_1Pin(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pins_all_mask;
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO);
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pins_all_mask, GPIO_HIGH));

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pin8_mask, GPIO_LOW));

    assertBitsAreLOW(posToBits(pin_num_8), GPIOB->ODR);
}

void test_MyGPIO_InitPinMaskIsSizeIs2Bytes(void)
{
    TEST_ASSERT_EQUAL(2, sizeof(GPIO_Pin_Mask_t));
}

void test_MyGPIO_OutputCanBeSetLowWhenCalledMultipleTimesWithDifferentPins(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pins_all_mask;
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO);
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pins_all_mask, GPIO_HIGH));

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pin6_mask, GPIO_LOW));
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, pin8_mask, GPIO_LOW));

    io_register expected = 0xFFFF & ~(posToBits(pin_num_6) | posToBits(pin_num_8));
    assertOnlyTheseBitsHigh(expected, GPIOB->ODR);
}

void test_MyGPIO_MultiplePortsMayBeInitialisedAsOutput(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pins_all_mask;
    testGPIO.mode = GPIO_OUTPUT;

    MyGPIO_Init(&testGPIO);

    testGPIO.gpio_register = GPIOA;
    MyGPIO_Init(&testGPIO);

    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOB, (GPIO_Pin_Mask_t)(pin1_mask | pin2_mask), GPIO_HIGH));
    TEST_ASSERT_EQUAL_INT(ECODE_OK, MyGPIO_Write(GPIOA, (GPIO_Pin_Mask_t)(pin1_mask | pin3_mask), GPIO_HIGH));

    assertOnlyTheseBitsHigh(posToBits(pin_num_1) | posToBits(pin_num_2), GPIOB->ODR);
    assertOnlyTheseBitsHigh(posToBits(pin_num_1) | posToBits(pin_num_3), GPIOA->ODR);
}

void test_MyGPIO_InitialiseAsInputMakesRegisterValue00(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pins_all_mask;
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO); // to see if changes make effect

    testGPIO.mode = GPIO_INPUT;
    testGPIO.pin_mask = pin1_mask;
    MyGPIO_Init(&testGPIO);

    io_register expected = MODERfromPinAndMode_OutAllPins();
    CONF_BITS(GPIO_MODE_MASK << pin_num_1, expected, GPIO_INPUT << pin_num_1);
    TEST_ASSERT_BITS(ALL_32_BITS, GPIOB->MODER, expected);
}

void test_MyGPIO_InputIsReadStraightFromIDRRegisterForHighSignal(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin3_mask;
    testGPIO.mode = GPIO_INPUT;
    GPIOB->IDR = (0x1U << pin_num_3);   // set it to some value (this won't do anything on target but doesnt matter)

    MyGPIO_Init(&testGPIO);
    io_register idr = (GPIOB->IDR & (0x1U << pin_num_3));   // read IDR back

    GPIO_State_e expectedState = (idr == 0) ? GPIO_LOW : GPIO_HIGH; // expected output to be whatever IDR is

    TEST_ASSERT_BITS(FIRST_16_BITS, expectedState, MyGPIO_Read(GPIOB, pin_num_3));
}

void test_MyGPIO_InputIsReadStraightFromIDRRegisterForLowSignal(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin1_mask;
    testGPIO.mode = GPIO_INPUT;
    GPIOB->IDR &= ~(posToBits(pin_num_1));

    MyGPIO_Init(&testGPIO);
    io_register idr = (GPIOB->IDR & (0x1U << pin_num_1));   // read IDR back
    GPIO_State_e expectedState = (idr == 0) ? GPIO_LOW : GPIO_HIGH;

    TEST_ASSERT_EQUAL(expectedState, MyGPIO_Read(GPIOB, pin_num_1));
}

void test_MyGPIO_ReadingFromPortNotInitialisedAsInputReturns0(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin2_mask;
    testGPIO.mode = GPIO_OUTPUT;
    MyGPIO_Init(&testGPIO);
    GPIOB->IDR = ~0U;

    TEST_ASSERT_EQUAL(GPIO_LOW, MyGPIO_Read(GPIOB, pin_num_2));
}

void test_MyGPIO_InitAsAlternateFunction_Pins0to7(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = (GPIO_Pin_Mask_t)
        (pin0_mask | pin1_mask | pin2_mask | pin3_mask | pin4_mask | pin5_mask | pin6_mask | pin7_mask);
    testGPIO.mode = GPIO_ALT;
    testGPIO.alt_func = GPIO_ALTF_3;

    io_register expected = MODERfromPinAndMode(pin_num_0, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_1, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_2, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_3, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_4, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_5, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_6, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_7, GPIO_ALT);

    GPIOB->AFR[AFR_LOW] = ~0;  // to make sure things really change

    MyGPIO_Init(&testGPIO);

    TEST_ASSERT_EQUAL_HEX(expected, testGPIO.gpio_register->MODER);
    TEST_ASSERT_EQUAL_HEX(0x33333333, testGPIO.gpio_register->AFR[AFR_LOW]);
    TEST_ASSERT_EQUAL_HEX(0, testGPIO.gpio_register->AFR[AFR_HIGH]);
}

void test_MyGPIO_InitAsAlternateFunction_Pins8to15(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = (GPIO_Pin_Mask_t)
        (pin8_mask | pin9_mask | pin10_mask | pin11_mask | pin12_mask | pin13_mask | pin14_mask | pin15_mask);
    testGPIO.mode = GPIO_ALT;
    testGPIO.alt_func = GPIO_ALTF_5;

    io_register expected = MODERfromPinAndMode(pin_num_8, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_9, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_10, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_11, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_12, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_13, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_14, GPIO_ALT)
        |  MODERfromPinAndMode(pin_num_15, GPIO_ALT);

    GPIOB->AFR[AFR_HIGH] = 0xABCDEF;  // to make sure things really change
    GPIOB->AFR[AFR_LOW] = 0xABCDEF;  // to make sure things haven't changed

    MyGPIO_Init(&testGPIO);

    TEST_ASSERT_EQUAL_HEX(expected, testGPIO.gpio_register->MODER);
    TEST_ASSERT_EQUAL_HEX(0xABCDEF, testGPIO.gpio_register->AFR[AFR_LOW]);
    TEST_ASSERT_EQUAL_HEX(0x55555555, testGPIO.gpio_register->AFR[AFR_HIGH]);
}

void test_MyGPIO_AltFunctionInitAboveLimitDoesNothingAndReturnsError(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin2_mask;
    testGPIO.mode = GPIO_ALT;
    testGPIO.alt_func = MAX_ALT_FUNCTIONS;

    io_register expected = 0x55555555;
    GPIOB->AFR[AFR_LOW] = expected;
    GPIOB->AFR[AFR_HIGH] = expected;

    TEST_ASSERT_EQUAL(ECODE_BAD_PARAM, MyGPIO_Init(&testGPIO));

    TEST_ASSERT_EQUAL_HEX(expected, testGPIO.gpio_register->AFR[AFR_LOW]);
    TEST_ASSERT_EQUAL_HEX(expected, testGPIO.gpio_register->AFR[AFR_HIGH]);
}

void test_MyGPIO_outputTypeCanBeConfiguredAsPushPull(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin5_mask;
    testGPIO.mode = GPIO_OUTPUT;

    testGPIO.output_type = GPIO_PUSH_PULL;

    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&testGPIO));

    TEST_ASSERT_BIT_LOW(pin_num_5, testGPIO.gpio_register->OTYPER);
}

void test_MyGPIO_outputTypeCanBeConfiguredAsOpenDrainForOnePin(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin7_mask;
    testGPIO.mode = GPIO_OUTPUT;

    testGPIO.output_type = GPIO_OPEN_DRAIN;

    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&testGPIO));

    TEST_ASSERT_EQUAL_HEX((1UL << pin_num_7), testGPIO.gpio_register->OTYPER);
}

void test_MyGPIO_outputTypeCanBeConfiguredAsOpenDrainForMultiplePins(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = (pin9_mask | pin10_mask);
    testGPIO.mode = GPIO_OUTPUT;

    testGPIO.output_type = GPIO_OPEN_DRAIN;

    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&testGPIO));

    TEST_ASSERT_EQUAL_HEX((1UL << 9) | (1UL << 10), testGPIO.gpio_register->OTYPER);
}

void test_MyGPIO_outputTypeCanBeSetAsOpenDrainAndWontMessUpPreviousConfig(void)
{
    testGPIO.gpio_register = GPIOB; // both GPIOB
    testGPIO.pin_mask = pin9_mask;  // pin 9 for this
    testGPIO.mode = GPIO_OUTPUT;
    testGPIO.output_type = GPIO_OPEN_DRAIN;

    MyGPIO secondGpio =
    {
        .gpio_register = GPIOB,  // both GPIOB
        .pin_mask = pin3_mask,  // pin 3 for this
        .mode = GPIO_OUTPUT,
        .output_type = GPIO_OPEN_DRAIN
    };

    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&testGPIO));
    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&secondGpio));

    TEST_ASSERT_EQUAL_HEX((1UL << 3) | (1UL << 9), GPIOB->OTYPER);
}

void test_MyGPIO_canBeConfiguredAsPullupWithoutAffectingOthers(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin2_mask | pin3_mask;
    testGPIO.mode = GPIO_INPUT;

    testGPIO.pupd = GPIO_PUPD_UP;
    GPIOB->PUPDR = ~0;
    io_register expected = ~((3 << (2 * pin_num_2)) | (3 << (2 * pin_num_3)));
    expected |=  (1 << (2 * pin_num_2)) | (1 << (2 * pin_num_3));

    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&testGPIO));

    TEST_ASSERT_EQUAL_HEX(expected, GPIOB->PUPDR);
}

void test_MyGPIO_canBeConfiguredAsPulldownWithoutAffectingOthers(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin7_mask | pin15_mask;
    testGPIO.mode = GPIO_OUTPUT;

    testGPIO.pupd = GPIO_PUPD_DOWN;

    GPIOB->PUPDR = ~0;
    io_register expected = ~((3 << (2 * pin_num_7)) | (3 << (2 * pin_num_15)));
    expected |=  (0x2 << (2 * pin_num_7)) | (0x2 << (2 * pin_num_15));

    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&testGPIO));

    TEST_ASSERT_EQUAL_HEX(expected, GPIOB->PUPDR);
}

void test_MyGPIO_canBeConfiguredAsNoPullUpOrPulldown(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin1_mask;
    testGPIO.mode = GPIO_OUTPUT;

    testGPIO.pupd = GPIO_PUPD_NONE;

    GPIOB->PUPDR = ~0;
    io_register expected = ~(0x3 << (2 * pin_num_1));

    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&testGPIO));

    TEST_ASSERT_EQUAL_HEX(expected, GPIOB->PUPDR);
}

void test_MyGPIO_outputAsPushPull(void)
{
    testGPIO.gpio_register = GPIOB;
    testGPIO.pin_mask = pin10_mask;
    testGPIO.mode = GPIO_OUTPUT;
    testGPIO.output_type = GPIO_PUSH_PULL;

    GPIOB->OTYPER = ~0;
    io_register expected = ~(1 << pin_num_10);

    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&testGPIO));

    TEST_ASSERT_EQUAL_HEX(expected, GPIOB->OTYPER);
}

// TODO, what happens if a reserved value is sent for output type config
// TODO, gpio config can be changed and values are as expected

/************************* Private functions *********************/

static io_register MODERfromPinAndMode_Out(GPIO_Pin_Number_e pin)
{
    return MODERfromPinAndMode(pin, GPIO_OUTPUT);
}

static io_register MODERfromPinAndMode(GPIO_Pin_Number_e pin, GPIO_Mode_e mode)
{
    return (mode << (pin * 2));
}

static io_register MODERfromPinAndMode_OutAllPins(void)
{
    io_register ret = 0;
    for (GPIO_Pin_Number_e i = pin_num_0; i < MAX_GPIO_PINS; i++)
    {
        ret |= MODERfromPinAndMode_Out(i);
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
