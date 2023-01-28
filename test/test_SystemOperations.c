#include "unity.h"
#include "SystemOperations.h"
#include "mock_MySD.h"
#include "mock_MyTimeString.h"

void test_openNewFile(void)
{
    MyTimeString_Init_ExpectAndReturn(0);
    MyTimeString_GetFileName_ExpectAndReturn("hi.txt");
    MySD_Init_ExpectAndReturn("hi.txt", FR_OK);

    TEST_ASSERT_EQUAL(SO_SUCCESS, SystemOperations_OpenNewFile());
}

void test_SendTimestamp(void)
{
    uint8_t data = 'a';
    
    MyTimeString_GetTimeStamp_ExpectAndReturn("[26-11-22]");    // get timestamp
    MySD_WriteString_ExpectAndReturn("[26-11-22]", FR_OK);      // write it to SD
    MySD_Write_ExpectAndReturn(&data, 1, FR_OK);

    SystemOperations_TransferByteToSD(data);
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