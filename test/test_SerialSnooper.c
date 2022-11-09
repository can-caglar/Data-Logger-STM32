#include "unity.h"
#include "SerialSnooper.h"
#include "mock_MySD.h"
#include "mock_MyTimeString.h"
#include "mock_MyCircularBuffer.h"
#include "mock_stm32f0xx_hal.h"
#include "string.h"

void successfulInit(void);

void test_SerialSnooper_InitSuccess(void)
{
    successfulInit();
}

void test_SerialSnooper_Fail(void)
{
    MyCircularBuffer_init_Ignore();
    MyTimeString_Init_IgnoreAndReturn(0);
    MyTimeString_GetFileName_IgnoreAndReturn("hi.txt");
    MySD_Init_ExpectAndReturn("hi.txt", FR_NOT_READY);

    SerialSnooper_Init();

    // should do nothing
    SerialSnooper_Run();
}

void test_SerialSnooper_PollsCircularBufferWhenEmpty(void)
{
    successfulInit();

    MyCircularBuffer_isEmpty_ExpectAndReturn(1);
    HAL_GetTick_ExpectAndReturn(0);

    SerialSnooper_Run();
}

void test_SerialSnooper_WriteSDSuccess(void)
{
    successfulInit();
    uint8_t byteWritten = 'c';

    MyCircularBuffer_isEmpty_ExpectAndReturn(0);
    MyCircularBuffer_read_ExpectAndReturn('c');
    MyTimeString_GetTimeStamp_ExpectAndReturn("example");    // get timestamp
    MySD_WriteString_ExpectAndReturn("example", FR_OK);      // write it to SD
    MySD_Write_ExpectAndReturn(&byteWritten, 1, FR_OK);
    HAL_GetTick_ExpectAndReturn(0);

    SerialSnooper_Run();
}

void test_SerialSnooper_WriteSDFail(void)
{
    successfulInit();
    uint8_t byteWritten = 'c';

    MyCircularBuffer_isEmpty_ExpectAndReturn(0);
    MyCircularBuffer_read_ExpectAndReturn('c');
    MyTimeString_GetTimeStamp_ExpectAndReturn("example");    // get timestamp
    MySD_WriteString_ExpectAndReturn("example", FR_OK);      // write it to SD
    MySD_Write_ExpectAndReturn(&byteWritten, 1, FR_DISK_ERR);
    HAL_GetTick_ExpectAndReturn(0);

    SerialSnooper_Run();
}

void test_SerialSnooper_WriteSDCarriageReturnWillAddTimestamp(void)
{
    successfulInit();
    uint8_t byteWritten = '\r';

    // with \r
    MyCircularBuffer_isEmpty_ExpectAndReturn(0);
    MyCircularBuffer_read_ExpectAndReturn(byteWritten);
    MyTimeString_GetTimeStamp_ExpectAndReturn("example");    // get timestamp
    MySD_WriteString_ExpectAndReturn("example", FR_OK);      // write it to SD
    MySD_Write_ExpectAndReturn(&byteWritten, 1, FR_OK);
    HAL_GetTick_ExpectAndReturn(0);
    SerialSnooper_Run();

    // any other character
    byteWritten = 'a';
    MyCircularBuffer_isEmpty_ExpectAndReturn(0);
    MyCircularBuffer_read_ExpectAndReturn(byteWritten);
    MyTimeString_GetTimeStamp_ExpectAndReturn("example");    // get timestamp
    MySD_WriteString_ExpectAndReturn("example", FR_OK);      // write it to SD
    MySD_Write_ExpectAndReturn(&byteWritten, 1, FR_OK);      // write new word to sd
    HAL_GetTick_ExpectAndReturn(0);
    SerialSnooper_Run();

    // another character
    MyCircularBuffer_isEmpty_ExpectAndReturn(0);
    MyCircularBuffer_read_ExpectAndReturn(byteWritten);
    MySD_Write_ExpectAndReturn(&byteWritten, 1, FR_OK);
    HAL_GetTick_ExpectAndReturn(0);
    SerialSnooper_Run();
}

void test_SerialSnooper_FlushesEvery500ms(void)
{
    successfulInit();
    uint8_t byteWritten = 0x2;

    // with \r
    MyCircularBuffer_isEmpty_IgnoreAndReturn(0);
    MyCircularBuffer_read_IgnoreAndReturn(byteWritten);
    MyTimeString_GetTimeStamp_ExpectAndReturn("example");    // get timestamp
    MySD_WriteString_ExpectAndReturn("example", FR_OK);      // write it to SD
    MySD_Write_IgnoreAndReturn(FR_OK);

    HAL_GetTick_ExpectAndReturn(0);

    SerialSnooper_Run();

    HAL_GetTick_ExpectAndReturn(400);

    SerialSnooper_Run();

    HAL_GetTick_ExpectAndReturn(500);
    MySD_Flush_ExpectAndReturn(FR_OK);
    HAL_GetTick_ExpectAndReturn(500);

    SerialSnooper_Run();

    // won't call it again
    HAL_GetTick_ExpectAndReturn(510);
    SerialSnooper_Run();
}

/************************************************************/

// Helper functions

void successfulInit(void)
{
    MyCircularBuffer_init_Expect();

    MyTimeString_Init_ExpectAndReturn(0);
    MyTimeString_GetFileName_ExpectAndReturn("hi.txt");
    MySD_Init_ExpectAndReturn("hi.txt", FR_OK);

    SerialSnooper_Init();
}

/*
- [x] Opens a file with a name
    - [x] Name might be related to the date-time opened
        - [ ] Get the RTC working
- [x] Kick-starts a UART interrupt routine
    - [ ] UART interrupt routine will handle filling in the write buffer
        - [ ] Will generate date and time and fill it in
        - [ ] Write buffer is a circular buffer w/X size
        - [ ] The SD card writer will consume circular buffer
        - [ ] Make circular buf power of 2
- [ ] If there is data available in the write buffer,
    will write to the SD card. (Not sure how this will behave w/circ buf)
        - [ ] 
*/
