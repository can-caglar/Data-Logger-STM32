#include "unity.h"
#include "MyRCC.h"
#include "MyUSART.h"
#include "MyGPIO.h"
#include "user_stm32f407xx.h"
#include "MyBitStuff.h"

MyGPIO UARTGPIO;

void setUp(void)
{
    MyRCC_USARTClockEnable(RCC_USART6_EN_REG, USART6_Mask);
    MyRCC_GPIOClockEnable(RCC_GPIO_EN_REG, GPIO_PORT_C_e);

    // UARTGPIO.gpio_register = GPIOC;
    // UARTGPIO.mode = GPIO_ALT;
    // UARTGPIO.alt_func = GPIO_ALTF_7;
    // UARTGPIO.pin_mask = (pin_mask_6 | pin_mask_7 | pin_mask_8);
    // TEST_ASSERT_EQUAL(ECODE_OK, MyGPIO_Init(&UARTGPIO));
}

void tearDown(void)
{
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

// Continue: [x] go back to GPIO module and add alternate function ability on there.
// [...] then on this module, initialise the GPIO pins associated with the USART
// ...and continue to initialise USART!

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
