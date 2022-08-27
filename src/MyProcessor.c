#include "MyProcessor.h"
#include <string.h>

typedef void(*CommandPtr)(void);
static char cmdResponse[20] = "say";

// Forward declarations
static void cmdHelp(void);
static void cmdSay(void);

typedef enum
{
    CMD_HELP,
    CMD_SAY,
    CMD_COUNT
} AllCommands_e;

// The command table
static struct
{
    char str[MAX_COMMAND_LEN];
    CommandPtr fn;
    char help[50];
} commands[CMD_COUNT] =
{
    [CMD_HELP] = {"help", cmdHelp, "Usage: help <command>"},
    [CMD_SAY] = {"say", cmdSay, "Usage: say <string>"}
};

//
void MyProcessor_HandleCommandWithString(char* str)
{
    size_t len = strcspn(str, " ");

    if (len != 0)   // we got some command...
    {
        for (unsigned int i = 0; i < CMD_COUNT; i++)
        {
            if (strncmp(commands[i].str, str, len) == 0)
            {
                commands[i].fn();
            }
        }
    }

}

char* MyProcessor_GetResponseMessage(void)
{
    return cmdResponse;
}



/************** The commands ***************/

static void cmdHelp(void)
{
    strcpy(cmdResponse, commands[CMD_SAY].help);
}

static void cmdSay(void)
{
    // strcpy(cmdResponse, "")
}
