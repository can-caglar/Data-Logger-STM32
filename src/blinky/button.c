#include "button.h"
#include "nvic_hal.h"
#include "MyInterrupts.h"
#include "system_hal.h"
#include "global.h"

#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal.h"

// Dev board specific parameters
static const uint16_t _btnPin = GPIO_PIN_0;
static GPIO_TypeDef* const _btnPort = GPIOA;
static GPIO_InitTypeDef _btnGpio = 
{
    .Pin = _btnPin,
    .Mode = GPIO_MODE_IT_RISING,
    .Pull = GPIO_NOPULL
};

// Debounce vars
static const uint32_t debounceTime = 50; // ms
static uint32_t lastIrq = 0;
static int buttonPressed = 0;

static obs_func observer; // just 1 observer for now

STATIC void button_irq(void)
{
    uint32_t t_now = get_tick();
    if (t_now - lastIrq >= debounceTime)
    {
        buttonPressed = HAL_GPIO_ReadPin(_btnPort, GPIO_PIN_0); // gpio_read(_port, _gpio.pin);
        observer();
    }
    lastIrq = t_now;
}

void button_init(void)
{
    lastIrq = 0;
    buttonPressed = 0;
    observer = 0;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_Init(GPIOA, &_btnGpio);
    gpio_register_interrupt_callback(_btnPin, button_irq);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void button_close(void)
{
    lastIrq = 0;
    buttonPressed = 0;
    observer = 0;
}

int button_pressed(void)
{
    return buttonPressed;
}

void button_register_observer(obs_func func)
{
    observer = func;
}
