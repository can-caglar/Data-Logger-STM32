#include "unity.h"
#include "SystemOperations.h"
#include "mock_stm32f3xx_hal.h"

#include "fake_SDCard.h"
#include "fake_myTimeString.h"
#include "MyCircularBuffer.h"
#include "DataHolder.h"
#include "FileNameIterator.h"

#include <string.h>

#define TEST_TIMESTAMP "[Timestamp]"

static void successfulInit(void);
static void expectToOpenAFile(int withSuccessOrFailure);
static void setNewHALTime(uint32_t newTime);  // TODO, test what happens when overflows
static void circBufWriteString(const char* str);
static void doWriteSD(int amountOfTimes);
static void inputStream(const char* str);

// Tests

void setUp(void)
{
    SystemOperations_Init();
    // Reset circular buffer
    MyCircularBuffer_close();
    MyCircularBuffer_init();
    // reset fake sd card module
    fake_SDCard_reset();
}

void tearDown(void)
{
    MySD_Close();
    fake_myTimeString_reset();
}

void test_successInit(void)
{
    successfulInit();
}

void test_WriteSD_DoesNotWriteIfSDCardNotInitialised(void)
{
    MyCircularBuffer_write('a');

    SystemOperations_WriteSD();

    MySD_Flush();

    TEST_ASSERT_EQUAL(1, fake_SDCard_isFileEmpty());
}

void test_WriteSD_DoesNotWriteWhenBufferIsEmpty(void)
{
    // Circ buf empty

    doWriteSD(1);

    TEST_ASSERT_EQUAL(1, fake_SDCard_isFileEmpty());
}

void test_WriteSD_TimestampsAndSendsByte(void)
{
    fake_myTimeString_setTimestamp("[Timestamp]");

    MySD_Init("file.txt");
    inputStream("c");
    MySD_Flush();

    TEST_ASSERT_EQUAL_STRING("[Timestamp]c", fake_SDCard_getFileData());
}

void test_WriteSD_CROrLFAtEndWillAddTimestamp(void)
{
    fake_myTimeString_setTimestamp("[Timestamp]");
    
    MySD_Init("file.txt");
    inputStream("hi\rho");
    MySD_Flush();

    TEST_ASSERT_EQUAL_STRING("[Timestamp]hi\r[Timestamp]ho", fake_SDCard_getFileData());
}

void test_WriteSD_ConstantCRorLFWillAddTS(void)
{
    fake_myTimeString_setTimestamp("[Timestamp]");
    
    MySD_Init("file.txt");
    inputStream("\r\r\r");
    MySD_Flush();

    TEST_ASSERT_EQUAL_STRING("[Timestamp]\r[Timestamp]\r[Timestamp]\r", fake_SDCard_getFileData());
}

void test_WriteSD_RespectsCRLF(void)
{
    fake_myTimeString_setTimestamp("[Timestamp]");
    MySD_Init("file.txt");

    MySD_Init("file.txt");
    inputStream("hi\r\nthere");
    MySD_Flush();

    TEST_ASSERT_EQUAL_STRING("[Timestamp]hi\r\n[Timestamp]there", fake_SDCard_getFileData());
}


void test_WriteSD_RespectsCRLFEvenWhenNotQueuedInCircBuf(void)
{
    fake_myTimeString_setTimestamp("[Timestamp]");

    MySD_Init("file.txt");
    inputStream("hi\r");
    inputStream("\nthere");
    MySD_Flush();

    TEST_ASSERT_EQUAL_STRING("[Timestamp]hi\r\n[Timestamp]there", fake_SDCard_getFileData());
}

void test_WriteSD_RespectsMultipleCRLFCombinationsByMakingNewLines(void)
{
    fake_myTimeString_setTimestamp("[Timestamp]");

    MySD_Init("file.txt");
    inputStream("hi\r\n\r\n\rthere");
    MySD_Flush();

    TEST_ASSERT_EQUAL_STRING("[Timestamp]hi\r\n"
                            "[Timestamp]\r\n"
                            "[Timestamp]\r"
                            "[Timestamp]there", 
                            fake_SDCard_getFileData());
}

void test_sdCardFlusher_FlushesEveryFLUSH_TIME_MS(void)
{
    fake_myTimeString_setTimestamp("");

    MySD_Init("file.txt");
    inputStream("hello");

    setNewHALTime(0);

    // don't expect flush
    SystemOperations_FlushSD();  
    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getFileData());

    setNewHALTime(FLUSH_TIME_MS / 2);

    // don't expect flush
    SystemOperations_FlushSD();  // don't expect flush
    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getFileData());

    setNewHALTime(FLUSH_TIME_MS);

    // expect flush
    SystemOperations_FlushSD();
    TEST_ASSERT_EQUAL_STRING("hello", fake_SDCard_getFileData());

    inputStream("hi");

    setNewHALTime(FLUSH_TIME_MS * 1.5);

    // won't call it again
    SystemOperations_FlushSD();
    TEST_ASSERT_EQUAL_STRING("hello", fake_SDCard_getFileData());
}


void test_OpenLogFile_OpensOneFileClosesTheOtherWhenNoFileOpen(void)
{
    // first time round
    SystemOperations_OpenLogFile();

    TEST_ASSERT_EQUAL(1, fake_SDCard_totalNumOfFilesOpened());

    // a file is already open, should do nothing
    SystemOperations_OpenLogFile();

    TEST_ASSERT_EQUAL(1, fake_SDCard_totalNumOfFilesOpened());
}

void test_OpenLogFile_TriesAgainIfOpenFailed(void)
{
    // First time fail
    fake_SDCard_toReturn(FR_DISK_ERR);
    SystemOperations_OpenLogFile();
    TEST_ASSERT_EQUAL(0, fake_SDCard_totalNumOfFilesOpened());

    // second time round, tries again
    fake_SDCard_toReturn(FR_OK);
    SystemOperations_OpenLogFile();
    TEST_ASSERT_EQUAL(1, fake_SDCard_totalNumOfFilesOpened());
}

void test_OpenLogFile_OpensAnotherFileIfSizeAboveLimit(void)
{
    // Open log file
    SystemOperations_OpenLogFile();
    TEST_ASSERT_EQUAL(1, fake_SDCard_totalNumOfFilesOpened());

    // update the file size with MAX data
    char data[MAX_FILE_SIZE];
    memset(data, 'x', MAX_FILE_SIZE);
    data[MAX_FILE_SIZE] = '\0';
    inputStream(data);
    MySD_Flush();
    TEST_ASSERT_EQUAL(MAX_FILE_SIZE, MySD_getOpenedFileSize());
    
    // expect to open a new file since max size reached
    SystemOperations_OpenLogFile();
    TEST_ASSERT_EQUAL(2, fake_SDCard_totalNumOfFilesOpened());
}

void test_OpenLogFile_OpensAnotherFileBeforeSizeLimitIfDataBufferEmpty(void)
{
    // Open log file
    SystemOperations_OpenLogFile();
    TEST_ASSERT_EQUAL(1, fake_SDCard_totalNumOfFilesOpened());

    // Update the file size with FILE_SIZE_LOWER_THRESHOLD data
    char data[FILE_SIZE_LOWER_THRESHOLD];
    memset(data, 'x', FILE_SIZE_LOWER_THRESHOLD);
    data[FILE_SIZE_LOWER_THRESHOLD] = '\0';
    inputStream(data);
    MySD_Flush();
    TEST_ASSERT_EQUAL(FILE_SIZE_LOWER_THRESHOLD, MySD_getOpenedFileSize());

    // Fill up circular buffer so it isn't empty
    MyCircularBuffer_write('a');
    
    // don't expect new file as buffer not empty
    SystemOperations_OpenLogFile();
    TEST_ASSERT_EQUAL(1, fake_SDCard_totalNumOfFilesOpened());

    // expect new file since no new data
    MyCircularBuffer_read();
    SystemOperations_OpenLogFile();
    TEST_ASSERT_EQUAL(2, fake_SDCard_totalNumOfFilesOpened());
}

// Helper functions


void successfulInit(void)
{
    int res = SystemOperations_Init();
    TEST_ASSERT_EQUAL(SO_SUCCESS, res);
}

static void setNewHALTime(uint32_t newTime)
{
    HAL_GetTick_ExpectAndReturn(newTime);
}

static void circBufWriteString(const char* str)
{
    while (*str)
    {
        MyCircularBuffer_write(*str++);
    }
}

static void doWriteSD(int amountOfTimes)
{
    MySD_Init("test");  // sd card has to be initialised first
    for (int i = 0; i < amountOfTimes; i++)
    {
        SystemOperations_WriteSD();
    }
    MySD_Flush();   // flush the data else remains in sd cache
}

static void inputStream(const char* testStr)
{
    circBufWriteString(testStr);
    int stringLen = strlen(testStr);

    for (int i = 0; i < stringLen; i++)
    {
        SystemOperations_WriteSD();
    }
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