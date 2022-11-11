#include "BaudDecider.h"
#include "MyDipSwitch.h"

#define STATE_INITIALISED (1 << 0)

static uint8_t brState = 0;

void BaudDecider_Init(void)
{
    MyDIP_Init();
    brState |= STATE_INITIALISED;
}

uint32_t BaudDecider_GetBR(void)
{
    if ((brState & STATE_INITIALISED) == 0)
    {
        BaudDecider_Init();
    }
    int ret;
    uint8_t dip = MyDIP_Read();
    switch (dip)
    {
        case 0:
            ret = 921600;
            break;
        case 1:
            ret = 460800;
            break;
        case 2:
            ret = 230400;
            break;
        case 3:
            ret = 115200;
            break;
        case 4:
            ret = 57600;
            break;
        case 5:
            ret = 38400;
            break;
        case 6:
            ret = 19200;
            break;
        case 7:
            ret = 14400;
            break;
        case 8:
            ret = 9600;
            break;
        case 9:
            ret = 4800;
            break;
        case 10:
            ret = 2400;
            break;
        case 11:
            ret = 1200;
            break;
        case 12:
            ret = 600;
            break;
        case 13:
            ret = 300;
            break;
        case 14:
            ret = 110;
            break;
        case 15:
            ret = 19200; // CLI
            break;
        default:
            ret = 19200;
            break;
    }
    return ret;
}
