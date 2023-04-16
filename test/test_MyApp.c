#include "unity.h"
#include "MyApp.h"
#include "mock_MyCLI.h"
#include "mock_AppDecider.h"
#include "mock_main.h"
#include "Loop.h"

#include "MyScheduler.h"
#include "fake_stm32f0xx_hal.h"
// #include "fake_SDCard.h"
#include "fake_myTimeString.h"
#include "SystemOperations.h"
#include "DataHolder.h"
#include "MyCircularBuffer.h"
#include "FileNameIterator.h"
#include "MySD.h"
#include "fake_led.h"
#include "fakeff.h"
#include "fakefilesystem.h"

#include <string.h>

#define MIN_FILES_OPENED 2

// Helpers
void expectMySchedulerInitHelper(void);
void setUp_SerialSnooperAppTests(void);
void streamNewDataIn(char* data);
void fillDataBuffer(unsigned int amount);
void fillDataBuffer(unsigned int amount);
void fillUpFile(const char* fileName, size_t amount);
int TheApplication;    // could be CLI or serial snooper

/* Tests for the CLI App */

void test_App_CLI(void)
{
    TEST_IGNORE();  // these should be rewritten

    #if 0
    LOOP_COUNT(3);  // expecting 3 times round the loop
    CubeMX_SystemInit_Expect(CMX_FATFS);
    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_CLI);

    MyCLI_Init_Expect();
    // expecting 3 calls
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();

    runInfiniteLoop(&TheApplication);
    #endif
}

/* Tests for the Serial Snooping App */

void setUp(void)
{
    LOOP_COUNT(1);  // expecting X times round the loop by default
    // A hacky set up routine - older tests
    // depended heavily on mocks. Leaving these
    // packacged up and hidden here for the time being
    expectMySchedulerInitHelper();
    // reset dependencies
    fakeff_reset();
    fake_halTick_reset();
    fake_myTimeString_reset();
    MyCircularBuffer_close();
    MyCircularBuffer_init();
    TheApplication = initialise();
}

void tearDown(void)
{
    MySD_Close();
}

void test_App_SystemShallOpenAFileWhenItRunsForTheFirstTime(void)
{
    // given
    bool existsBefore = fakefilesystem_fileExists("newfile.txt");
    // when
    fake_myTimeString_setFileName("newfile.txt");
    runInfiniteLoop(&TheApplication);
    // then
    TEST_ASSERT_FALSE(existsBefore);
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("newfile.txt"));
}

void test_App_SystemShallOpenANewFileWhenSizeOfFileReachesMax(void)
{
    // given
    fake_myTimeString_setFileName("newfile.txt");
    fillUpFile("newfile.txt", FILE_SIZE_LOWER_THRESHOLD);
    LOOP_COUNT(1);
    runInfiniteLoop(&TheApplication);
    LOOP_COUNT(1);
    fake_myTimeString_setFileName("next.txt");
    // when
    runInfiniteLoop(&TheApplication);
    // then
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("newfile.txt"));
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("next.txt"));
}

void test_App_SystemShallNotOpenANewFileYetWhenThereIsDataInTheBuffer(void)
{
    // given
    streamNewDataIn("hello one two");
    fake_myTimeString_setFileName("newfile.txt");
    fillUpFile("newfile.txt", FILE_SIZE_LOWER_THRESHOLD);
    LOOP_COUNT(1);
    runInfiniteLoop(&TheApplication);
    LOOP_COUNT(1);
    fake_myTimeString_setFileName("next.txt");
    TEST_ASSERT_FALSE(MyCircularBuffer_isEmpty());
    // when
    runInfiniteLoop(&TheApplication);
    // then
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("newfile.txt"));
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("next.txt"));
}

void test_App_SystemCanForceOpenANewFileEvenWhenDataInBufferIfMaxFileSizeReached(void)
{
    // given
    streamNewDataIn("hello one two");
    fake_myTimeString_setFileName("newfile.txt");
    fillUpFile("newfile.txt", MAX_FILE_SIZE);
    LOOP_COUNT(1);
    runInfiniteLoop(&TheApplication);
    LOOP_COUNT(1);
    fake_myTimeString_setFileName("next.txt");
    TEST_ASSERT_FALSE(MyCircularBuffer_isEmpty());
    // when
    runInfiniteLoop(&TheApplication);
    // then
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("newfile.txt"));
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("next.txt"));
}

void test_App_ShallNotFlushToSDCardIfTimeNotRight(void)
{
    // given
    fake_halTick_setTickValue(FLUSH_TIME_MS - 1);
    fake_myTimeString_setFileName("newfile.txt");
    streamNewDataIn("hello, this is a new message");
    // when
    runInfiniteLoop(&TheApplication);
    // then
    TEST_ASSERT_EQUAL_STRING("", READ_FILE("newfile.txt"));
}

void test_App_ShallFlushToSDCardAsTimePasses(void)
{
    // given
    LOOP_COUNT(50);
    fake_myTimeString_setFileName("newfile.txt");
    streamNewDataIn("hello, this is a new message");
    fake_halTick_enableAutoIncrement(FLUSH_TIME_MS);
    // when
    runInfiniteLoop(&TheApplication);
    // then
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("newfile.txt"));
    TEST_ASSERT_EQUAL_STRING("hello, this is a new message", 
        READ_FILE("newfile.txt"));
}

void test_App_firstFileOpenedWasTheLastOneWrittenTo


#if 0
void test_App_OpensOneFileToReadAndOneToWrite(void)
{
    setUp_SerialSnooperAppTests();
    
    LOOP_COUNT(MAX_FILE_SIZE + 10); // configure to run some amount of times

    runInfiniteLoop(&TheApplication);

    TEST_ASSERT_EQUAL_INT(MIN_FILES_OPENED, fake_SDCard_totalNumOfFilesOpened());
}

void test_App_ShallOpenANewFileAfterUpperLimitWhenData(void)
{
    setUp_SerialSnooperAppTests();
    
    LOOP_COUNT(MAX_FILE_SIZE + 10); // configure to run some amount of times

    fillDataBuffer(MAX_FILE_SIZE);

    runInfiniteLoop(&TheApplication);

    TEST_ASSERT_EQUAL_INT(MIN_FILES_OPENED + 1, fake_SDCard_totalNumOfFilesOpened());
}

void test_App_ShallOpenANewFileAfterLowerLimitWhenNoData(void)
{
    setUp_SerialSnooperAppTests();
    
    LOOP_COUNT(MAX_FILE_SIZE + 10); // configure to run some amount of times

    fillDataBuffer(FILE_SIZE_LOWER_THRESHOLD);

    runInfiniteLoop(&TheApplication);

    TEST_ASSERT_EQUAL_INT(MIN_FILES_OPENED + 1, fake_SDCard_totalNumOfFilesOpened());
}

void test_firstFileOpenedIsOneReadFromSDCard(void)
{
    setUp_SerialSnooperAppTests();

    fake_myTimeString_setFileName("ABC");
    fake_SDCard_helperWriteFileData("prevFile", 9);

    LOOP_COUNT(10); // configure to run some amount of times

    runInfiniteLoop(&TheApplication);

    const char* name = fake_SDCard_getOpenFileName();

    TEST_ASSERT_EQUAL_STRING("prevFile", name);
}

#endif


/* Helpers */

void expectMySchedulerInitHelper(void)
{
    CubeMX_SystemInit_Expect(CMX_FATFS);
    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_SNOOPING);
    CubeMX_SystemInit_Expect(CMX_UART);
}

void streamNewDataIn(char* data)
{
    while (*data)
    {
        MyCircularBuffer_write(*data);
        data++;
    }
}

void fillDataBuffer(unsigned int amount)
{
    // fill up data up to lower limit only
    for (int i = 0; i < amount; i++)
    {
        streamNewDataIn("b");
    }
}

void fillUpFile(const char* fileName, size_t amount)
{
    fakefilesystem_createFile(fileName);
    for (size_t i = 0; i < amount; i++)
    {
        fakefilesystem_writeFile(fileName, "x");
    }
}