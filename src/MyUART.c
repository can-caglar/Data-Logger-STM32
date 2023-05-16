#include "MyUART.h"
#include "stm32f3xx_hal.h"

UART_HandleTypeDef huartIT;
static uint8_t configured = 0;
static uint8_t uartRecvBuf = 0;

extern void Error_Handler(void);

void MyUART_init(MyUART_Config* config)
{
    if (!configured)
    {
        // STM32F3 Nucleo uses USART2
        huartIT.Instance = USART2;
        huartIT.Init.BaudRate = config->baudrate;
        huartIT.Init.WordLength = UART_WORDLENGTH_8B;
        huartIT.Init.StopBits = UART_STOPBITS_1;
        huartIT.Init.Parity = UART_PARITY_NONE;
        huartIT.Init.Mode = UART_MODE_TX_RX;
        huartIT.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huartIT.Init.OverSampling = UART_OVERSAMPLING_16;
        huartIT.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huartIT.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huartIT) != HAL_OK)
        {
            Error_Handler();
        }

        HAL_UART_Receive_IT(&huartIT, &uartRecvBuf, 1);
        
        configured = 1;
    }
}
