#include "unity.h"
#include "SystemOperations.h"
#include "mock_MySD.h"
#include "mock_MyTimeString.h"
#include "mock_stm32f0xx_hal.h"
#include "mock_MyCircularBuffer.h"

typedef enum
{
    DONT_EXPECT_TIMESTAMP,
    EXPECT_TIMESTAMP,
} ExpectTimestamp_e;

static void successfulInit(void);
static void expectSerialSnooper(uint8_t* thisValue, uint8_t* nextVal, ExpectTimestamp_e expectTimestamp);

// Tests

void test_successInit(void)
{
    successfulInit();
}

void test_notifySdCardDoesNothingWhenBufferIsEmpty(void)
{
    SubjectData_t data = {.isEmpty = 1};

    notifySdCardWriter(&data);
    notifySdCardWriter(&data);
}

void test_notifySdCard_FirstTimeWritesTimestampAndByteToSD(void)
{
    SubjectData_t data = {.isEmpty = 0, .circBufData = 'c'};

    MyCircularBuffer_read_ExpectAndReturn(data.circBufData);
    MyTimeString_GetTimeStamp_ExpectAndReturn("example");    // get timestamp
    MySD_WriteString_ExpectAndReturn("example", FR_OK);      // write it to SD
    MySD_Write_ExpectAndReturn(&data.circBufData, 1, FR_OK);
    
    notifySdCardWriter(&data);
}

void test_notifySdCard_WriteSD_CROrLFAtEndWillAddTimestamp(void)
{
    SubjectData_t data = {.isEmpty = 0, .circBufData = 'c'};

    successfulInit();

    // first character shall always be timestamped
    uint8_t byteWritten = 'q';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);
    
    // with \r, this shall not be timestamped
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    // // any other character after the \r will be timestamped
    byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    // another character, no timestamp
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);
}


void test_notifySdCard_WriteSD_ConstantCRorLFWillAddTS(void)
{
    successfulInit();

    SubjectData_t data = {.isEmpty = 0, .circBufData = 'c'};

    // first character shall always be timestamped
    uint8_t byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);
}


void test_notifySdCardWriter_WriteSD_RespectsCRLF(void)
{
    successfulInit();

    SubjectData_t data = {.isEmpty = 0, .circBufData = 'c'};

    // first character shall always be timestamped
    uint8_t byteWritten = 'a';
    uint8_t nextByte = '\r';
    expectSerialSnooper(&byteWritten, &nextByte, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    byteWritten = '\r';
    nextByte = '\n';
    expectSerialSnooper(&byteWritten, &nextByte, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    byteWritten = '\n';
    nextByte = 'q';
    expectSerialSnooper(&byteWritten, &nextByte, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    nextByte = 'q';
    expectSerialSnooper(&byteWritten, &nextByte, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);
}


void test_SerialSnooper_WriteSD_RespectsCRLFEvenWhenNotQueuedInCircBuf(void)
{
    successfulInit();

    SubjectData_t data = {.isEmpty = 0, .circBufData = 'c'};

    HAL_GetTick_IgnoreAndReturn(0);

    // first character shall always be timestamped
    uint8_t byteWritten = 'a';
    uint8_t nextByte = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    byteWritten = '\n';
    byteWritten = 'q';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);
}

void test_sdCardWriter_WriteSD_RespectsMultipleCRLFByMakingNewLine(void)
{
    successfulInit();

    SubjectData_t data = {.isEmpty = 0, .circBufData = 'c'};

    HAL_GetTick_IgnoreAndReturn(0);

    // first character shall always be timestamped
    // '[Timestamp]: a'
    uint8_t byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    // '[Timestamp]: a\r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    // '[Timestamp]: a\r\n'
    byteWritten = '\n';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    byteWritten = '\n';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    // '[Timestamp]: \r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    // '[Timestamp]: \r'
    // '[Timestamp]: a'
    byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(&data);
}


void test_sdCardFlusher_FlushesEvery500ms(void)
{
    successfulInit();
    SubjectData_t data = {.isEmpty = 0, .circBufData = 'c'};

    uint8_t byteWritten = 0x2;

    HAL_GetTick_ExpectAndReturn(0);

    notifySdCardFlusher(&data);

    HAL_GetTick_ExpectAndReturn(400);

    notifySdCardFlusher(&data);

    HAL_GetTick_ExpectAndReturn(500);
    MySD_Flush_ExpectAndReturn(FR_OK);
    HAL_GetTick_ExpectAndReturn(500);

    notifySdCardFlusher(&data);

    // won't call it again
    HAL_GetTick_ExpectAndReturn(510);
    notifySdCardFlusher(&data);
}

void test_OpenLogFile_OpensOneFileClosesTheOther(void)
{
    // gets name of file from time string
    MyTimeString_GetFileName_ExpectAndReturn("hi.txt");

    // opens file
    MySD_Init_ExpectAndReturn("hi.txt", FR_OK);

    SystemOperations_OpenLogFile();
}


// Helper functions


void successfulInit(void)
{
    MyCircularBuffer_init_Expect();

    MyTimeString_Init_ExpectAndReturn(0);

    // gets name of file from time string
    MyTimeString_GetFileName_ExpectAndReturn("hi.txt");

    // opens file
    MySD_Init_ExpectAndReturn("hi.txt", FR_OK);

    int res = SystemOperations_Init();
    TEST_ASSERT_EQUAL(SO_SUCCESS, res);
}

static void expectSerialSnooper(uint8_t* thisValue, uint8_t* nextVal, ExpectTimestamp_e expectTimestamp)
{
    // first character shall always be timestamped
    MyCircularBuffer_read_ExpectAndReturn(*thisValue);
    if (expectTimestamp == EXPECT_TIMESTAMP)
    {
        MyTimeString_GetTimeStamp_ExpectAndReturn("[26-11-22]");    // get timestamp
        MySD_WriteString_ExpectAndReturn("[26-11-22]", FR_OK);      // write it to SD
    }
    MySD_Write_ExpectAndReturn(thisValue, 1, FR_OK);
}


/*
// Schedules / states:
- [ ] Closes file and open new one
    - When greater than max size
    - When greater than certain size and circular buffer empty
- [ ] Write to SD card:
    - Whenever circular buffer is not empty
        - Time stamp
        - Error message
        - Latest character in circular buffer
- [ ] Flushes file
    - When max time elapsed
    - When certain time elapsed and circular buffer empty

*/