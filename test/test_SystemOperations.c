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

static const int testValue = 777;
static const DataContext* fakeDataContext = (DataContext*)testValue;

static void successfulInit(void);
static void expectSerialSnooper(uint8_t* thisValue, uint8_t* nextVal, ExpectTimestamp_e expectTimestamp);
static void assertDataContextWasPassedInToGetters(void);
// Tests

void test_successInit(void)
{
    successfulInit();
}

void test_notifySdCardDoesNothingWhenBufferIsEmpty(void)
{
    fakeSetIsThereNewData(0);

    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();
}

void test_notifySdCard_FirstTimeWritesTimestampAndByteToSD(void)
{
    uint8_t ch = 'c';
    fakeSetIsThereNewData(1);
    fakeSetLatestData(ch);
    fakeSetTimestampString("example");

    MySD_WriteString_ExpectAndReturn("example", FR_OK);      // write it to SD
    MySD_Write_ExpectAndReturn(&ch, 1, FR_OK);
    
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();
}

void test_notifySdCard_WriteSD_CROrLFAtEndWillAddTimestamp(void)
{
    fakeSetIsThereNewData(1);
    fakeSetLatestData('c');

    successfulInit();

    // first character shall always be timestamped
    uint8_t byteWritten = 'q';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();
    
    // with \r, this shall not be timestamped
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // // any other character after the \r will be timestamped
    byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // another character, no timestamp
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();
}


void test_notifySdCard_WriteSD_ConstantCRorLFWillAddTS(void)
{
    successfulInit();

    fakeSetIsThereNewData(1);

    // first character shall always be timestamped
    uint8_t byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();
}


void test_notifySdCardWriter_WriteSD_RespectsCRLF(void)
{
    successfulInit();
    fakeSetIsThereNewData(1);

    // first character shall always be timestamped
    uint8_t byteWritten = 'a';
    uint8_t nextByte = '\r';
    expectSerialSnooper(&byteWritten, &nextByte, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    byteWritten = '\r';
    nextByte = '\n';
    expectSerialSnooper(&byteWritten, &nextByte, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    byteWritten = '\n';
    nextByte = 'q';
    expectSerialSnooper(&byteWritten, &nextByte, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    nextByte = 'q';
    expectSerialSnooper(&byteWritten, &nextByte, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();
}


void test_SerialSnooper_WriteSD_RespectsCRLFEvenWhenNotQueuedInCircBuf(void)
{
    successfulInit();
    fakeSetIsThereNewData(1);

    fakeSetTime(0);

    // first character shall always be timestamped
    uint8_t byteWritten = 'a';
    uint8_t nextByte = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    byteWritten = '\n';
    byteWritten = 'q';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();
}

void test_sdCardWriter_WriteSD_RespectsMultipleCRLFByMakingNewLine(void)
{
    successfulInit();
    fakeSetIsThereNewData(1);

    fakeSetTime(0);

    // first character shall always be timestamped
    // '[Timestamp]: a'
    uint8_t byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // '[Timestamp]: a\r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // '[Timestamp]: a\r\n'
    byteWritten = '\n';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    byteWritten = '\n';
    expectSerialSnooper(&byteWritten, NULL, DONT_EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    // '[Timestamp]: \r'
    byteWritten = '\r';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // '[Timestamp]: a\r\n'
    // '[Timestamp]: \r\n'
    // '[Timestamp]: \r'
    // '[Timestamp]: a'
    byteWritten = 'a';
    expectSerialSnooper(&byteWritten, NULL, EXPECT_TIMESTAMP);
    notifySdCardWriter(fakeDataContext);
    assertDataContextWasPassedInToGetters();
}


void test_sdCardFlusher_FlushesEvery500ms(void)
{
    successfulInit();

    uint8_t byteWritten = 0x2;

    fakeSetTime(0);

    notifySdCardFlusher(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    fakeSetTime(400);

    notifySdCardFlusher(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    fakeSetTime(500);
    MySD_Flush_ExpectAndReturn(FR_OK);
    fakeSetTime(500);

    notifySdCardFlusher(fakeDataContext);
    assertDataContextWasPassedInToGetters();

    // won't call it again
    fakeSetTime(510);
    notifySdCardFlusher(fakeDataContext);
    assertDataContextWasPassedInToGetters();
}

void test_OpenLogFile_OpensOneFileClosesTheOther(void)
{
    // gets name of file from time string
    fakeSetFileName("hi.txt");

    // opens file
    MySD_Init_ExpectAndReturn("hi.txt", FR_OK);

    SystemOperations_OpenLogFile(fakeDataContext);
    assertDataContextWasPassedInToGetters();
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

static void assertDataContextWasPassedInToGetters(void)
{
    TEST_ASSERT_EQUAL(fakeDataContext, fakeGetLatestDataContextPassedIn());
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