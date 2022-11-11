#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "controller.h"
#include "AppDecider.h"
#include "SerialSnooper.h"

#include "main.h"

#include "MyRTC.h" //rm me

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
        #if 0
        MyRTC_Init();
        MyTime t =
        {
            .year = 22,
            .month = 11,
            .day = 11,
            .hour = 8,
            .minute = 59,
            .second = 0,
            .weekday = 5
        };
        MyRTC_WriteTime(&t);
        #endif
        SerialSnooper_Init();
        LOOP
        {
            SerialSnooper_Run();
        } 
    }
}
