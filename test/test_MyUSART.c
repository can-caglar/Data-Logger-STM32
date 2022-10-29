#include "unity.h"
#include "MyRCC.h"
#include "MyUSART.h"
#include "MyGPIO.h"
#include "user_stm32f407xx.h"
#include "MyBitStuff.h"
#include "MyCommon.h"
#include <string.h>
#include <stdio.h>

MyGPIO UARTGPIO;

static void TXE(USART_TypeDef* usart, unsigned char val);
static void RXNE(USART_TypeDef* usart, unsigned char val);
static void testUsartWrite(USART_TypeDef* usart, unsigned char txeVal);
static void testUsartRead(USART_TypeDef* usart, unsigned char rxneVal);
static inline void WAIT_FOR(USART_TypeDef* usart, io_register mask);
static void doLoopbackTest(unsigned char transmitByte);

enum
{
    NOT_READY,
    READY,
};

void setUp(void)
{
    SystemCoreClockUpdate(); // will set clock to 16Mhz

    // enable USART and GPIO clocks
    MyRCC_USARTClockEnable(RCC_USART1_EN_REG, MY_USART_UART_RCC_MASK);
    MyRCC_GPIOClockEnable(RCC_GPIO_EN_REG, GPIO_PORT_C_e);

    // Configure GPIO pins used in USART
    UARTGPIO.gpio_register = MY_USART_GPIO;
    UARTGPIO.mode = GPIO_ALT;
    UARTGPIO.alt_func = MY_USART_ALT;
    UARTGPIO.output_type = GPIO_PUSH_PULL;
    UARTGPIO.pupd = GPIO_PUPD_UP;
    UARTGPIO.pin_mask = (MY_USART_TX | MY_USART_RX);
    TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&UARTGPIO));
}

void tearDown(void)
{
    memset(USART1, 0, sizeof(*USART1));
}

void test_UsartInit(void)
{
    // Put registers in some state so we know they are really being changed by code under test
    CLR_BIT(USART_CR1_UE_Pos, USART1->CR1);
    SET_BIT(USART_CR1_M_Pos, USART1->CR1);
    CONF_BITS(USART_CR2_STOP_Msk, USART1->CR2, 0x3UL << 12);
    USART1->BRR = 0;
    CLR_BIT(USART_CR1_TE_Pos, USART1->CR1);

    TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Init(USART1, USART_BR_19200));

    TEST_ASSERT_BIT_HIGH(USART_CR1_UE_Pos, USART1->CR1);  // UE bit high
    TEST_ASSERT_BIT_LOW(USART_CR1_M_Pos, USART1->CR1);   // M bit low (8 data bits)
    TEST_ASSERT_BITS_LOW(USART_CR2_STOP_Msk, USART1->CR2);  // stop bits 0
    TEST_ASSERT_EQUAL_HEX(0x341, USART1->BRR);  // 19200 BR at 16MHz
    TEST_ASSERT_BIT_HIGH(USART_CR1_TE_Pos, USART1->CR1);  // send idle frame, enable transmitter
    TEST_ASSERT_BIT_HIGH(USART_CR1_RE_Pos, USART1->CR1);  // receive enable
}

void test_UsartWriteReturnsSucessIfTXEReady(void)
{
    TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Init(USART1, USART_BR_19200));

    testUsartWrite(USART1, READY);
}

void test_UsartWriteReturnsErrorIfTXENotReady(void)
{
    TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Init(USART1, USART_BR_19200));

    testUsartWrite(USART1, NOT_READY);
}

void test_UsartReadReturnsSuccessIfRXNE(void)
{
    TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Init(USART1, USART_BR_19200));
    testUsartRead(USART1, READY);
}

void test_UsartReadRetursBadParamIfNullptrPassed(void)
{
    TEST_ASSERT_EQUAL(ECODE_BAD_PARAM, MyUSART_Read(USART1, 0));
}

// TODO, add test to ensure we only read back an unsigned byte?

void test_UsartReadReturnsErrorIfRXNENotReady(void)
{
    TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Init(USART1, USART_BR_19200));
    testUsartRead(USART1, NOT_READY);
}

void test_LoopBack(void)
{
    TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Init(USART1, USART_BR_19200));

    int count = 10000;
    while (count--);    // hw needs this delay. not sure why!
    doLoopbackTest('C');
    doLoopbackTest(255);
}

void test_WriteString(void)
{
    
}

#if 0
void test_UsartFunctionsShouldReturnErrorIfNotInitialised(void)
{
    // Let TXE be ready
    TXE(USART1, 1);
    TEST_ASSERT_EQUAL(ECODE_NOT_INITIALISED, MyUSART_Write(USART1, 'A'));
}
#endif


/*************************** Helpers **********************/


// Enables or disables the TXE bit in USART SR
// val 0 means not ready, anything else is ready.
// Does nothing on target
static void TXE(USART_TypeDef* usart, unsigned char val)
{
    val = (val == 0) ? 0 : USART_ISR_TXE;
    CONF_BITS(USART_ISR_TXE, usart->ISR, val);
}

// Enables or disables the RXNE bit in USART SR
// val 0 means not ready, anything else is ready.
// Ends up clearing on target even if we write 1?
static void RXNE(USART_TypeDef* usart, unsigned char val)
{
    val = (val == 0) ? 0 : USART_ISR_RXNE;
    CONF_BITS(USART_ISR_RXNE, usart->ISR, val);
}

// Helps void fails in hardware tests by checking the value of TXE before making decision
static void testUsartWrite(USART_TypeDef* usart, unsigned char txeVal)
{
    // set or reset TXE
    TXE(USART1, txeVal);    // TODO, change USART1 -> usart 

    // checking TXE as on hardware this register is readonly,
    // but on dev pc it will be whatever we set passed in
    if (USART1->ISR & USART_ISR_TXE)
    {
        // TXE ready
        TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Write(USART1, 0));
    }
    else
    {
        // TXE not ready
        TEST_ASSERT_EQUAL(ECODE_NOT_READY, MyUSART_Write(USART1, 0));
    }
}

static void testUsartRead(USART_TypeDef* usart, unsigned char rxneVal)
{
    // set or reset RXNE
    RXNE(usart, rxneVal);

    unsigned char output;

    // checking RXNE as on hardware this register is readonly,
    // but on dev pc it will be whatever we set passed in
    if (usart->ISR & USART_ISR_RXNE)
    {
        // RXNE ready
        TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Read(usart, &output));
    }
    else
    {
        // RXNE not ready
        TEST_ASSERT_EQUAL(ECODE_NOT_READY, MyUSART_Read(usart, &output));
    }
}

static inline void WAIT_FOR(USART_TypeDef* usart, io_register mask)
{
    uint32_t countdown = 200000;
    while (!(usart->ISR & mask))
    {
        if (countdown-- <= 0)
        {
            TEST_FAIL_MESSAGE("Failed waiting for usart->ISR mask!");
        }
    }
}

static void doLoopbackTest(unsigned char transmitByte)
{
    Error_Code_e err = (Error_Code_e)0xFF;
    unsigned char receivedByte = ~transmitByte;

    // when testing on hardware, requires the RX/TX lines
    // to be wired together

    TXE(USART1, READY);
    WAIT_FOR(USART1, USART_ISR_TXE);  // only waits on target

    err =  MyUSART_Write(USART1, transmitByte);
    TEST_ASSERT_EQUAL(ECODE_OK, err);

    RXNE(USART1, READY);
    WAIT_FOR(USART1, USART_ISR_RXNE);  // only waits on target

    err = MyUSART_Read(USART1, &receivedByte);
    TEST_ASSERT_EQUAL(ECODE_OK, err);
    TEST_ASSERT_EQUAL(transmitByte, receivedByte);  // the loopback test
}

/*
- [ ] Word length can be set with M bit in USART_CR1 (set = 9 bit word, reset = 8 bit word)
- [ ] Transmission and reception are driven by common baud rate generator. Clock for each generated when enable bit set for xmitter and receiver.
- [ ] Data shifts out LSB firt on the TX pin from USART_DR
- [ ] The TE bit shall not be reset during transmission of data, or corruption may occur
- [ ] USART_CR2 may configure stop bits, 1, 2, 0.5 or 1.5 stop bits.
- [ ] it is mandatory to wait for TC=1 before disabling the USART

Requirements:
- [ ] Word size is fixed at 8
- [ ] Only two USARTs allowed for now, USART1 and USART1?
- [ ] No DMA
- [ ] Baud rate can be configured
- [ ] Always over sampling 16 (0)

*/
