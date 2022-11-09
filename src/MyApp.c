#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "controller.h"
#include "AppDecider.h"
#include "SerialSnooper.h"

#include "main.h"

#ifndef TEST
#define runApp main    // TODO, change back to main
#endif

/*
int main(void)
*/
int runApp(void)
{
    CubeMX_SystemInit(CMX_FATFS);
    AppDecider_Init();
    
    if (AppDecider_Decide() == APP_CLI)
    {
        MyCLI_Init();
        LOOP
        {
            MyCLI_Run();
        }
    }
    else
    {
        CubeMX_SystemInit(CMX_UART | CMX_I2C);
        SerialSnooper_Init();
        LOOP
        {
            SerialSnooper_Run();
        } 
    }
    // never returns
}
