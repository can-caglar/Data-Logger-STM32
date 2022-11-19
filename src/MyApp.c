#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "AppDecider.h"
#include "SerialSnooper.h"

#include "main.h"

#ifndef TEST
#define runApp main
#endif

/*
int main(void)
*/
int runApp(void)
{
    CubeMX_SystemInit(CMX_FATFS);   // needs a 1s delay
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
        SerialSnooper_Init();
        LOOP
        {
            SerialSnooper_Run();
        } 
    }
}
