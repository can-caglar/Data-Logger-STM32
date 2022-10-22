#include "unity.h"
#include "mock_ff.h"
#include "MySD.h"
#include "string.h"

// TODO fix this to make it static again (when a close function done)
extern uint8_t mounted;

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
    FATFS FatFs = { 0 };
    FIL file = { 0 };
    BYTE mode = FA_WRITE | FA_OPEN_APPEND;

    f_mount_ExpectAndReturn(&FatFs, "", 1, FR_OK);
    f_open_ExpectAndReturn(&file, "testFile.txt", mode, FR_OK);

    FRESULT err = MySD_Init("testFile.txt");

    TEST_ASSERT_EQUAL_INT(FR_OK, err);
}


void test_init_sd_when_no_mount(void)
{
    FATFS FatFs = { 0 };

    f_mount_ExpectAndReturn(&FatFs, "", 1, FR_NOT_READY);

    FRESULT err = MySD_Init("testFile.txt");

    TEST_ASSERT_EQUAL_INT(FR_NOT_READY, err);
}

void test_init_sd_twice(void)
{
    FIL file = { 0 };
    BYTE mode = FA_WRITE | FA_OPEN_APPEND;

    // The first call (ignore what happens but make it pass)
    f_mount_IgnoreAndReturn(FR_OK);
    f_open_IgnoreAndReturn(FR_OK);

    MySD_Init("testFile.txt");

    f_mount_StopIgnore();
    f_open_StopIgnore();

    // The second call
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

void test_write_sd_all_bytes_written(void)
{
    FIL file = { 0 };
    char buf[] = "test buffer";
    int len = strlen(buf);
    unsigned int bytesWrote = 0;

    f_write_ExpectAndReturn(&file, buf, len, &bytesWrote, FR_OK);
    f_write_ReturnThruPtr_bw(&len);

    FRESULT err = MySD_Write(buf);
    TEST_ASSERT_EQUAL_INT(FR_OK, err);
}

/*

- [x] Init will mount SD card then open a file for writing
- [x] Init again will not mount again and will close
    the open file and open a new one
- [ ] Write will write to the end of that file
    - Write without init will return error
    - A successful write will return 0
- [x] Close file shall close the file and unmount SD
    - Even if mount failed, will do the unmount routine
    - Even if file not opened, will close

*/
