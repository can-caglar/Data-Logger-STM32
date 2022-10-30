#include "AppDecider.h"
#include "MyDipSwitch.h"

void AppDecider_Init()
{
    MyDIP_Init();
}

Applications AppDecider_Decide()
{
    Applications app = APP_CLI;
    if (MyDIP_Read())
    {
        app = APP_SNOOPING;
    }
    return app;
}