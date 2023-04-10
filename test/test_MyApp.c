#include "unity.h"
#include "MyApp.h"
#include "mock_MyCLI.h"
#include "mock_AppDecider.h"
#include "mock_main.h"
#include "Loop.h"

#include "MyScheduler.h"
#include "fake_stm32f0xx_hal.h"
#include "fake_SDCard.h"
#include "fake_myTimeString.h"
#include "SystemOperations.h"
#include "DataHolder.h"
#include "MyCircularBuffer.h"
#include "FileNameIterator.h"

#include <string.h>

#define MIN_FILES_OPENED 2

// Helpers
void expectMySchedulerInitHelper(void);
void setUp_SerialSnooperAppTests(void);
void streamNewData(char* data);
void fillDataBuffer(unsigned int amount);

/* Tests for the CLI App */

void test_App_CLI(void)
{
    LOOP_COUNT(3);  // expecting 3 times round the loop

    CubeMX_SystemInit_Expect(CMX_FATFS);
    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_CLI);

    MyCLI_Init_Expect();
    // expecting 3 calls
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();
    MyCLI_Run_Expect();

    runApp();
}

void test_RewriteTheseTestsToUseNewFatfsModule(void)
{
    TEST_IGNORE();
}

/* Tests for the Serial Snooping App */

void test_App_OpeningNewFiles(void)
{
    setUp_SerialSnooperAppTests();

    // set up file name
    fake_myTimeString_setFileName("newfile.txt");

    // First time running, open new file
    runApp();

    TEST_ASSERT_EQUAL_STRING("newfile.txt", fake_SDCard_getOpenFileName());
    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_numFilesOpen());
}

void test_App_ShallWriteNothingToSDCardIfNoData(void)
{
    setUp_SerialSnooperAppTests();

    LOOP_COUNT(10); // configure to run some amount of times

    runApp();

    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_isFileEmpty());
}

void test_App_ShallNotFlushUntilItsTime(void)
{
    setUp_SerialSnooperAppTests();

    LOOP_COUNT(10); // configure to run some amount of times

    streamNewData("hello, this is a new message");
    fake_halTick_setTickValue(FLUSH_TIME_MS - 1);

    runApp();

    // expect no data
    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getFileData());
}

void test_App_ShallWriteAllDataToSDAsLongAsItIsFlushing(void)
{
    setUp_SerialSnooperAppTests();

    LOOP_COUNT(strlen("hello, this is a new message")); // configure to run some amount of times

    streamNewData("hello, this is a new message");
    fake_myTimeString_setTimestamp("[1] ");
    fake_halTick_setTickValue(0);
    fake_halTick_enableAutoIncrement(FLUSH_TIME_MS);

    runApp();

    // output is the time stamp + string
    TEST_ASSERT_EQUAL_STRING("[1] hello, this is a new message", 
        fake_SDCard_getFileData());
}

void test_App_OpensOneFileToReadAndOneToWrite(void)
{
    setUp_SerialSnooperAppTests();
    
    LOOP_COUNT(MAX_FILE_SIZE + 10); // configure to run some amount of times

    runApp();

    TEST_ASSERT_EQUAL_INT(MIN_FILES_OPENED, fake_SDCard_totalNumOfFilesOpened());
}

void test_App_ShallOpenANewFileAfterUpperLimitWhenData(void)
{
    setUp_SerialSnooperAppTests();
    
    LOOP_COUNT(MAX_FILE_SIZE + 10); // configure to run some amount of times

    fillDataBuffer(MAX_FILE_SIZE);

    runApp();

    TEST_ASSERT_EQUAL_INT(MIN_FILES_OPENED + 1, fake_SDCard_totalNumOfFilesOpened());
}

void test_App_ShallOpenANewFileAfterLowerLimitWhenNoData(void)
{
    setUp_SerialSnooperAppTests();
    
    LOOP_COUNT(MAX_FILE_SIZE + 10); // configure to run some amount of times

    fillDataBuffer(FILE_SIZE_LOWER_THRESHOLD);

    runApp();

    TEST_ASSERT_EQUAL_INT(MIN_FILES_OPENED + 1, fake_SDCard_totalNumOfFilesOpened());
}

void test_firstFileOpenedIsOneReadFromSDCard(void)
{
    setUp_SerialSnooperAppTests();

    fake_myTimeString_setFileName("ABC");
    fake_SDCard_helperWriteFileData("prevFile", 9);

    LOOP_COUNT(10); // configure to run some amount of times

    runApp();

    const char* name = fake_SDCard_getOpenFileName();

    TEST_ASSERT_EQUAL_STRING("prevFile", name);
}

/*********** Helpers ***********/

void expectMySchedulerInitHelper(void)
{
    CubeMX_SystemInit_Expect(CMX_FATFS);
    AppDecider_Init_Expect();
    AppDecider_Decide_ExpectAndReturn(APP_SNOOPING);
    CubeMX_SystemInit_Expect(CMX_UART);
}

void setUp_SerialSnooperAppTests(void)
{
    // A hacky set up routine - older tests
    // depended heavily on mocks. Leaving these
    // packacged up and hidden here for the time being

    LOOP_COUNT(1);  // expecting X times round the loop
    
    expectMySchedulerInitHelper();  // the init sequence

    fake_SDCard_reset();    // clean slate SD card
    fake_myTimeString_reset();  // clean slate time string
    MyCircularBuffer_close();
    MyCircularBuffer_init();
}

void streamNewData(char* data)
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
        streamNewData("b");
    }
}

/*
TODO: 
    Perhaps begin from scratch.

    Write some tests, maybe then create the fake modules.

*/