#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "AppDecider.h"
#include "MyScheduler.h"
#include "SystemOperations.h"
#include "main.h"

// Ceedling has its own main function
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
        // The CLI application
        MyCLI_Init();
        LOOP
        {
            MyCLI_Run();
        }
    }
    else
    {
        // The Serial Snooping application
        CubeMX_SystemInit(CMX_UART);
        MyScheduler_Init();
        SystemOperations_Init();
        MyScheduler_AddTask(SystemOperations_OpenLogFile,
            0, true, true);
        MyScheduler_AddTask(SystemOperations_WriteSD, 
            0, true, true);
        MyScheduler_AddTask(SystemOperations_FlushSD,
            0, true, true);
        LOOP
        {
            MyScheduler_Run();
        }
    }
}
