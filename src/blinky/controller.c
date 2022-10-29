#include "stm32f0xx_hal.h"
#include "controller.h"
#include "button.h"
#include "LED.h"
#include "global.h"

#if 0
// For led on when button held and off when not
STATIC void handleButtonMomentary(void)
{
    if (button_pressed())
    {
        led_on();
    }
    else
    {
        led_off();
    }
}
#endif

STATIC void handleButton(void)
{
    led_toggle();
}

void run_controller(void)
{
    HAL_Init();
    led_init();
    button_init();
    button_register_observer(handleButton);
}
