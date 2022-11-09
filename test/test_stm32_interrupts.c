#include "unity.h"
#include "stm32f0xx_it.h"
#include "exploding_fakes.h"
#include "mock_stm32f0xx_hal_uart.h"
#include "mock_MyCircularBuffer.h"
#include "mock_MyTimeString.h"
#include <stdint.h>

UART_HandleTypeDef huart1; // fake huart1

void test_USART1_IRQHandler_calls_hal(void)
{
    uint8_t fakeUartBuf = 8; // fake UART buffer
    huart1.pRxBuffPtr = &fakeUartBuf;
    HAL_UART_IRQHandler_Expect(&huart1);
    huart1.pRxBuffPtr++;    // simulating the HAL

    MyCircularBuffer_init_Expect();
    MyCircularBuffer_write_Expect(fakeUartBuf);

    HAL_UART_Receive_IT_ExpectAndReturn(&huart1,
        &fakeUartBuf, 1, 0);

    USART1_IRQHandler();
}
