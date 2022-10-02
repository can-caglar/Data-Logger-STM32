#include "unity.h"
#include "MyProcessor.h"
#include <string.h>

#define LIST_OF_CMDS "help say seeAll"

void setUp()
{

}

void tearDown()
{

}

void test_MyProcessor_Init(void)
{
}

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

void test_MyProcessor_seeAll_ShowsAListOfAllCommands(void)
{
    MyProcessor_HandleCommandWithString(CMD_STR_SEE_ALL);
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING(LIST_OF_CMDS, resp);
}

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


// TODO, refactor!

// TODO: maybe make the \r a global? a string literal?

/*
[x] The commands shall not be changed at runtime, instead, this module shall include all commands

[x] I would like to be able to see all commands registered

[x] I would like to see a "help" section for each command, i.e. 'help ledOn'

[x] I would like to be able to pass in a command string and this module shall
invoke the function, if it exists, along with arguments, e.g. "ledOn 1 2 3"

[ ] I would like to get a response from the command, depending on the command
and also depending on whether or not any failures happened.

(The function could be of type void(*)(char*)
The function can provide a reply in the char* argument)

This module can be asked to return the latest reply.

---

Implementation todos:
- [x] Function for writing to output buffer. make sure it clips the output.

---

*/
