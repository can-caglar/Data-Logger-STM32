#include "MyApp.h"
#include "MyCLI.h"
#include "Loop.h"
#include "AppDecider.h"
#include "MyScheduler.h"
#include "SystemOperations.h"
#include "main.h"
// perhaps below should be in the other modules only
#include "MyTimeString.h"
#include "MyCircularBuffer.h"
#include <stdio.h>
#include "stm32f3xx.h"
#include "MySD.h"

// Ceedling has its own main function
#ifndef TEST
#define runApp main
#endif

#define SAFE_PRINT(str) __disable_irq(); printf(str); __enable_irq()
#define SAFE_PRINT_ARGS(str, ...) __disable_irq(); printf(str, __VA_ARGS__); __enable_irq()

typedef enum TEST_e
{
  TEST_DEFAULT_PROGRAM,
  TEST_CIRC_BUF,
  TEST_SD_WRITE,
} TEST_e;

TEST_e test = TEST_SD_WRITE;
uint32_t tNow;
uint32_t tElapsed;

/*
int main(void)
*/
int runApp(void)
{
    CubeMX_SystemInit(CMX_FATFS);   // needs a 1s delay
    AppDecider_Init();
    CubeMX_SystemInit(CMX_UART);
    MyScheduler_Init();
    SystemOperations_Init();
    MyTimeString_Init();
      
    switch (test)
    {
      case TEST_DEFAULT_PROGRAM:
      {
        // The Serial Snooping application
        
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
        break;
      }
      case TEST_CIRC_BUF:
      {
          // Testing how long it takes to fill up circular buffer
          SAFE_PRINT("Starting circular buffer test. Blast data via UART!\n\r");
          MyCircularBuffer_init();
          uint32_t bufSize = 0;
          while(MyCircularBuffer_isEmpty()) 
          {
              tNow = HAL_GetTick();  // sit here while buf is empty
          }
          while (!MyCircularBuffer_isFull())
          {
              // sit here while buf is not full
          }
          tElapsed = HAL_GetTick() - tNow;
          bufSize = MyCircularBuffer_size();
          SAFE_PRINT_ARGS("Time to fill circular buffer %u ms | Total data: %u bytes\n\r", tElapsed, bufSize);
          break;
      }
      case TEST_SD_WRITE:
      {
          // Testing how long of a delay causes the circ buf to get filled up
          // during a write/flush cycle.
          SAFE_PRINT("Starting SD write test. Test will end when circular buf is full.\n\r");
          uint32_t maxDelay = 0;
          uint32_t maxDelayIndex = 0;
          uint32_t loopCount = 0;
          MyCircularBuffer_init();

          while (!MyCircularBuffer_isFull())
          {
            tNow = HAL_GetTick();
            SystemOperations_OpenLogFile();
            SystemOperations_WriteSD();
            SystemOperations_FlushSD();
            tElapsed = HAL_GetTick() - tNow;
            loopCount++;
            if (maxDelay < tElapsed)
            {
                maxDelay = tElapsed;
                maxDelayIndex = loopCount;
            }
          }
          SAFE_PRINT_ARGS("Max time delay = %u at maxDelayIndex = %u. Total loops = %u\n\r",
             maxDelay, maxDelayIndex, loopCount);
          break;
      }
      default:
      {
        printf("No test program chosen\n\r");
        break;
      }
    };
}
