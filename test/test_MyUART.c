#include "unity.h"
#include "MyRCC.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Hello(void)
{
    TEST_IGNORE_MESSAGE("Ignoring uart tests for now");
    //TEST_FAIL_MESSAGE("UART FAILED");
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
- [ ] GPIO pins are fixed

*/