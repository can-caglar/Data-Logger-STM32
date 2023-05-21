#include "fake_MyTimer.h"
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

static void(*Callback)(uint32_t);
static void defaultCb(uint32_t);
static uint32_t lastTimeInterruptedNs;

static void pretendToReceiveChar(const char data, uint32_t baudRate);
static void fake_mytimer_pretendInterruptOccured(uint32_t atTimeNs);

void MyTimer_init(void(*Fn)(uint32_t))
{
    lastTimeInterruptedNs = 0;
    Callback = Fn;
}

void fake_mytimer_reset(void)
{
    Callback = defaultCb;   // to prevent crash
    lastTimeInterruptedNs = 0;
}

void fake_mytimer_pretendToReceiveString(const char* data, uint32_t baudRate)
{
    size_t dataLength = strlen(data);
    for (size_t i = 0; i < dataLength; i++)
    {
        pretendToReceiveChar(data[i], baudRate);
    }
}

void pretendToReceiveChar(char data, uint32_t baudRate)
{
    double periodNs = 1000000000.0f / baudRate;
    double currentTime = 0;
    
    uint8_t previousBit = 0; // start bit is always 0
    // ignore start bit transition interrupt as it'll most likely be
    // bigger than baud period.
    // data
    for (uint8_t i = 0; i < 8; i++) // lsb first
    {
        currentTime += periodNs;    // each bit takes the same time to xfer
        uint8_t bit = (data & (1U << i)) != 0;
        if (bit != previousBit)
        {
            // a transition
            fake_mytimer_pretendInterruptOccured(roundl(currentTime));
        }
        previousBit = bit;
    }
    // idle will also cause a transition
    uint8_t idleVal = 1;
    currentTime += periodNs;
    if (previousBit != idleVal)
    {
        // another transition
        fake_mytimer_pretendInterruptOccured(currentTime);
    }
}

static void fake_mytimer_pretendInterruptOccured(uint32_t atTimeNs)
{
    // the callback receives the time since last interrupt
    Callback(atTimeNs - lastTimeInterruptedNs);
    lastTimeInterruptedNs = atTimeNs;
}

static void defaultCb(uint32_t arg)
{
}