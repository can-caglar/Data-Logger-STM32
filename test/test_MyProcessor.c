#include <string.h>
#include "unity.h"
#include "MyProcessor.h"
#include "mock_MySD.h"
#include "mock_MyDipSwitch.h"
#include "mock_LED.h"
#include "mock_MyCircularBuffer.h"
#include "mock_MyTimeString.h"
#include "mock_MyRTC.h"

#define LIST_OF_CMDS \
"help seeAll writeSD readDip led cirbufWrite cirbufRead getTime"

void setUp()
{

}

void tearDown()
{
}

void test_MyProcessor_Init(void)
{
}

// ******************** General ********************

void test_MyProcessor_sendingBadCommand(void)
{
    MyProcessor_HandleCommandWithString("randomCmd");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Command doesn't exist: randomCmd", resp);
}

void test_MyProcessor_sendingEmptyStringGivesUniqueResponse(void)
{
    MyProcessor_HandleCommandWithString("");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Received no commands.", resp);
}

void test_MyProcessor_sendingVeryLongStringGivesUniqueResponse(void)
{
    MyProcessor_HandleCommandWithString("01234567890123456789");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Command too long!", resp);
}

// ******************** 'help' command ********************

void test_MyProcessor_help_ShowsHelpMessageForCommands(void)
{
    // "help seeAll"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " seeAll");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: seeAll", resp);

    // "help writeSD"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " writeSD");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: writeSD <text>", resp);

    // "help readDip"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " readDip");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: readDip", resp);

    // "help led"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " led");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: led <on/off>", resp);

    // "help cirbufWrite"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " cirbufWrite");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: cirbufWrite <bytes>", resp);

    // "help cirbufRead"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " cirbufRead");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: cirbufRead", resp);
    
    // "help getTime"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " getTime");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: getTime", resp);
}

void test_MyProcessor_help_ShowsItsOwnHelpWhenNoParams(void)
{
    // "help"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP);
    const char* response = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: help <command>", response);
}

void test_MyProcessor_help_ReturnsErrorMessageWhenCmdNotFound(void)
{
    MyProcessor_HandleCommandWithString(CMD_STR_HELP" badcommand");
    const char* response = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("No such command exists: badcommand", response);
}

// ******************** 'seeAll' command ********************

void test_MyProcessor_seeAll_ShowsAListOfAllCommands(void)
{
    MyProcessor_HandleCommandWithString("seeAll");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING(LIST_OF_CMDS, resp);
}

// ******************** 'writeSD' command ********************

void test_MyProcessor_writeSDCommandSuccess(void)
{
    MySD_Init_ExpectAndReturn("cli.txt", FR_OK);
    MySD_WriteString_ExpectAndReturn("hello there", FR_OK);

    MyProcessor_HandleCommandWithString("writeSD hello there");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("\"hello there\" has been written to SD card.", resp);
}

void test_MyProcessor_writeSDCommandFailInit(void)
{
    MySD_Init_ExpectAndReturn("cli.txt", FR_NOT_READY);

    MyProcessor_HandleCommandWithString("writeSD hello there");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Could not initialise SD card.", resp);
}

void test_MyProcessor_writeSDCommandFailWrite(void)
{
    MySD_Init_ExpectAndReturn("cli.txt", FR_OK);
    MySD_WriteString_ExpectAndReturn("hello there", FR_NOT_READY);

    MyProcessor_HandleCommandWithString("writeSD hello there");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Failed to write to SD card.", resp);
}


void test_MyProcessor_writeSDCommand0Params(void)
{
    MySD_Init_IgnoreAndReturn(FR_OK);
    MySD_WriteString_IgnoreAndReturn(FR_OK);

    MyProcessor_HandleCommandWithString("writeSD");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("\"\" has been written to SD card.", resp);
}

// ******************** 'readDip' command ********************

void test_MyProcessor_readDip(void)
{
    MyDIP_Init_Expect();
    MyDIP_Read_ExpectAndReturn(5);
    
    MyProcessor_HandleCommandWithString("readDip");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("DIP Switch: 5", resp);
}

// ******************** 'led' command ********************
void test_MyProcessor_led_on(void)
{
    led_init_Expect();
    led_on_Expect();

    MyProcessor_HandleCommandWithString("led on");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("LED is now ON.", resp);
}

void test_MyProcessor_led_off(void)
{
    led_init_Expect();
    led_off_Expect();

    MyProcessor_HandleCommandWithString("led off");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("LED is now OFF.", resp);
}

void test_MyProcessor_led_0_param(void)
{
    MyProcessor_HandleCommandWithString("led");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Missing parameter!", resp);
}

void test_MyProcessor_led_wrong_param(void)
{
    MyProcessor_HandleCommandWithString("led onn");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Invalid parameter: \"onn\".", resp);
}

// ******************** 'cirbuf' commands ********************

void test_MyProcessor_cirbufWrite(void)
{
    MyCircularBuffer_init_Expect();
    MyCircularBuffer_write_Expect('h');
    MyCircularBuffer_write_Expect('i');

    MyProcessor_HandleCommandWithString("cirbufWrite hi");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("\"hi\" written to circular buffer.", resp);
}

void test_MyProcessor_cirbufWriteNothing(void)
{
    MyCircularBuffer_init_Expect();

    MyProcessor_HandleCommandWithString("cirbufWrite");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("\"\" written to circular buffer.", resp);
}

void test_MyProcessor_cirbufRead(void)
{
    MyCircularBuffer_init_Expect();

    for (int i = 0; i < 5; i++)
    {
        MyCircularBuffer_isEmpty_ExpectAndReturn(0);
        MyCircularBuffer_read_ExpectAndReturn('a' + i);
    }
    MyCircularBuffer_isEmpty_ExpectAndReturn(1);

    MyProcessor_HandleCommandWithString("cirbufRead");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Buffer: abcde", resp);
}

void test_MyProcessor_cirbufReadMax20Bytes(void)
{
    MyCircularBuffer_init_Expect();

    for (int i = 0; i < 12; i++)
    {
        MyCircularBuffer_isEmpty_ExpectAndReturn(0);
        MyCircularBuffer_read_ExpectAndReturn('a' + i);
    }

    MyCircularBuffer_isEmpty_ExpectAndReturn(0);

    MyProcessor_HandleCommandWithString("cirbufRead");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Buffer: abcdefghijkl", resp);
}

// ******************** 'getTime' commands ********************

void test_MyProcessor_getTime(void)
{
    MyTimeString_Init_ExpectAndReturn(0);
    MyTimeString_GetTimeStamp_ExpectAndReturn("[the time]");
    MyProcessor_HandleCommandWithString("getTime");

    const char* resp = MyProcessor_GetResponseMessage();

    TEST_ASSERT_EQUAL_STRING("[the time]", resp);
}

// ******************** 'setTime' command ********************

#if 0   // Will add this later
void test_MyProcessor_setTime(void)
{
    MyTime expectedTime = 
    {
        .year = 22,
        .month = 11,
        .day = 10,
        .hour = 18,
        .minute = 56,
        .second = 26,
        .weekday = 4,
    };

    MyRTC_Init_Expect();
    MyRTC_WriteTime_ExpectAndReturn(&expectedTime, 0);

    MyProcessor_HandleCommandWithString("setTime 22 11 10 18 56 26 4");
    const char* resp = MyProcessor_GetResponseMessage();

    TEST_ASSERT_EQUAL_STRING("Time has been set.", resp);
}
#endif

// TODO, max cir buf read shall be in line with MAX_RESPONSE_LEN

// TODO, refactor!

// TODO: maybe make the \r a global? a string literal?
