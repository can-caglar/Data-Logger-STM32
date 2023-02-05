#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "AppDecider.h"
#include "MyScheduler.h"
#include "SystemOperations.h"
#include "main.h"
// perhaps below should be in the other modules only
#include "MyTimeString.h"

extern UART_HandleTypeDef huart1;
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char printBuf[20];

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
        MyTimeString_Init();
        MyScheduler_AddTask(SystemOperations_OpenLogFile,
            0, true, true);
        MyScheduler_AddTask(SystemOperations_WriteSD, 
            0, true, true);
        MyScheduler_AddTask(SystemOperations_FlushSD,
            0, true, true);
        uint32_t tNow;
        uint32_t tElapsed;
        uint32_t logTimer = 0;
        LOOP
        {
            tNow = HAL_GetTick();
          
            MyScheduler_Run();
          
            tElapsed = HAL_GetTick() - tNow;
            
            if (tElapsed > 50)
            {
              sprintf(printBuf, "All T = %u\n\r---\n\r", tElapsed);
              HAL_StatusTypeDef err = HAL_UART_Transmit(&huart1, (uint8_t*)printBuf, strlen(printBuf), 500);
              if (err !=0)
              {
                sprintf(printBuf, "Couldn't transmit\n\r");
                HAL_StatusTypeDef err = HAL_UART_Transmit(&huart1, (uint8_t*)printBuf, strlen(printBuf), 500);
              }
            }
        }
    }
}
