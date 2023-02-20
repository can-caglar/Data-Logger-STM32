#include "unity.h"
#include "unity_helper.h"
#include "mock_stm32f3xx_hal_gpio.h"
#include "mock_stm32f3xx_hal_rcc.h"
#include "MyDipSwitch.h"
#include "MyCommon.h"

static void expectToRead(uint8_t num);

void test_Init(void)
{
    // HAL_GPIO_Init
    GPIO_InitTypeDef gpio = { 0 };
    gpio.Pin = GPIO_PIN_DIP_1 | GPIO_PIN_DIP_2 | GPIO_PIN_DIP_3 | GPIO_PIN_DIP_4;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    gpio.Alternate = 0;

    MY_HAL_RCC_GPIOB_CLK_ENABLE_Expect();
    HAL_GPIO_Init_Expect(GPIO_PORT_DIP, &gpio);

    MyDIP_Init();
}

void test_Read(void)
{
    for (int i = 0; i < 16; i++)
    {
        expectToRead(i);

        uint8_t result = MyDIP_Read();

        TEST_ASSERT_EQUAL_HEX(i, result);
    }
}

// Helper functions
static void expectToRead(uint8_t num)
{
    TEST_ASSERT_LESS_OR_EQUAL(16, num);
    #define shift(val, amount) (((val) >> (amount)) & 0x1)

    HAL_GPIO_ReadPin_ExpectAndReturn(GPIO_PORT_DIP, GPIO_PIN_DIP_1, shift(num, 0));
    HAL_GPIO_ReadPin_ExpectAndReturn(GPIO_PORT_DIP, GPIO_PIN_DIP_2, shift(num, 1));
    HAL_GPIO_ReadPin_ExpectAndReturn(GPIO_PORT_DIP, GPIO_PIN_DIP_3, shift(num, 2));
    HAL_GPIO_ReadPin_ExpectAndReturn(GPIO_PORT_DIP, GPIO_PIN_DIP_4, shift(num, 3));
}
