#include "autobaudrate.h"
#include "MyTimer.h"
#include "stm32f3xx_hal.h"
#include <string.h>
#include <stdio.h>

#define NUM_PERIODS_STORED 20

static uint32_t periods[NUM_PERIODS_STORED];
static uint8_t index = 0;
static uint32_t baud;

static void baudrateCb(uint32_t timePeriodNs);

void autobaudrate_init(void)
{
    MyTimer_init(baudrateCb);
    baud = UNDEFINED_BR;
    memset(periods, ~0U, NUM_PERIODS_STORED * sizeof(periods[0]));
    index = 0;
}

uint32_t autobaudrate_getBr(void)
{
    if (index >= NUM_PERIODS_STORED)
    {
        uint32_t minPeriodNs = ~0;
        for (int i = 0; i < NUM_PERIODS_STORED; i++)
        {
            if (minPeriodNs > periods[i])
            {
                minPeriodNs = periods[i];
            }
        }
        if (minPeriodNs != ~0)
        {
            baud = 1000000000 / minPeriodNs;
        }
    }
    return baud;
}

// Private funcs

static void baudrateCb(uint32_t timePeriodNs)
{
    if (index < NUM_PERIODS_STORED)
    {
        periods[index++] = timePeriodNs;
    }
}
