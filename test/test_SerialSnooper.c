#include "unity.h"
#include "SerialSnooper.h"
#include "mock_MySD.h"
#include "mock_MyRTC.h"

#include "mock_stm32f0xx_hal_uart.h"

#define CHANGE_ME   "06-11-22-1131.txt"

void test_SerialSnooper_Init(void)
{
    MyRTC_Init_ExpectAndReturn(0);
    MyRTC_GetTimeStr_ExpectAndReturn(CHANGE_ME);
    MySD_Init_ExpectAndReturn(CHANGE_ME, FR_OK);

    SerialSnooper_Init();
}

void test_SerialSnooper_Run(void)
{

}

/*
- [x] Opens a file with a name
    - [x] Name might be related to the date-time opened
        - [ ] Get the RTC working
- [ ] Kick-starts a UART interrupt routine
    - [ ] UART interrupt routine will handle filling in the write buffer
        - [ ] Will generate date and time and fill it in
        - [ ] Write buffer is a circular buffer w/X size
        - [ ] The SD card writer will consume circular buffer
- [ ] If there is data available in the write buffer,
    will write to the SD card. (Not sure how this will behave w/circ buf)
        - [ ] 
*/