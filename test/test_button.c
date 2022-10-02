#include "unity.h"
#include "button.h"
#include "mock_MyInterrupts.h"
#include "mock_system_hal.h"
#include "unity_helper.h"

#include "mock_stm32f4xx_hal_rcc_ex.h"
#include "mock_stm32f4xx_hal_gpio.h"
#include "mock_stm32f4xx_hal_cortex.h"

extern void button_irq(void);

// helpers
static void fake_press_button(void);

static int testVar = 0;
void testFunc(void)
{
    testVar = 1;
}

void tearDown(void)
{
    // unity teardown
    button_close();
}

void test_buttonInit(void)
{
    GPIO_InitTypeDef gpio = 
    {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_NOPULL,
    };

    MY_HAL_RCC_GPIOA_CLK_ENABLE_Expect();
    
    HAL_GPIO_Init_Expect(GPIOA, &gpio);

    gpio_register_interrupt_callback_Expect(GPIO_PIN_0, button_irq);
    
    HAL_NVIC_EnableIRQ_Expect(EXTI0_IRQn);

    button_init();

    TEST_ASSERT_EQUAL_INT(0, button_pressed());
}

void test_button_pressed_debounce(void)
{
    testVar = 0;
    button_register_observer(testFunc);

    get_tick_ExpectAndReturn(0);
    button_irq();
    TEST_ASSERT_EQUAL_INT(0, button_pressed());

    // too soon, shall do nothing
    get_tick_ExpectAndReturn(10);
    button_irq();
    TEST_ASSERT_EQUAL_INT(0, button_pressed());

    // 50 ms later, press shall go through
    TEST_ASSERT_EQUAL_INT(0, testVar);
    get_tick_ExpectAndReturn(60);
    HAL_GPIO_ReadPin_ExpectAndReturn(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    // gpio_read_ExpectAndReturn(GH_PORT_A, GH_PIN_0, 1);
    button_irq();
    TEST_ASSERT_EQUAL_INT(1, button_pressed());
    TEST_ASSERT_EQUAL_INT(1, testVar);

    // 10 ms later
    get_tick_ExpectAndReturn(70);
    button_irq();
    TEST_ASSERT_EQUAL_INT(1, button_pressed());
}


void test_button_pressed_callback(void)
{
    testVar = 0;
    button_register_observer(testFunc);

    TEST_ASSERT_EQUAL_INT(0, testVar);

    fake_press_button();
    
    TEST_ASSERT_EQUAL_INT(1, testVar);
}

void fake_press_button(void)
{
    get_tick_ExpectAndReturn(0);
    button_irq();
    TEST_ASSERT_EQUAL_INT(0, button_pressed());

    get_tick_ExpectAndReturn(50);
    HAL_GPIO_ReadPin_ExpectAndReturn(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    button_irq();
    TEST_ASSERT_EQUAL_INT(1, button_pressed());
}

/*

- [ ] Button module shall expose: init, read
- [ ] irq function will change flag state depending on debounce
- [ ] read will just return a flag
*/