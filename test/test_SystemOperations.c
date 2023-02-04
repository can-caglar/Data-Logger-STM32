#include "unity.h"
#include "SystemOperations.h"
#include "mock_MySD.h"
#include "mock_MyTimeString.h"
#include "mock_stm32f0xx_hal.h"
#include "mock_MyCircularBuffer.h"
#include "fake_DataHolder.h"

typedef enum
{
    DONT_EXPECT_TIMESTAMP,
    EXPECT_TIMESTAMP,
} ExpectTimestamp_e;

enum
{
    WITH_SUCCESS,
    WITH_FAILURE,
};

static void successfulInit(void);
static void expectSerialSnooper(uint8_t* thisValue, uint8_t* nextVal, ExpectTimestamp_e expectTimestamp);
static void expectToOpenAFile(int withSuccessOrFailure);
static void setNewHALTime(uint32_t newTime);  // TODO, test what happens when overflows

// Tests

void setUp(void)
{
    SystemOperations_Init();
}

void test_successInit(void)
{
    successfulInit();
}

void test_notifySdCardDoesNothingWhenBufferIsEmpty(void)
{
    fakeSetIsThereNewData(0);

    notifySdCardWriter();

    notifySdCardWriter();
}

void test_notifySdCard_FirstTimeWritesTimestampAndByteToSD(void)
{
    uint8_t ch = 'c';
    fakeSetIsThereNewData(1);
    fakeSetLatestData(ch);
    fakeSetTimestampString("example");

    MySD_WriteString_ExpectAndReturn("example", FR_OK);      // write it to SD
    MySD_Write_ExpectAndReturn(&ch, 1, FR_OK);
    
    notifySdCardWriter();
}

void test_notifySdCard_WriteSD_CROrLFAtEndWillAddTimestamp(void)
{
    fakeSetIsThereNewData(1);
    fakeSetLatestData('c');

    successfulInit();

    // first character shall always be timestamped
    uint8_t byteWritten = 'q';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();
    
    // with \r, this shall not be timestamped
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter();

    // // any other character after the \r will be timestamped
    byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();

    // another character, no timestamp
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter();
}


void test_notifySdCard_WriteSD_ConstantCRorLFWillAddTS(void)
{
    successfulInit();

    fakeSetIsThereNewData(1);

    // first character shall always be timestamped
    uint8_t byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();
}


void test_notifySdCardWriter_WriteSD_RespectsCRLF(void)
{
    successfulInit();
    fakeSetIsThereNewData(1);

    // first character shall always be timestamped
    uint8_t byteWritten = 'a';
    uint8_t nextByte = '\r';
    expectSerialSnooper(&byteWritten, &nextByte, EXPECT_TIMESTAMP);
    notifySdCardWriter();

    byteWritten = '\r';
    nextByte = '\n';
    expectSerialSnooper(&byteWritten, &nextByte, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter();

    byteWritten = '\n';
    nextByte = 'q';
    expectSerialSnooper(&byteWritten, &nextByte, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter();

    nextByte = 'q';
    expectSerialSnooper(&byteWritten, &nextByte, EXPECT_TIMESTAMP);
    notifySdCardWriter();
}


void test_SerialSnooper_WriteSD_RespectsCRLFEvenWhenNotQueuedInCircBuf(void)
{
    successfulInit();
    fakeSetIsThereNewData(1);

    // first character shall always be timestamped
    uint8_t byteWritten = 'a';
    uint8_t nextByte = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter();

    byteWritten = '\n';
    byteWritten = 'q';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();
}

void test_sdCardWriter_WriteSD_RespectsMultipleCRLFByMakingNewLine(void)
{
    successfulInit();
    fakeSetIsThereNewData(1);

    // first character shall always be timestamped
    // '[Timestamp]: a'
    uint8_t byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();

    // '[Timestamp]: a\r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter();

    // '[Timestamp]: a\r\n'
    byteWritten = '\n';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    byteWritten = '\n';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    // '[Timestamp]: \r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    // '[Timestamp]: \r'
    // '[Timestamp]: a'
    byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter();
}


void test_sdCardFlusher_FlushesEveryFLUSH_TIME_MS(void)
{
    successfulInit();

    setNewHALTime(0);

    notifySdCardFlusher();  // don't expect flush

    setNewHALTime(FLUSH_TIME_MS / 2);

    notifySdCardFlusher();  // don't expect flush

    setNewHALTime(FLUSH_TIME_MS);

    MySD_Flush_ExpectAndReturn(FR_OK);  // expect flush
    
    notifySdCardFlusher();

    // won't call it again
    setNewHALTime(FLUSH_TIME_MS * 1.5);
    notifySdCardFlusher();
}

void test_OpenLogFile_OpensOneFileClosesTheOtherWhenNoFileOpen(void)
{
    expectToOpenAFile(WITH_SUCCESS);

    // first time round
    SystemOperations_OpenLogFile();

    // a file is already open, should do nothing
    SystemOperations_OpenLogFile();
}

void test_OpenLogFile_TriesAgainIfOpenFailed(void)
{
    expectToOpenAFile(WITH_FAILURE);

    SystemOperations_OpenLogFile();

    // second time round, tries again
    expectToOpenAFile(WITH_SUCCESS);

    SystemOperations_OpenLogFile();
}

void test_OpenLogFile_OpensAnotherFileIfSizeAboveLimit(void)
{
    // gets name of file from time string
    expectToOpenAFile(WITH_SUCCESS);

    SystemOperations_OpenLogFile();

    // update the file size
    fakeSetFileSize(MAX_FILE_SIZE);
    fakeSetIsThereNewData(1);  // say we have data

    // expect to open a new file since max size reached
    expectToOpenAFile(WITH_SUCCESS);

    SystemOperations_OpenLogFile();
}

void test_OpenLogFile_OpensAnotherFileBeforeSizeLimitIfDataBufferEmpty(void)
{
    // expect to open new file
    expectToOpenAFile(WITH_SUCCESS);
    SystemOperations_OpenLogFile();

    // update the file size to low threshold
    fakeSetFileSize(FILE_SIZE_LOWER_THRESHOLD);
    // but we have data in buffer
    fakeSetIsThereNewData(1); 

    // new data available, don't expect to open a new file
    SystemOperations_OpenLogFile();

    fakeSetIsThereNewData(0); // no new data, expect to open now
    expectToOpenAFile(WITH_SUCCESS);
    SystemOperations_OpenLogFile();
}

// Helper functions


void successfulInit(void)
{
    int res = SystemOperations_Init();
    TEST_ASSERT_EQUAL(SO_SUCCESS, res);
}

static void expectSerialSnooper(uint8_t* thisValue, uint8_t* nextVal, ExpectTimestamp_e expectTimestamp)
{
    // first character shall always be timestamped
    fakeSetLatestData(*thisValue);

    if (expectTimestamp == EXPECT_TIMESTAMP)
    {
        fakeSetTimestampString("[26-11-22]");
        MySD_WriteString_ExpectAndReturn("[26-11-22]", FR_OK);      // write it to SD
    }
    MySD_Write_ExpectAndReturn(thisValue, 1, FR_OK);
}

static void expectToOpenAFile(int withSuccessOrFailure)
{
    int err = (withSuccessOrFailure == WITH_SUCCESS) ?
        FR_OK : FR_DISK_ERR;

    // gets name of file from time string
    fakeSetFileName("hi.txt");

    // open file
    MySD_Init_ExpectAndReturn("hi.txt", err);
}

static void setNewHALTime(uint32_t newTime)
{
    HAL_GetTick_ExpectAndReturn(newTime);
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