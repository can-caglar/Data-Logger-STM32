#ifndef NVIC_HAL_H
#define NVIC_HAL_H

typedef enum NVIC_IRQ_e
{
    NVIC_EXTI0,
    NVIC_COUNT,
} NVIC_IRQ_e;

void nvic_enable_irq(NVIC_IRQ_e irq);

#endif
