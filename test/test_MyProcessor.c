#include "unity.h"
#include "MyProcessor.h"
#include <string.h>

#define LIST_OF_CMDS "say"

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

    // cspn could be used. where it returns is = 0.
    // if it returns 0, an empty string must have been passed in
    // if it's non 0, it will be the end of the first word.
    // 

}

void test_MyProcessor_SeeAllCommands(void)
{
    MyProcessor_HandleCommandWithString(CMD_STR_SEE_ALL);
    char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING(LIST_OF_CMDS, resp);
}

void test_MyProcessor_SeeHelp(void)
{
    MyProcessor_HandleCommandWithString(CMD_STR_HELP " say");
    char* resp = MyProcessor_GetResponseMessage();
    TEST_ASSERT_EQUAL_STRING("Usage: say <string>", resp);
}
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

Commands:

0. Fail response is: "<command> failed. See "help <command>"

1. "say"
    help: "Usage: say <string>"
    response: "You asked me to say: <string>!""

*/