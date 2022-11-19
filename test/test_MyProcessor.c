#include <string.h>
#include "unity.h"
#include "MyProcessor.h"
#include "mock_MySD.h"
#include "mock_MyDipSwitch.h"
#include "mock_LED.h"
#include "mock_MyCircularBuffer.h"
#include "mock_MyTimeString.h"
#include "mock_MyRTC.h"
#include "mock_BaudDecider.h"

#define LIST_OF_CMDS \
"help seeAll getTime setTime getBR"

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
    MyProcessor_HandleCommandWithString("012345678901234567890");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Input too long!", resp);
}

// ******************** 'help' command ********************

void test_MyProcessor_help_ShowsHelpMessageForCommands(void)
{
    // "help seeAll"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " seeAll");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: seeAll", resp);
    
    // "help getTime"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " getTime");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: getTime", resp);

    // "help setTime"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " setTime");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: setTime <yymmddhhmmss>", resp);
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
        .weekday = 0,
    };

    MyRTC_Init_Expect();
    MyRTC_WriteTime_ExpectAndReturn(&expectedTime, 0);

    // 2022-11-10 18:56:26
    MyProcessor_HandleCommandWithString("setTime 221110185626");
    const char* resp = MyProcessor_GetResponseMessage();

    TEST_ASSERT_EQUAL_STRING("Time has been set.", resp);
}

void test_MyProcessor_setTimeFailRTC(void)
{
    MyTime expectedTime = 
    {
        .year = 22,
        .month = 11,
        .day = 10,
        .hour = 18,
        .minute = 56,
        .second = 26,
        .weekday = 0,
    };

    MyRTC_Init_Expect();
    MyRTC_WriteTime_ExpectAndReturn(&expectedTime, 1);  // fails

    // 2022-11-10 18:56:26
    MyProcessor_HandleCommandWithString("setTime 221110185626");
    const char* resp = MyProcessor_GetResponseMessage();

    TEST_ASSERT_EQUAL_STRING("Time not set. RTC error.", resp);
}

void test_MyProcessor_setTimeBadInput(void)
{
    MyProcessor_HandleCommandWithString("setTime 12");
    const char* resp = MyProcessor_GetResponseMessage();

    TEST_ASSERT_EQUAL_STRING("Invalid parameter!", resp);
}

void test_MyProcessor_setTimeNoInput(void)
{
    MyProcessor_HandleCommandWithString("setTime");
    const char* resp = MyProcessor_GetResponseMessage();

    TEST_ASSERT_EQUAL_STRING("Missing parameter!", resp);
}

// ******************** 'getBR' command ********************

void test_MyProcessor_getBR(void)
{
    BaudDecider_Init_Expect();
    BaudDecider_GetBR_ExpectAndReturn(115200);

    MyProcessor_HandleCommandWithString("getBR");

    const char* resp = MyProcessor_GetResponseMessage();

    TEST_ASSERT_EQUAL_STRING("Baud Conf: 115200", resp);
}

// TODO, max cir buf read shall be in line with MAX_RESPONSE_LEN

// TODO, refactor!

// TODO: maybe make the \r a global? a string literal?
