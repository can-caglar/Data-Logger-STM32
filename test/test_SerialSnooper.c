#include "unity.h"
#include "SerialSnooper.h"
#include "mock_MySD.h"
#include "mock_MyRTC.h"
#include "mock_MyCircularBuffer.h"

#include "mock_stm32f0xx_hal_uart.h"
#include "string.h"

#define FAKE_DATE   "06-11-22-1131.txt"
#define READ_TO_RETURN 5
#define MAX_BUF 2048

uint8_t isEmptyStubCallback(int numCalls);
uint8_t readStubCallback(int numCalls);

void tearDown(void)
{
    SerialSnooper_Close();
}

void test_SerialSnooper_Init(void)
{
    MyRTC_Init_ExpectAndReturn(0);
    MyRTC_GetTimeStr_ExpectAndReturn(FAKE_DATE);
    MySD_Init_ExpectAndReturn(FAKE_DATE, FR_OK);

    SerialSnooper_Init();
}

void test_SerialSnooper_Run_BufferReadsUntil2048(void)
{
    uint8_t buf[MAX_BUF + 1];   // 1 extra for null terminator
    memset(buf, READ_TO_RETURN, MAX_BUF);
    buf[MAX_BUF] = 0; // to keep unity happy

    // keep reading while there is data in buffer
    MyCircularBuffer_isEmpty_StubWithCallback(isEmptyStubCallback);
    MyCircularBuffer_read_StubWithCallback(readStubCallback);
    // Write to SD when done
    MySD_Write_ExpectAndReturn(buf, MAX_BUF, FR_OK);

    SerialSnooper_Run();
}

void test_SerialSnooper_Run_readsUntilEmpty(void)
{
    const int sz = 10;
    const int toReturn = 0x42;
    uint8_t buf[sz + 1];
    memset(buf, toReturn, sz);
    buf[sz] = 0;  // to keep unity happy

    for (int i = 0; i < sz; i++)
    {
        MyCircularBuffer_isEmpty_ExpectAndReturn(0);
        MyCircularBuffer_read_ExpectAndReturn(toReturn);
    }
    MyCircularBuffer_isEmpty_ExpectAndReturn(1);
    MySD_Write_ExpectAndReturn(buf, sz, FR_OK);

    SerialSnooper_Run();
}

/************************************************************/

// Helper functions
uint8_t isEmptyStubCallback(int numCalls)
{
    TEST_ASSERT_TRUE_MESSAGE(numCalls <= MAX_BUF, "isEmpty called more than MAX_BUF times");
    return 0;
}

uint8_t readStubCallback(int numCalls)
{
    TEST_ASSERT_TRUE_MESSAGE(numCalls <= MAX_BUF, "readStub called more than MAX_BUF times");
    return READ_TO_RETURN;
}

/*
- [x] Opens a file with a name
    - [x] Name might be related to the date-time opened
        - [ ] Get the RTC working
- [ ] Kick-starts a UART interrupt routine
    - [ ] UART interrupt routine will handle filling in the write buffer
        - [ ] Will generate date and time and fill it in
        - [ ] Write buffer is a circular buffer w/X size
        - [ ] The SD card writer will consume circular buffer
        - [ ] Make circular buf power of 2
- [ ] If there is data available in the write buffer,
    will write to the SD card. (Not sure how this will behave w/circ buf)
        - [ ] 
*/
