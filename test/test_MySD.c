#include "unity.h"
#include "mock_ff.h"
#include "MySD.h"
#include "string.h"
#include "mock_LED.h"

// TODO: refactor tests. Ideally we would have faked fatfs

void initialiseSD(void);

void setUp(void)
{
}

void tearDown(void)
{
    // Close the module to reset internal state variables
    f_close_IgnoreAndReturn(FR_OK);
    f_mount_IgnoreAndReturn(FR_OK);
    MySD_Close();
}

void test_init_sd_when_all_ok(void)
{
    initialiseSD();
}


void test_init_sd_when_no_mount(void)
{
    FATFS FatFs = { 0 };

    f_mount_ExpectAndReturn(&FatFs, "", 1, FR_NOT_READY);

    FRESULT err = MySD_Init("testFile.txt");

    TEST_ASSERT_EQUAL_INT(FR_NOT_READY, err);
}

void test_init_sd_when_open_fails(void)
{
    FATFS FatFs = { 0 };
    FIL file = { 0 };
    BYTE mode = FA_WRITE | FA_OPEN_APPEND;

    f_mount_ExpectAndReturn(&FatFs, "", 1, FR_OK);
    f_open_ExpectAndReturn(&file, "testFile.txt", mode, FR_NOT_READY);

    FRESULT err = MySD_Init("testFile.txt");

    TEST_ASSERT_EQUAL_INT(FR_NOT_READY, err);
}

void test_init_sd_twice_closes_file_opens_another(void)
{
    FIL file = { 0 };
    BYTE mode = FA_WRITE | FA_OPEN_APPEND;

    // The first call:
    f_mount_IgnoreAndReturn(FR_OK);
    f_open_IgnoreAndReturn(FR_OK);
    led_init_Ignore();
    led_on_Ignore();

    MySD_Init("testFile.txt");

    f_mount_StopIgnore();
    f_open_StopIgnore();

    // The second call:
    f_close_ExpectAndReturn(&file, FR_OK);
    f_open_ExpectAndReturn(&file, "testFile2.txt", mode, FR_OK);

    FRESULT err = MySD_Init("testFile2.txt");

    TEST_ASSERT_EQUAL_INT(FR_OK, err);
}

void test_close_sd(void)
{
    FIL file = { 0 };
    f_close_ExpectAndReturn(&file, FR_OK);
    f_mount_ExpectAndReturn(NULL, "", 0, FR_OK);
    MySD_Close();
}

void test_write_sd_string_doesnt_call_fatfs_if_data_less_than_buf(void)
{
    FIL file = { 0 };
    uint32_t bufSizeLessThanMax = WRITE_BUFFER_SIZE - 1;
    char buf[bufSizeLessThanMax];
    memset(buf, 'X', bufSizeLessThanMax);
    buf[bufSizeLessThanMax] = '\0'; // null terminate for MySD_WriteString
    unsigned int bytesWrote = 0;

    // don't expect
    // f_write_ExpectAndReturn(&file, buf, len, &bytesWrote, FR_OK);

    FRESULT err = MySD_WriteString(buf);
    TEST_ASSERT_EQUAL_INT(FR_OK, err);
}


void test_write_sd_string_calls_fatfs_at_max_buf_size_only(void)
{
    initialiseSD();

    FIL file = { 0 };
    char* oneLengthStr = "X";

    unsigned int bytesWrote = 0;
    // build expected buffer
    char expectedBuf[WRITE_BUFFER_SIZE];
    memset(expectedBuf, 'X', WRITE_BUFFER_SIZE);

    // send 1 character string at a time, but less than buf size
    for (uint32_t i = 0; i < WRITE_BUFFER_SIZE - 1; i++)
    {
        MySD_WriteString(oneLengthStr);
    }

    // expect to pass in a buffer containing all data written
    f_write_ExpectWithArrayAndReturn(&file, 1,
        expectedBuf, WRITE_BUFFER_SIZE,
        WRITE_BUFFER_SIZE,
        &bytesWrote, 1,
        FR_OK);

    led_toggle_Expect();    // still toggle

    MySD_WriteString(oneLengthStr);
}

void test_write_sd_doesnt_call_fatfs_if_data_less_than_max_buf(void)
{
    initialiseSD();

    FIL file = { 0 };
    uint32_t sizeLessThanMax = WRITE_BUFFER_SIZE - 2;
    char buf[sizeLessThanMax];
    unsigned int bytesWrote = 0;

    // don't expect
    // f_write_ExpectAndReturn(&file, buf, buflen, &bytesWrote, FR_OK);

    FRESULT err = MySD_Write(buf, sizeLessThanMax);
    TEST_ASSERT_EQUAL_INT(FR_OK, err);
}

void test_write_sd_calls_fatfs_if_data_reaches_size_of_max_buf(void)
{
    initialiseSD();

    FIL file = { 0 };
    FRESULT err;
    char data = 8;
    char expectedData[WRITE_BUFFER_SIZE];
    unsigned int bytesWrote = 0;
    memset(expectedData, 8, WRITE_BUFFER_SIZE);

    // don't expect
    // f_write_ExpectAndReturn(&file, buf, buflen, &bytesWrote, FR_OK);

    for (uint32_t i = 0; i < WRITE_BUFFER_SIZE - 1; i++)
    {
        err = MySD_Write(&data, 1);
        TEST_ASSERT_EQUAL_INT(FR_OK, err);
    }
    
    // now expect
    f_write_ExpectWithArrayAndReturn(&file, 1,
        expectedData, WRITE_BUFFER_SIZE,
        WRITE_BUFFER_SIZE,
        &bytesWrote,
        1,
        FR_OK);
    led_toggle_Expect();

    MySD_Write(&data, 1);
}

void test_write_sd_calls_fatfs_twice_if_data_higher_than_max(void)
{
    initialiseSD();

    FIL file = { 0 };
    FRESULT err;
    char data = 8;
    uint32_t doubleTheSize = WRITE_BUFFER_SIZE * 2;
    char expectedData[doubleTheSize];
    unsigned int bytesWrote = 0;
    memset(expectedData, 8, doubleTheSize);

    // don't expect
    // f_write_ExpectAndReturn(&file, buf, buflen, &bytesWrote, FR_OK);

    // now expect
    f_write_ExpectWithArrayAndReturn(&file, 1,
        expectedData, WRITE_BUFFER_SIZE,
        WRITE_BUFFER_SIZE,
        &bytesWrote,
        1,
        FR_OK);

    led_toggle_Expect();

    f_write_ExpectWithArrayAndReturn(&file, 1,
        expectedData, WRITE_BUFFER_SIZE,
        WRITE_BUFFER_SIZE,
        &bytesWrote,
        1,
        FR_OK);

    led_toggle_Expect();

    MySD_Write(expectedData, doubleTheSize);
}

void test_whenWritingLess(void)
{
    
}

void test_write_sd_calls_fatfs_twice_if_data_higher_than_max(void)
{
    initialiseSD();

    FIL file = { 0 };
    FRESULT err;
    char data = 8;
    uint32_t doubleTheSize = WRITE_BUFFER_SIZE * 2;
    char expectedData[doubleTheSize];
    unsigned int bytesWrote = 0;
    memset(expectedData, 8, doubleTheSize);

    // don't expect
    // f_write_ExpectAndReturn(&file, buf, buflen, &bytesWrote, FR_OK);

    // now expect
    f_write_ExpectWithArrayAndReturn(&file, 1,
        expectedData, WRITE_BUFFER_SIZE,
        WRITE_BUFFER_SIZE,
        &bytesWrote,
        1,
        FR_OK);

    led_toggle_Expect();

    f_write_ExpectWithArrayAndReturn(&file, 1,
        expectedData, WRITE_BUFFER_SIZE,
        WRITE_BUFFER_SIZE,
        &bytesWrote,
        1,
        FR_OK);

    led_toggle_Expect();

    MySD_Write(expectedData, doubleTheSize);
}

void test_write_sd_fails_doesnt_toggle_led(void)
{
    char buf[WRITE_BUFFER_SIZE] = { 0 };
    unsigned int bytesWrote = 0;

    f_write_IgnoreAndReturn(FR_DISK_ERR);

    FRESULT err = MySD_Write(buf, WRITE_BUFFER_SIZE);
    TEST_ASSERT_EQUAL_INT(FR_DISK_ERR, err);
}

void test_write_string_fails_doesnt_toggle_led(void)
{
    char strBuf[WRITE_BUFFER_SIZE];
    memset(strBuf, 'X', WRITE_BUFFER_SIZE);
    strBuf[WRITE_BUFFER_SIZE] = '\0';
    unsigned int bytesWrote = 0;

    f_write_IgnoreAndReturn(FR_DISK_ERR);

    FRESULT err = MySD_WriteString(strBuf);
    TEST_ASSERT_EQUAL_INT(FR_DISK_ERR, err);
}

void test_mysd_flush(void)
{
    FIL file = { 0 };

    f_sync_ExpectAndReturn(&file, FR_OK);

    FRESULT err = MySD_Flush();
    TEST_ASSERT_EQUAL_INT(FR_OK, err);
}

void test_mysd_size(void)
{
    FIL file = { 0 };
    // funny hack to get around encapsulating the FIL object
    FIL fileWithSize = { 0 };
    fileWithSize.obj.objsize = 33;

    BYTE mode = FA_WRITE | FA_OPEN_APPEND;

    f_mount_IgnoreAndReturn(0);
    f_open_ExpectAndReturn(&file,
        "filename", mode, FR_OK);
    // here's the hack, the encapsulated file will now
    // have a size of 33
    f_open_ReturnMemThruPtr_fp(&fileWithSize, sizeof(FIL));
    led_init_Ignore();
    led_on_Ignore();

    FRESULT err = MySD_Init("filename");

    FSIZE_t size = MySD_getOpenedFileSize();

    TEST_ASSERT_EQUAL(33, size);
}

/**************** Helpers ***************/

void initialiseSD(void)
{
    FATFS FatFs = { 0 };
    FIL file = { 0 };
    BYTE mode = FA_WRITE | FA_OPEN_APPEND;

    f_mount_ExpectAndReturn(&FatFs, "", 1, FR_OK);
    f_open_ExpectAndReturn(&file, "testFile.txt", mode, FR_OK);
    led_init_Expect();
    led_on_Expect();

    FRESULT err = MySD_Init("testFile.txt");

    TEST_ASSERT_EQUAL_INT(FR_OK, err);
}

/*

- [x] Init will mount SD card then open a file for writing
- [x] Init again will not mount again and will close
    the open file and open a new one
- [x] Write will write to the end of that file
    - Write without init will return error
    - A successful write will return 0
- [x] Close file shall close the file and unmount SD
    - Even if mount failed, will do the unmount routine
    - Even if file not opened, will close

*/
