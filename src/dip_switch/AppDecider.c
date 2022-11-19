#include "AppDecider.h"
#include "MyDipSwitch.h"

#define CLI_VALUE   0x0F

void AppDecider_Init()
{
    MyDIP_Init();
}

Applications AppDecider_Decide()
{
    Applications app = APP_SNOOPING;
    if (MyDIP_Read() == CLI_VALUE)
    {
        app = APP_CLI;
    }
    return app;
}
