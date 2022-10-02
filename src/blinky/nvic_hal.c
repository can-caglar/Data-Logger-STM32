#include "nvic_hal.h"
#include "stm32f4xx_hal.h"

static IRQn_Type _irqs[NVIC_COUNT] = 
{
    [NVIC_EXTI0] = EXTI0_IRQn
};

void nvic_enable_irq(NVIC_IRQ_e irq)
{
    HAL_NVIC_EnableIRQ(_irqs[irq]);
}
