#include "unity.h"
#include "SystemOperations.h"

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
#include "fake_stm32f0xx_hal.h"

#include <string.h>

#define TEST_TIMESTAMP "[Timestamp]"
#define SET_NEXT_LOGFILE_NAME(name) fake_myTimeString_setFileName(name)
#define SET_NEXT_TIMESTAMP(ts) fake_myTimeString_setTimestamp(ts)

// Helper functions
static void fillUpFile(char* fileName, size_t amount);
static int isNewFileOpenedWhenLogfileSizeIs(size_t size);
static void doWriteOperationsNoFlush(const char* fileName, const char* data);
static void fillUpCircularBuffer(const char* data, size_t amount);
static void writeWithoutFlush(const char* file, const char* str);

void setUp(void)
{
    fakeff_reset();
    fake_halTick_reset();
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
    TEST_ASSERT_TRUE(isNewFileOpenedWhenLogfileSizeIs(
        FILE_SIZE_LOWER_THRESHOLD));
}

void test_OpenLogFile_DoesNotOpenAnotherFileIsLessThanThreshold(void)
{
    TEST_ASSERT_FALSE(isNewFileOpenedWhenLogfileSizeIs(
        FILE_SIZE_LOWER_THRESHOLD - 1));
}

void test_OpenLogFile_doesNotOpenAnotherFileIfCircularBufferNotEmpty(void)
{
    MyCircularBuffer_write('a');

    TEST_ASSERT_FALSE(isNewFileOpenedWhenLogfileSizeIs(
        FILE_SIZE_LOWER_THRESHOLD));
}

void test_OpenLogFile_opensNewFileIfBufferNotEmptyButMaxSizeReached(void)
{
    MyCircularBuffer_write('a');

    TEST_ASSERT_TRUE(isNewFileOpenedWhenLogfileSizeIs(
        MAX_FILE_SIZE));
}

void test_WriteSD_writesDataFromCircularBuffer(void)
{
    // given
    // when
    doWriteOperationsNoFlush("file.txt", "x");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("x", READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForFirstData(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "x");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]x", READ_FILE("file.txt"));
}

void test_WriteSD_writesNothingIfNoDataOrTimestamp(void)
{
    // given
    // when
    doWriteOperationsNoFlush("file.txt", "");
    MySD_Flush();
    TEST_ASSERT_TRUE(MyCircularBuffer_isEmpty());
    // then
    TEST_ASSERT_EQUAL_STRING("", READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampOncePerLine(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "xy");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]xy", READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForLineEndings_CR(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "\ra");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]\r[10:34]a", 
        READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForLineEnding_LF(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "\na");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]\n[10:34]a", 
        READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForLineEnding_CRLF(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "\r\na");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]\r\n[10:34]a",
        READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForLineEnding_LFCR(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "\n\ra");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]\n\r[10:34]a",
        READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForMultipleLineEndings_CR(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "\r\ra");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]\r[10:34]\r[10:34]a",
        READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForMultipleLineEndings_LF(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "\n\na");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]\n[10:34]\n[10:34]a",
        READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForMultipleLineEndings_CRLF(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "\r\n\r\na");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]\r\n[10:34]\r\n[10:34]a",
        READ_FILE("file.txt"));
}

void test_WriteSD_writesTimestampForMultipleLineEndings_LFCR(void)
{
    // given
    SET_NEXT_TIMESTAMP("[10:34]");
    // when
    doWriteOperationsNoFlush("file.txt", "\n\r\n\ra");
    MySD_Flush();
    // then
    TEST_ASSERT_EQUAL_STRING("[10:34]\n\r[10:34]\n\r[10:34]a",
        READ_FILE("file.txt"));
}

void test_FlushSD_FlushesAtRightTime(void)
{
    // given
    fake_halTick_setTickValue(FLUSH_TIME_MS);
    doWriteOperationsNoFlush("file.txt", "x");
    // when
    SystemOperations_FlushSD();
    // then
    TEST_ASSERT_EQUAL_STRING("x", READ_FILE("file.txt"));
}

void test_FlushSD_FlushesDataToSD(void)
{   
    // given
    doWriteOperationsNoFlush("file.txt", "x");
    // when
    SystemOperations_FlushSD();
    // then
    TEST_ASSERT_EQUAL_STRING("x", READ_FILE("file.txt"));
}

// Helpers

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

static int isNewFileOpenedWhenLogfileSizeIs(size_t size)
{
    fillUpFile("abc.txt", size);

    SET_NEXT_LOGFILE_NAME("abc.txt");
    SystemOperations_OpenLogFile();

    SET_NEXT_LOGFILE_NAME("next.txt");
    SystemOperations_OpenLogFile();

    return fakefilesystem_fileExists("next.txt");
}

static void doWriteOperationsNoFlush(const char* fileName, const char* data)
{
    SET_NEXT_LOGFILE_NAME("file.txt");
    SystemOperations_OpenLogFile();

    for (size_t i = 0; i < strlen(data); i++)
    {
        MyCircularBuffer_write(data[i]);
    }

    for (size_t i = 0; i < strlen(data); i++)
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