#include "unity.h"
#include "MySD.h"
#include "fakeff.h"
#include "fakefilesystem.h"
#include "fake_led.h"
#include "string.h"

static const BYTE g_Mode = FA_WRITE | FA_READ | FA_OPEN_APPEND;

void setUp(void)
{
    // Close the module to reset internal state variables
    MySD_Close();
    fakeLed_reset();
    fakeff_reset();
}

void tearDown(void)
{
}

void test_OpenLogFileMountsAndOpensAFileAndLedIsOn(void)
{
    //given
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("file"));
    //when
    FRESULT err = MySD_Init("file");
    // then
    TEST_ASSERT_EQUAL_INT(FR_OK, err);
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("file"));
    TEST_ASSERT_TRUE(fakeLed_isOn());
}

void test_OpenLogFileDoesntTurnOnLedIfNoSDCard(void)
{
    // given
    fakeff_setState(NO_SD_CARD);
    // when 
    FRESULT err = MySD_Init("file");
    // then
    TEST_ASSERT_EQUAL_INT(FR_NOT_READY, err);
    TEST_ASSERT_FALSE(fakefilesystem_fileExists("file"));
    TEST_ASSERT_FALSE(fakeLed_isOn());
}

void test_OpenLogFileCanOpenLogFilesConsecutively(void)
{
    FRESULT err = MySD_Init("file");
    FRESULT err2 = MySD_Init("file2");

    TEST_ASSERT_EQUAL_INT(FR_OK, err);
    TEST_ASSERT_EQUAL_INT(FR_OK, err2);

    TEST_ASSERT_TRUE(fakefilesystem_fileExists("file"));
    TEST_ASSERT_TRUE(fakefilesystem_fileExists("file2"));
}

void test_OpenLogFileReturnsErrorAndDoesntTurnOnLedIfFails(void)
{
    char* badPath = "";
    FRESULT err = MySD_Init(badPath);

    TEST_ASSERT_EQUAL_INT(FR_INVALID_PARAMETER, err);
    TEST_ASSERT_FALSE(fakeLed_isOn());
}

void test_MySDWrite_WritesToOpenFileAfterFlushing(void)
{
    // given
    MySD_Init("file");
    // when
    FRESULT err = MySD_Write("hello", 5);
    MySD_Flush();   // also happening to test flush
    fakefilesystem_seek("file", 0);
    // then
    TEST_ASSERT_EQUAL_INT(FR_OK, err);
    const char* result = fakefilesystem_readfile("file");
    TEST_ASSERT_EQUAL_STRING("hello", result);
}

void test_MySDWrite_TogglesLedWithEachWriteIfSuccess(void)
{
    // given
    uint8_t ledState[3];
    MySD_Init("file");
    // when
    ledState[0] = fakeLed_isOn();
    MySD_Write("a", 1);
    ledState[1] = fakeLed_isOn();
    MySD_Write("a", 1);
    ledState[2] = fakeLed_isOn();
    // then
    TEST_ASSERT_TRUE(ledState[0]);
    TEST_ASSERT_FALSE(ledState[1]);
    TEST_ASSERT_TRUE(ledState[2]);
}

void test_MySDWrite_FailsIfNotOpenedLogFileAndDoesntToggleLed(void)
{
    // given
    TEST_ASSERT_FALSE(fakeLed_isOn());
    // when
    FRESULT err = MySD_Write("hello", 5);
    // then
    TEST_ASSERT_FALSE(fakeLed_isOn());
    TEST_ASSERT_EQUAL_INT(FR_INVALID_DRIVE, err);
}

void test_MySDGetOpenedFileSize_returnCorrectFileSize(void)
{
    // given
    MySD_Init("file");
    MySD_Write("abc", 3);
    MySD_Flush();
    // when
    FSIZE_t size = MySD_getOpenedFileSize();
    // then
    TEST_ASSERT_EQUAL_INT(3, size);
}
