#include "unity.h"
#include "stm32f3xx_it.h"
#include "exploding_fakes.h"
#include "mock_stm32f3xx_hal_uart.h"
#include "mock_MyCircularBuffer.h"
#include "mock_MyTimeString.h"
#include "mock_ErrorIndicator.h"
#include <stdint.h>

typedef enum
{
    NOT_FULL,
    FULL
} CircularBufferState;

static void callWhenCircularBufferIs(CircularBufferState state);

UART_HandleTypeDef huartIT; // fake huartIT
const uint8_t fakeUartBuf = 8; // fake UART buffer

void test_USART2_IRQHandler_normal_operation(void)
{
    callWhenCircularBufferIs(NOT_FULL);

    USART2_IRQHandler();
}

void test_USART2_IRQHandler_when_buffer_full_indicates_error(void)
{
    callWhenCircularBufferIs(FULL);

    USART2_IRQHandler();
}

static void callWhenCircularBufferIs(CircularBufferState state)
{
    huartIT.pRxBuffPtr = &fakeUartBuf;
    huartIT.pRxBuffPtr++;    // simulating the HAL which increments addr

    HAL_UART_IRQHandler_Expect(&huartIT);
    MyCircularBuffer_init_Expect();

    if (state == NOT_FULL)
    {
        MyCircularBuffer_write_ExpectAndReturn(fakeUartBuf, 0);
    }
    else
    {
        // turn on led if overflowed
        MyCircularBuffer_write_ExpectAndReturn(fakeUartBuf, 1);
        ErrorIndicator_Indicate_Expect();
    }
    // Expect the same buffer address to be written to for next time
    HAL_UART_Receive_IT_ExpectAndReturn(&huartIT,
        &fakeUartBuf, 1, 0);
}