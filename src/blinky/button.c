#include "button.h"
#include "nvic_hal.h"
#include "rcc_hal.h"
#include "gpio_hal.h"
#include "system_hal.h"
#include "global.h"

// Dev board specific vars
static GH_Init_s _gpio = 
{
    .pin = GH_PIN_0,
    .pull = GH_PULL_NONE,
    .mode = GH_MODE_IT_RISING,
};
static const GPIOPort_e _port = GH_PORT_A;

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
        buttonPressed = gpio_read(_port, _gpio.pin);
        observer();
    }
    lastIrq = t_now;
}

void button_init(void)
{
    lastIrq = 0;
    buttonPressed = 0;
    observer = 0;

    rcc_gpioa_clk_enable();
    gpio_init(_port, &_gpio);
    gpio_register_interrupt_callback(_gpio.pin, button_irq);
    nvic_enable_irq(NVIC_EXTI0);
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
