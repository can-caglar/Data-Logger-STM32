#include "unity.h"
#include "MyRCC.h"
#include "MyUSART.h"
#include "user_stm32f407xx.h"

#define SET_BIT(bit, reg)   ((reg) |= (1UL << (bit)))
#define CONF_BITS(mask, reg, new) ((reg) &= ~(mask)); ((reg) |= (new))
#define CLR_BIT(bit, reg)   ((reg) &= ~(1UL << (bit)))

void setUp(void)
{
    MyRCC_USARTClockEnable(RCC_USART6_EN_REG, USART6_Mask);
}

void tearDown(void)
{
}

void testBitStuff(void)
{
    io_register myReg = 0;

    SET_BIT(2, myReg);
    TEST_ASSERT_BIT_HIGH(2, myReg);

    CLR_BIT(2, myReg);
    TEST_ASSERT_BIT_LOW(2, myReg);

    CONF_BITS((0x3 << 2), myReg, (0x6 << 2));

    TEST_ASSERT_BITS(0x3 << 2, 0x2 << 2, myReg);

}

void test_UsartInit(void)
{
    SystemCoreClockUpdate(); // will set clock to 16Mhz

    // Put registers in some state so we know they are really being changed by code under test
    CLR_BIT(13, USART6->CR1);
    SET_BIT(12, USART6->CR1);
    CONF_BITS(0x3UL << 12, USART6->CR2, 0x3UL << 12);
    USART6->BRR = 0;
    CLR_BIT(3, USART6->CR1);

    TEST_ASSERT_EQUAL(ECODE_OK, MyUSART_Init(USART6, USART_BR_19200));

    TEST_ASSERT_BIT_HIGH(13, USART6->CR1);  // UE bit high
    TEST_ASSERT_BIT_LOW(12, USART6->CR1);   // M bit low (8 data bits)
    TEST_ASSERT_BITS_LOW(0x3UL << 12, USART6->CR2);  // stop bits 0
    TEST_ASSERT_EQUAL_HEX(0x341, USART6->BRR);  // 19200 BR at 16MHz
    TEST_ASSERT_BIT_HIGH(3, USART6->CR1);  // send idle frame
}

// Continue: go back to GPIO module and add alternate function ability on there.
// ... then on this module, initialise the GPIO pins associated with the USART

/*
- [ ] Word length can be set with M bit in USART_CR1 (set = 9 bit word, reset = 8 bit word)
- [ ] Transmission and reception are driven by common baud rate generator. Clock for each generated when enable bit set for xmitter and receiver.
- [ ] Data shifts out LSB firt on the TX pin from USART_DR
- [ ] The TE bit shall not be reset during transmission of data, or corruption may occur
- [ ] USART_CR2 may configure stop bits, 1, 2, 0.5 or 1.5 stop bits.
- [ ] it is mandatory to wait for TC=1 before disabling the USART

Requirements:
- [ ] Word size is fixed at 8
- [ ] Two USARTs allowed for now, USART6 and USART1?
- [ ] No DMA
- [ ] Baud rate can be configured
- [ ] Always over sampling 16 (0)

*/
