#include "gpio_interrupts.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdint.h>

#define PIN_COUNT 16

static irq_func _callbacks[PIN_COUNT];

void gpio_register_interrupt_callback(uint16_t pin, irq_func func)
{
    // TODO, TDD this module!
    for (int i = 0; i < 16; i++)
    {
        if (pin & 1)
        {
            _callbacks[i] = func;
        }
        pin >>= 1;
    }
}

// For pin 0s
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    for (uint8_t i = 0; i < PIN_COUNT; i++)
    {
        if ((pin >> i) & 1)
        {
            _callbacks[i]();
        }
    }
}

#if 0
void SysTick_Handler(void)
{
    HAL_IncTick();
}
#endif
