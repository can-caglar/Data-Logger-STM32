#include "unity.h"
#include "fake_SDCard.h"
#include <string.h>

// Expecting that all newly opened files are files that
// were created for the first time.
const char expectedNewlyOpenFileData[FAKE_MAX_FILE_SIZE] = { 0 };

void setUp(void)
{
    fake_SDCard_reset();
}

void test_SdCardNumFilesOpenDepends(void)
{
    TEST_ASSERT_EQUAL_INT(0, fake_SDCard_numFilesOpen());

    MySD_Init("file.txt");

    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_numFilesOpen());

    MySD_Close();

    TEST_ASSERT_EQUAL_INT(0, fake_SDCard_numFilesOpen());
}

void test_fake_SDCard_reset_willResetEverything(void)
{
    MySD_Init("file.txt");
    MySD_WriteString("hello");
    MySD_Flush();
    
    // The reset
    fake_SDCard_reset();

    // The tests
    TEST_ASSERT_EQUAL_INT(0, fake_SDCard_numFilesOpen());    
    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getOpenFileName());    
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedNewlyOpenFileData, fake_SDCard_getFileData(), FAKE_MAX_FILE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, fake_SDCard_totalNumOfFilesOpened());

    MySD_Flush();   // flush again shall not flush anything
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedNewlyOpenFileData, fake_SDCard_getFileData(), FAKE_MAX_FILE_SIZE);
}

void test_fake_SDCard_reset_willResetFilePointer(void)
{
    const char* testStr = "hello";

    MySD_Init("file.txt");
    MySD_WriteString(testStr);
    MySD_Flush();
    
    // The reset
    fake_SDCard_reset();

    // Write again
    MySD_Init("file.txt");
    MySD_WriteString(testStr);
    MySD_Flush();

    // Expect the same
    TEST_ASSERT_EQUAL_CHAR_ARRAY(testStr, 
        fake_SDCard_getFileData(), sizeof(testStr));
}

void test_fake_SDCard_reset_willResetActualFilePointer(void)
{
    MySD_Init("file.txt");
    MySD_WriteString("hi");
    MySD_Flush();

    fake_SDCard_reset();

    TEST_ASSERT_EQUAL_INT(0, MySD_getOpenedFileSize());
}

void test_fake_SDCard_init_willResetFilePointer(void)
{
    const char* testStr = "hello";

    MySD_Init("file.txt");
    MySD_WriteString(testStr);
    MySD_Flush();

    // Write again
    MySD_Init("file.txt");

    TEST_ASSERT_EQUAL_INT(0, MySD_getOpenedFileSize());
    MySD_WriteString(testStr);
    MySD_Flush();

    // Expect the same
    TEST_ASSERT_EQUAL_CHAR_ARRAY(testStr, 
        fake_SDCard_getFileData(), sizeof(testStr));
}

void test_SdCardOpenFileNameCanBeAccessedAndChangesAccordingly(void)
{
    // initially no name
    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getOpenFileName());

    MySD_Init("newfile.txt");

    TEST_ASSERT_EQUAL_STRING("newfile.txt", fake_SDCard_getOpenFileName());

    MySD_Init("newfile2.txt");

    TEST_ASSERT_EQUAL_STRING("newfile2.txt", fake_SDCard_getOpenFileName());
}

void test_SdCardClosedFileHasNoName(void)
{
    MySD_Init("newfile.txt");

    MySD_Close();

    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getOpenFileName());
}

void test_SdCardDataWontBeWrittenUntilFlushed(void)
{
    MySD_Init("file.txt");
    MySD_WriteString("hello");

    TEST_ASSERT_EQUAL_CHAR_ARRAY(expectedNewlyOpenFileData, 
        fake_SDCard_getFileData(), FAKE_MAX_FILE_SIZE);
}

void test_SdCardDataWillBeWrittenWhenFlushedUsingWriteStringAPI(void)
{
    const char thisStr[] = "hello";
    MySD_Init("file.txt");
    MySD_WriteString(thisStr);
    MySD_Flush();

    TEST_ASSERT_EQUAL_CHAR_ARRAY(thisStr, 
        fake_SDCard_getFileData(), strlen(thisStr));
}

void test_SdCardDataWillBeWrittenWhenFlushedUsingWriteByteAPI(void)
{
    const char thisStr[] = "hello";
    MySD_Init("file.txt");
    MySD_Write(thisStr, strlen(thisStr));
    MySD_Flush();

    TEST_ASSERT_EQUAL_CHAR_ARRAY(thisStr, 
        fake_SDCard_getFileData(), strlen(thisStr));
}

void test_SdCardDataWriteMultipleTimesConcatenates(void)
{
    const char thisStr[20] = "hello";
    const char str2[] = " world!";

    MySD_Init("file.txt");
    MySD_Write(thisStr, strlen(thisStr));
    MySD_Write(str2, strlen(str2));
    MySD_Flush();

    TEST_ASSERT_EQUAL_CHAR_ARRAY("hello world!", 
        fake_SDCard_getFileData(), strlen(thisStr));
}

void test_SdCardWriteDataWith0sAlsoGetsWritten(void)
{
    const uint8_t expectedData[] = {0, 1, 2, 0, 3};
    uint8_t size = sizeof(expectedData);

    MySD_Init("file.txt");
    MySD_Write(expectedData, size);
    MySD_Flush();

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedData, 
        fake_SDCard_getFileData(), size);
}

void test_SdCardFileSizeDependsOnAmountWritten(void)
{
    MySD_Init("file.txt");
    
    for (uint32_t i = 0; i < FAKE_MAX_FILE_SIZE; i++)
    {
        MySD_WriteString("a");
    }

    MySD_Flush();

    TEST_ASSERT_EQUAL_UINT32(FAKE_MAX_FILE_SIZE, MySD_getOpenedFileSize());
}

void test_writeToNonOpenFileDoesNothing(void)
{
    // Do not open file:  MySD_Init("file.txt");
    MySD_WriteString("hello!");
    MySD_Flush();

    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getFileData());
}

void test_sdCardIsEmptyCanBeQueried(void)
{
    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_isFileEmpty());

    MySD_Init("file");
    MySD_WriteString("hello!");
    MySD_Flush();

    TEST_ASSERT_EQUAL_INT(0, fake_SDCard_isFileEmpty());
}

void test_sdCardNumberOfFilesOpenedIncrementsEachCallToSDInit(void)
{
    TEST_ASSERT_EQUAL_INT(0, fake_SDCard_totalNumOfFilesOpened());
    
    MySD_Init("file");
    
    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_totalNumOfFilesOpened());

    MySD_Init("file");

    TEST_ASSERT_EQUAL_INT(2, fake_SDCard_totalNumOfFilesOpened());
}

void test_ApiAlwaysReturnsWhatWeWant(void)
{
    fake_SDCard_toReturn(FR_DISK_ERR);

    TEST_ASSERT_EQUAL_INT(FR_DISK_ERR, MySD_Init("file.txt"));
    TEST_ASSERT_EQUAL_INT(FR_DISK_ERR, MySD_WriteString("hello"));
    TEST_ASSERT_EQUAL_INT(FR_DISK_ERR, MySD_Write("abc", 3));
    TEST_ASSERT_EQUAL_INT(FR_DISK_ERR, MySD_Flush());
}

void test_ApiAlwaysReturnsFR_OK_afterInit(void)
{
    TEST_ASSERT_EQUAL_INT(FR_OK, MySD_WriteString("hello"));
    TEST_ASSERT_EQUAL_INT(FR_OK, MySD_Init("file.txt"));
    TEST_ASSERT_EQUAL_INT(FR_OK, MySD_Write("abc", 3));
    TEST_ASSERT_EQUAL_INT(FR_OK, MySD_Flush());
}

void test_IfReturningErrorInitDoesNothing(void)
{
    fake_SDCard_toReturn(FR_DISK_ERR);
    MySD_Init("file.txt");
    TEST_ASSERT_EQUAL_INT(0, fake_SDCard_numFilesOpen());
}

void test_IfReturningErrorFlushDoesNothing(void)
{
    MySD_Init("file.txt");
    MySD_WriteString("hello");

    fake_SDCard_toReturn(FR_DISK_ERR);
    MySD_Flush();

    // And this should do nothing
    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_numFilesOpen());
    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getFileData());
}

void test_IfReturningErrorWriteDoesNothing(void)
{
    MySD_Init("file.txt");

    fake_SDCard_toReturn(FR_DISK_ERR);
    MySD_WriteString("hello");
    MySD_Write("hi", 2);

    fake_SDCard_toReturn(FR_OK);
    MySD_Flush();

    // And this should do nothing
    TEST_ASSERT_EQUAL_INT(1, fake_SDCard_numFilesOpen());
    TEST_ASSERT_EQUAL_STRING("", fake_SDCard_getFileData());
}

void test_FileSizeIncrementOnlyAfterFlush(void)
{
    MySD_Init("file.txt");
    MySD_WriteString("hi");

    // size shall be 0
    TEST_ASSERT_EQUAL_INT(0, MySD_getOpenedFileSize());

    MySD_Flush();

    // size is now updated
    TEST_ASSERT_EQUAL_INT(2, MySD_getOpenedFileSize());
}


/* 
The fake SD module shall hold an internal state
for the data written to the SD card.

- [x] It shall be able to tell how many files are open.
- [x] Closing a file will reduce number of files open
- [x] Opening a file should close old file and open new one
- [x] It can return the name of the file that is currently open.
- [x] It shall be able to write to an open file and read from it.
- [x] Writing to a file that is not open does not write to file
- [x] File size is set by the module
- [x] The data will not be written to file unless flushed

*/
