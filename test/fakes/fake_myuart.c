#include "fake_myuart.h"
#include <string.h>

static struct
{
    MyUART_Config latestConfig;
    uint8_t configured;
} S; // Internal State

void fake_myuart_reset(void)
{
    memset(&S, 0, sizeof(S));
}

void MyUART_init(MyUART_Config* config)
{
    if (!S.configured)
    {
        S.latestConfig = *config;
        S.configured = 1;
    }
}

uint32_t fake_myuart_getBr(void)
{
    return S.latestConfig.baudrate;
}