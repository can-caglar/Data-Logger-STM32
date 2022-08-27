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

void test_strchr(void)
{
    const char str[] = "help say";
    const char str2[] = "allCmds";
    const char str3[] = "";
    const char str4[] = "\n";

    int len = strcspn(str, " ");
    TEST_ASSERT_EQUAL_INT(4, len);
    TEST_ASSERT_EQUAL_STRING("help say", str);

    len = strcspn(str2, " ");
    TEST_ASSERT_EQUAL_INT(7, len);
    TEST_ASSERT_EQUAL_CHAR('\0', str2[len]);

    len = strcspn(str3, " ");
    TEST_ASSERT_EQUAL_INT(0, len);

    len = strcspn(str4, " ");
    TEST_ASSERT_EQUAL_INT(1, len);

    int compare = strncmp("hello one", "hello", 2);
    TEST_ASSERT_EQUAL_INT(0, compare);

    // strlen ignores null terminator
    size_t mylen = strlen("hello");
    TEST_ASSERT_EQUAL_INT(5, mylen);

    mylen = strlen("");
    TEST_ASSERT_EQUAL_INT(0, mylen);

    char myword[] = "hello mate";
    myword[2] = 0;
    strcat(myword, "??");
    TEST_ASSERT_EQUAL_STRING("he??", myword);


    // cspn could be used. where it returns is = 0.
    // if it returns 0, an empty string must have been passed in
    // if it's non 0, it will be the end of the first word.
    // 

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
    MyProcessor_HandleCommandWithString("badcmd");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Command doesn't exist: badcmd", resp);
}

void test_MyProcessor_sendingWeirdChars(void)
{
    MyProcessor_HandleCommandWithString("");
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Received no commands.", resp);
}

void test_MyProcessor_sendingVeryLongString(void)
{
    MyProcessor_HandleCommandWithString(
        "0123456789" "0123456789" "0123456789"
        "0123456789" "0123456789" "0123456789"
        "0123456789" "0123456789" "0123456789"
        "0123456789" "0123456789" "0123456789"
    );
    const char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Command too long!", resp);
}

// TODO, refactor!

#if 0
#endif

/*
[ ] The commands shall not be changed at runtime, instead, this module shall include all commands

[ ] I would like to be able to see all commands registered

[ ] I would like to see a "help" section for each command, i.e. 'help ledOn'

[?] I would like to know if the function and command string was able to be registered 
and should respond with fail or pass (use Error_Code_e)

[ ] I would like to be able to pass in a command string and this module shall
invoke the function, if it exists, along with arguments, e.g. "ledOn 1 2 3"

[ ] I would like to get a response from the command, depending on the command
and also depending on whether or not any failures happened.

(The function could be of type void(*)(char*)
The function can provide a reply in the char* argument)

This module can be asked to return the latest reply.

---

Implementation todos:
- [ ] TODO - Function for writing to output buffer. make sure it clips the output.

---

Commands:

0. Fail response is: "<command> failed. See "help <command>"

1. "say"
    help: "Usage: say <string>"
    response: "You asked me to say: <string>!""

*/