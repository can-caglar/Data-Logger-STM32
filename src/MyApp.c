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

// For saving on stack space. Define just 1 of these!
// -----
// #define TEST_DEFAULT_PROGRAM
// #define TEST_CIRC_BUF
#define TEST_SD_WRITE
// -----

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
      
    #if defined(TEST_DEFAULT_PROGRAM)
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
    #endif

    #if defined(TEST_CIRC_BUF)
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
    #endif

    #if defined(TEST_SD_WRITE)
      // Testing how long of a delay causes the circ buf to get filled up
      // during a write/flush cycle.
      SAFE_PRINT("Starting SD write test. Test will end when circular buf is full.\n\r");
      uint32_t loopCount = 0;
      MyCircularBuffer_init();
      
      // Data being captured
      #define DELAY_WINDOW 32  // has to be power of 2 for quick wrapping
      static uint32_t last_X_delays_greater_than_50ms[DELAY_WINDOW];
      static uint32_t last_X_delays_loop_index[DELAY_WINDOW];
      static uint32_t arrayIndex = 0;

      // The test
      while (!MyCircularBuffer_isFull())
      {
        tNow = HAL_GetTick(); 
        SystemOperations_OpenLogFile();
        SystemOperations_FlushSD();
        SystemOperations_WriteSD();
        tElapsed = HAL_GetTick() - tNow;  // the reading for this loop
        loopCount++;
        if (tElapsed > 50)
        {
            last_X_delays_greater_than_50ms[arrayIndex] = tElapsed;
            last_X_delays_loop_index[arrayIndex] = loopCount;
            // increment array index
            arrayIndex = (arrayIndex + 1) & (DELAY_WINDOW - 1); // a basic circular buffer
        }
      }
      
      // test result output
      SAFE_PRINT("Printing results...\n\r");
      for (int i = 0; i < DELAY_WINDOW; i++)
      {
        SAFE_PRINT_ARGS("[%u ms at loop %u], ", last_X_delays_greater_than_50ms[i], last_X_delays_loop_index[i]);
      }
      SAFE_PRINT_ARGS("\r\nTotal loops = %u\n\r", loopCount);
    #endif
}
