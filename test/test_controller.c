#include "unity.h"
#include "controller.h"
#include "mock_button.h"
#include "mock_LED.h"

#include "mock_stm32f4xx_hal.h"

extern void handleButton(void);

void test_controllerInit()
{
    HAL_Init_ExpectAndReturn(HAL_OK);
    led_init_Expect();
    button_init_Expect();
    button_register_observer_Expect(handleButton);

    run_controller();
}

void test_control(void)
{
    led_toggle_Expect();
    handleButton();
}

/*
Note:

ORANGE  PD13
GREEN   PD12
BUTTON  PA0

- [ ] Input line on PA0 
- [ ] Interrupt routed to button
- [ ] Input line is debounced
- [ ] Output on PD13 when button pressed

*/