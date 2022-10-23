#include "unity.h"
#include "MyProcessor.h"
#include "mock_MySD.h"
#include <string.h>

#define LIST_OF_CMDS "help say seeAll writeSD"

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
    // "help say"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " say");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: say <string>", resp);

    // "help seeAll"
    MyProcessor_HandleCommandWithString(CMD_STR_HELP" "CMD_STR_SEE_ALL);
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: seeAll", resp);
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
    MyProcessor_HandleCommandWithString(CMD_STR_SEE_ALL);
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING(LIST_OF_CMDS, resp);
}

// ******************** 'say' command ********************

void test_MyProcessor_sayCommand(void)
{
    // 1 param
    MyProcessor_HandleCommandWithString("say hello");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("STM32 would like to say hello", resp);

    // 0 params
    MyProcessor_HandleCommandWithString("say");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Missing parameter!", resp);

    // Multiple params
    MyProcessor_HandleCommandWithString("say hi hi");
    resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("STM32 would like to say hi", resp);
}

// ******************** 'writeSD' command ********************

void test_MyProcessor_writeSDCommandSuccess(void)
{
    MySD_Init_ExpectAndReturn("cli.txt", FR_OK);
    MySD_Write_ExpectAndReturn("hello there", FR_OK);

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
    MySD_Write_ExpectAndReturn("hello there", FR_NOT_READY);

    MyProcessor_HandleCommandWithString("writeSD hello there");

    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Failed to write to SD card.", resp);
}

// TODO, refactor!

// TODO: maybe make the \r a global? a string literal?
