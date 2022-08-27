#include "MyProcessor.h"
#include <string.h>

typedef void(*CommandPtr)(void);
static char cmdResponse[20] = "say";
static char* cmdParameters;

static char cmdScratchpad[20];

// Forward declarations
// Commands
static void cmdHelp(void);
static void cmdSay(void);

// Helper functions
int findCommand(char* cmdStr);

typedef enum
{
    CMD_HELP,
    CMD_SAY,
    CMD_SEE_ALL,
    CMD_COUNT   // must be last
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
    [CMD_SAY] = {"say", cmdSay, "Usage: say <string>"},
    [CMD_SEE_ALL] = {"seeAll", cmdSay, "Usage: seeAll"},
};

//
void MyProcessor_HandleCommandWithString(char* str)
{
    size_t inputStrLen = strlen(str);
    strcpy(cmdScratchpad, str);
    
    char* token = strtok(cmdScratchpad, " ");

    if (inputStrLen != 0)   // we got some command...
    {
        size_t lenCmd = strcspn(str, " ");
        cmdParameters = strchr(str, ' ');
        
        // invoke it...
        int cmdIndex = findCommand(token);
        if (cmdIndex != -1)
        {
            commands[cmdIndex].fn();
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
    char* token = strtok(cmdParameters, " ");
    if (token != NULL)
    {
        // help for some command
        int cmdIndex = findCommand(token);
        if (cmdIndex != -1)
        {
            strcpy(cmdResponse, commands[cmdIndex].help);
        }
        else
        {
            // command not found
            char errMessage[40] = "No such command exists: ";
            strcat(errMessage, token);
            strcpy(cmdResponse, errMessage);
        }
    }
    else
    {
        // help for the "help" command
        strcpy(cmdResponse, commands[CMD_HELP].help);
    }
}

static void cmdSay(void)
{
    // strcpy(cmdResponse, "")
}

/********************* helper functions **************/

int findCommand(char* cmdStr)
{
    for (unsigned int i = 0; i < CMD_COUNT; i++)
    {
        if (strcmp(commands[i].str, cmdStr) == 0)
        {
            return i;
        }
    }
    return -1;
}