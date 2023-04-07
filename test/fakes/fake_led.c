#include "fake_led.h"
#include <string.h>

static struct
{
    uint8_t ledIsOn;
    uint8_t initialised;
} internals;


uint8_t fakeLed_reset(void)
{
    memset(&internals, 0, sizeof(internals));
}

uint8_t fakeLed_isOn(void)
{
    return internals.ledIsOn == 1;
}

void led_on(void)
{
    if (internals.initialised)
    {
        internals.ledIsOn = 1;
    }
}

void led_off(void)
{
    if (internals.initialised)
    {
        internals.ledIsOn = 0;
    }
}

void led_toggle(void)
{
    if (internals.initialised)
    {
        internals.ledIsOn ^= 1;
    }
}

void led_init(void)
{
    internals.initialised = 1;
}

