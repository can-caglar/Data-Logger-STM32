#include "unity.h"
#include "SerialSnooper.h"
#include "mock_MySD.h"
#include "mock_MyTimeString.h"
#include "mock_MyCircularBuffer.h"
#include "mock_stm32f0xx_hal.h"
#include "mock_main.h"
#include "string.h"

typedef enum
{
    DONT_EXPECT_TIMESTAMP,
    EXPECT_TIMESTAMP,
} ExpectTimestamp_e;

static void successfulInit(void);
static void expectSerialSnooper(uint8_t* thisValue, uint8_t* nextVal, ExpectTimestamp_e expectTimestamp)
{
    // first character shall always be timestamped
    MyCircularBuffer_isEmpty_ExpectAndReturn(0);
    MyCircularBuffer_read_ExpectAndReturn(*thisValue);
    if (expectTimestamp == EXPECT_TIMESTAMP)
    {
        MyTimeString_GetTimeStamp_ExpectAndReturn("[26-11-22]");    // get timestamp
        MySD_WriteString_ExpectAndReturn("[26-11-22]", FR_OK);      // write it to SD
    }
    MySD_Write_ExpectAndReturn(thisValue, 1, FR_OK);
}

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

    TEST_ASSERT_EQUAL_INT(SS_FAIL, SerialSnooper_Init());
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

void test_SerialSnooper_WriteSD_CROrLFAtEndWillAddTimestamp(void)
{
    successfulInit();

    HAL_GetTick_IgnoreAndReturn(0);

    // first character shall always be timestamped
    uint8_t byteWritten = 'q';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // with \r, this shall not be timestamped
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // // any other character after the \r will be timestamped
    byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // another character, no timestamp
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    SerialSnooper_Run();
}

void test_SerialSnooper_WriteSD_ConstantCRorLFWillAddTS(void)
{
    successfulInit();

    HAL_GetTick_IgnoreAndReturn(0);

    // first character shall always be timestamped
    uint8_t byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();
}

void test_SerialSnooper_WriteSD_RespectsCRLF(void)
{
    successfulInit();

    HAL_GetTick_IgnoreAndReturn(0);

    // first character shall always be timestamped
    uint8_t byteWritten = 'a';
    uint8_t nextByte = '\r';
    expectSerialSnooper(&byteWritten, &nextByte, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    byteWritten = '\r';
    nextByte = '\n';
    expectSerialSnooper(&byteWritten, &nextByte, DONT_EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    byteWritten = '\n';
    nextByte = 'q';
    expectSerialSnooper(&byteWritten, &nextByte, DONT_EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    nextByte = 'q';
    expectSerialSnooper(&byteWritten, &nextByte, EXPECT_TIMESTAMP);
    SerialSnooper_Run();
}

void test_SerialSnooper_WriteSD_RespectsCRLFEvenWhenNotQueuedInCircBuf(void)
{
    successfulInit();

    HAL_GetTick_IgnoreAndReturn(0);

    // first character shall always be timestamped
    uint8_t byteWritten = 'a';
    uint8_t nextByte = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    byteWritten = '\n';
    byteWritten = 'q';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();
}

void test_SerialSnooper_WriteSD_RespectsMultipleCRLFByMakingNewLine(void)
{
    successfulInit();

    HAL_GetTick_IgnoreAndReturn(0);

    // first character shall always be timestamped
    // '[Timestamp]: a'
    uint8_t byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // '[Timestamp]: a\r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // '[Timestamp]: a\r\n'
    byteWritten = '\n';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    byteWritten = '\n';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    // '[Timestamp]: \r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    SerialSnooper_Run();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    // '[Timestamp]: \r'
    // '[Timestamp]: a'
    byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
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

    TEST_ASSERT_EQUAL(SS_SUCCESS, SerialSnooper_Init());
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
