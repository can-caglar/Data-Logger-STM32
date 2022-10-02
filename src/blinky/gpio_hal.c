#include "gpio_hal.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdint.h>

/* Translate from adapter to ST HAL */

static GPIO_TypeDef* _ports[GH_PORT_COUNT] = 
{
    [GH_PORT_A] = GPIOA,
    [GH_PORT_B] = GPIOB,
    [GH_PORT_C] = GPIOC,
    [GH_PORT_D] = GPIOD,
    [GH_PORT_E] = GPIOE,
    [GH_PORT_F] = GPIOF,
};

static uint32_t _pins[GH_PIN_COUNT] = 
{
    [GH_PIN_0] = GPIO_PIN_0,
    [GH_PIN_1] = GPIO_PIN_1,
    [GH_PIN_2] = GPIO_PIN_2,
    [GH_PIN_3] = GPIO_PIN_3,
    [GH_PIN_4] = GPIO_PIN_4,
    [GH_PIN_5] = GPIO_PIN_5,
    [GH_PIN_6] = GPIO_PIN_6,
    [GH_PIN_7] = GPIO_PIN_7,
    [GH_PIN_8] = GPIO_PIN_8,
    [GH_PIN_9] = GPIO_PIN_9,
    [GH_PIN_10] = GPIO_PIN_10,
    [GH_PIN_11] = GPIO_PIN_11,
    [GH_PIN_12] = GPIO_PIN_12,
    [GH_PIN_13] = GPIO_PIN_13,
    [GH_PIN_14] = GPIO_PIN_14,
    [GH_PIN_15] = GPIO_PIN_15,
};

static uint32_t _pull[GH_PULL_COUNT] = 
{
    [GH_PULL_NONE] = GPIO_NOPULL,
    [GH_PULL_UP] = GPIO_PULLUP,
    [GH_PULL_DOWN] = GPIO_PULLDOWN,
};

static uint32_t _modes[GH_MODE_COUNT] = 
{
    [GH_MODE_INPUT] = GPIO_MODE_INPUT,
    [GH_MODE_OUTPUT_PP] = GPIO_MODE_OUTPUT_PP,
    [GH_MODE_OUTPUT_OD] = GPIO_MODE_OUTPUT_OD,
    [GH_MODE_IT_RISING_FALLING] = GPIO_MODE_IT_RISING_FALLING,
    [GH_MODE_IT_RISING] = GPIO_MODE_IT_RISING,
};

static GPIO_PinState _states[GH_STATE_COUNT] = 
{
    [GH_STATE_RESET] = GPIO_PIN_RESET,
    [GH_STATE_SET] = GPIO_PIN_SET
};

static irq_func _callbacks[GH_PIN_COUNT];

/* Public functions */

void gpio_init(GPIOPort_e port, GH_Init_s* gpio)
{
    GPIO_InitTypeDef st_gpio;
    GPIO_TypeDef* st_port = _ports[port];

    st_gpio.Pin = _pins[gpio->pin];
    st_gpio.Mode = _modes[gpio->mode];
    st_gpio.Pull = _pull[gpio->pull];

    HAL_GPIO_Init(st_port, &st_gpio);
}

void gpio_write(GPIOPort_e port, GPIOPin_e pin, GPIOState_e state)
{
    HAL_GPIO_WritePin(_ports[port], _pins[pin], _states[state]);
}

void gpio_toggle(GPIOPort_e port, GPIOPin_e pin)
{
    HAL_GPIO_TogglePin(_ports[port], _pins[pin]);
}

GPIOState_e gpio_read(GPIOPort_e port, GPIOPin_e pin)
{
    if (HAL_GPIO_ReadPin(_ports[port], _pins[pin]) == GPIO_PIN_RESET)
    {
        return GH_STATE_RESET;
    }
    return GH_STATE_SET;
}

void gpio_register_interrupt_callback(GPIOPin_e pin, irq_func func)
{
    _callbacks[pin] = func;
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

static void gpio_exti_callback(uint16_t pin)
{
    for (uint8_t i = 0; i < GH_PIN_COUNT; i++)
    {
        if ((pin >> i) & 1)
        {
            _callbacks[i]();
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    gpio_exti_callback(pin);
}
