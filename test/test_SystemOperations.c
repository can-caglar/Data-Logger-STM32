#include "unity.h"
#include "SystemOperations.h"
// #include "mock_stm32f3xx_hal.h"

#include "MySD.h"
#include "fakeff.h"
#include "fake_myTimeString.h"
#include "MyCircularBuffer.h"
#include "DataHolder.h"
#include "FileNameIterator.h"
#include "exploding_fakes.h"
#include "fakefilesystem.h"
#include "fake_led.h"
#include "SystemConfigs.h"

#include <string.h>

#define TEST_TIMESTAMP "[Timestamp]"
#define SET_NEXT_LOGFILE_NAME(name) fake_myTimeString_setFileName(name)

static void expectToOpenAFile(int withSuccessOrFailure);
static void setNewHALTime(uint32_t newTime);
static void circBufWriteString(const char* str);
static void doWriteSD(int amountOfTimes);
static void inputStream(const char* str);

// Helper functions
static void fillUpFile(char* fileName, size_t amount);
static int newLogFileIsOpenedWhenCurrentLogFileSizeIs(size_t size);

void setUp(void)
{
    fakeff_reset();
    MyCircularBuffer_close();
    MyCircularBuffer_init();
    SystemOperations_Init();
}

void tearDown(void)
{
    MySD_Close();
    fake_myTimeString_reset();
}

void test_initWillOpenConfigFile(void)
{
    TEST_ASSERT_TRUE(fakefilesystem_fileExists(CONFIG_FILE_NAME));
}

void test_OpenLogFile_opensFileBasedOnFileNameFromTimeString(void)
{
    // Given
    SET_NEXT_LOGFILE_NAME("abc.txt");
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("abc.txt"));
    // When
    SystemOperations_OpenLogFile();
    // Then
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("abc.txt"));
}

void test_OpenLogFile_WontOpenAnotherFileIfCalledInSuccessionAlone(void)
{
    // Given
    SET_NEXT_LOGFILE_NAME("abc.txt");
    SystemOperations_OpenLogFile();
    SET_NEXT_LOGFILE_NAME("next.txt");
    // When
    for (int i = 0; i < 1000; i++)
    {
        SystemOperations_OpenLogFile();
    }
    // Then
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("abc.txt"));
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("next.txt"));
}

void test_OpenLogFile_OpensAnotherFileIfFileIsAtThreshold(void)
{
    TEST_ASSERT_TRUE(newLogFileIsOpenedWhenCurrentLogFileSizeIs(
        FILE_SIZE_LOWER_THRESHOLD));
}

void test_OpenLogFile_DoesNotOpenAnotherFileIsLessThanThreshold(void)
{
    TEST_ASSERT_FALSE(newLogFileIsOpenedWhenCurrentLogFileSizeIs(
        FILE_SIZE_LOWER_THRESHOLD - 1));
}

void test_OpenLogFile_doesNotOpenAnotherFileIfCircularBufferNotEmpty(void)
{
    MyCircularBuffer_write('a');

    TEST_ASSERT_FALSE(newLogFileIsOpenedWhenCurrentLogFileSizeIs(
        FILE_SIZE_LOWER_THRESHOLD));
}

void test_OpenLogFile_opensNewFileIfBufferNotEmptyButMaxSizeReached(void)
{
    MyCircularBuffer_write('a');

    TEST_ASSERT_TRUE(newLogFileIsOpenedWhenCurrentLogFileSizeIs(
        MAX_FILE_SIZE));
}

void test_OpeningFilesSavesFileNameToSD(void)
{
    TEST_IGNORE();

    // given
    SET_NEXT_LOGFILE_NAME("first");
    // when
    SystemOperations_OpenLogFile();
    // then
    const char* configFile = fakefilesystem_readfile(CONFIG_FILE_NAME);
    TEST_ASSERT_EQUAL_STRING("first", configFile);
}

#if 0

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

void test_InitWillGetConfigsFromSD(void)
{
    const char* fname = "myFile.txt";
    fake_SDCard_helperWriteFileData(fname, strlen(fname));

    SystemOperations_Init();

    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_totalNumOfFilesOpened());
    TEST_ASSERT_EQUAL_STRING("ssdata", fake_SDCard_getOpenFileName());

    // first file opened
    SystemOperations_OpenLogFile();

    TEST_ASSERT_EQUAL_INT(2, fake_SDCard_totalNumOfFilesOpened());
    TEST_ASSERT_EQUAL_STRING("myFile.txt", fake_SDCard_getOpenFileName());
}

void test_InitWillGetDefaultsIfConfigsFromSDCorrupt(void)
{
    // prepare fake returns
    const char* fname = "myFile.txt";
    fake_SDCard_helperWriteFileData(fname, strlen(fname));
    fake_SDCard_toReturn(FR_DISK_ERR);
    fake_myTimeString_setFileName("timestr");

    SystemOperations_Init();

    TEST_ASSERT_EQUAL_INT(0, fake_SDCard_totalNumOfFilesOpened());
    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getOpenFileName());

    // first file opened
    fake_SDCard_toReturn(FR_OK);
    SystemOperations_OpenLogFile();

    TEST_ASSERT_EQUAL_STRING("timestr", fake_SDCard_getOpenFileName());
    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_totalNumOfFilesOpened());
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

// Helper functions

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

#endif

static void fillUpFile(char* fileName, size_t amount)
{
    TEST_ASSERT_EQUAL_INT(FR_OK, MySD_Init(fileName));
    for (size_t i = 0; i < amount; i++)
    {
        TEST_ASSERT_EQUAL_INT(i, MySD_getOpenedFileSize());
        MySD_WriteString("x");
        MySD_Flush();
    }
}

static int newLogFileIsOpenedWhenCurrentLogFileSizeIs(size_t size)
{
    fillUpFile("abc.txt", size);

    SET_NEXT_LOGFILE_NAME("abc.txt");
    SystemOperations_OpenLogFile();

    SET_NEXT_LOGFILE_NAME("next.txt");
    SystemOperations_OpenLogFile();

    return fakefilesystem_fileExists("next.txt");
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