#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "AppDecider.h"
#include "MyScheduler.h"
#include "SystemOperations.h"
#include "main.h"
// perhaps below should be in the other modules only
#include "MyTimeString.h"
#include "MySD.h"
#include "stm32f3xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ceedling has its own main function
#ifndef TEST
#define runApp main
#endif

uint32_t tBegin;
uint32_t tEnd;
uint32_t tElapsed;
uint32_t tElapsedMax = 0;
float averageTime;
float kbps;

uint32_t tBegin2;
uint32_t tEnd2;
uint32_t tElapsed2;

#define MAX_BUF_SIZE 4096
uint8_t buf[MAX_BUF_SIZE];
const uint16_t writeAmount = 1024;
const int testRunAmount = 3000;

/*
int main(void)
*/
int runApp(void)
{
    CubeMX_SystemInit(CMX_FATFS);   // needs a 1s delay
    CubeMX_SystemInit(CMX_UART);
    SystemOperations_Init();
    MyTimeString_Init();
  
    memset(buf, 'X', MAX_BUF_SIZE);
    
    // test begin
    printf("------------\n\rStarting test. Write amount : %u\n\r", writeAmount);
    MySD_Init("test.txt");
    
    // Begin timer
    tBegin = HAL_GetTick();
    uint8_t* bufPtr;
    uint32_t totalDataWrittenCounter = 0;
    for (int i = testRunAmount; i; i--)
    {
      bufPtr = buf;
      for (int j = 0; j < MAX_BUF_SIZE; j += writeAmount)
      {
        tBegin2 = HAL_GetTick();
        if (MySD_Write(bufPtr, writeAmount) != FR_OK)
        {
          printf("Write error\n\r");
          return 1;
        }
        tElapsed2 = HAL_GetTick() - tBegin2;
        if (tElapsedMax < tElapsed2)
        {
          tElapsedMax = tElapsed2;
        }
        bufPtr += writeAmount;
        totalDataWrittenCounter += writeAmount;
      }
    }
    // End timer
    tEnd = HAL_GetTick();
    MySD_Close();
    // Calculate benchmark data
    tElapsed = tEnd - tBegin;
    averageTime = (float)tElapsed / testRunAmount;
    kbps = MAX_BUF_SIZE / averageTime;
    // Print data
    printf("Total time = %u ms, average = %f ms, data written = %u bits \n\r", tElapsed, averageTime, totalDataWrittenCounter);
    printf("Data rate = %f kbits per second. Max delay writing = %u ms\n\r", kbps, tElapsedMax);
    
    printf("End of test\n\r");
}
