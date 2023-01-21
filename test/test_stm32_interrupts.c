#include "unity.h"
#include "stm32f0xx_it.h"
#include "exploding_fakes.h"
#include "mock_stm32f0xx_hal_uart.h"
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

UART_HandleTypeDef huart1; // fake huart1
const uint8_t fakeUartBuf = 8; // fake UART buffer

void test_USART1_IRQHandler_normal_operation(void)
{
    callWhenCircularBufferIs(NOT_FULL);

    USART1_IRQHandler();
}

void test_USART1_IRQHandler_when_buffer_full_indicates_error(void)
{
    callWhenCircularBufferIs(FULL);

    USART1_IRQHandler();
}

static void callWhenCircularBufferIs(CircularBufferState state)
{
    huart1.pRxBuffPtr = &fakeUartBuf;
    huart1.pRxBuffPtr++;    // simulating the HAL which increments addr

    HAL_UART_IRQHandler_Expect(&huart1);
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
    HAL_UART_Receive_IT_ExpectAndReturn(&huart1,
        &fakeUartBuf, 1, 0);
}