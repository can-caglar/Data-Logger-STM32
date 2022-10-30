#include "MyProcessor.h"
#include <string.h>
#include <stdio.h>

#include "MySD.h"
#include "MyDipSwitch.h"

// preprocessor
#define MAX_RESPONSE_LEN 50

// typedefs
typedef void(*CommandPtr)(void);

// Static globals
static char cmdResponse[MAX_RESPONSE_LEN];  // this is the response from the module
// Below variables for handling incoming string
// TODO This way of string handling is messy and needs to be refactored!
static char scratchpad[MAX_RESPONSE_LEN]; // Used for string manipulation
static char scratchpad2[MAX_RESPONSE_LEN];
static char* commandPhrase;

// Forward declarations

// Command handlers
static void cmdHelp(void);
static void cmdSay(void);
static void cmdSeeAll(void);
static void cmdWriteSD(void);
static void cmdReadDip(void);

// Helper functions
int findCommand(char* cmdStr);
void updateResponse(char* newResponse);

// Command count
typedef enum
{
    CMD_HELP,
    CMD_SAY,
    CMD_SEE_ALL,
    CMD_WRITESD,
    CMD_READDIP,
    CMD_COUNT   // must be last
} AllCommands_e;

//
// The command table
//
static struct
{
    char str[MAX_COMMAND_LEN];
    CommandPtr fn;
    char help[MAX_RESPONSE_LEN];
} commands[CMD_COUNT] =
{
    [CMD_HELP]    = {"help", cmdHelp, "Usage: help <command>"},
    [CMD_SAY]     = {"say", cmdSay, "Usage: say <string>"},
    [CMD_SEE_ALL] = {"seeAll", cmdSeeAll, "Usage: seeAll"},
    [CMD_READDIP] = {"readDip", cmdReadDip, "Usage: readDip"},
    [CMD_WRITESD] = {"writeSD", cmdWriteSD, "Usage: writeSD <text>"},
};

//
void MyProcessor_HandleCommandWithString(char* str)
{
    size_t inputStrLen = strlen(str);
    if (inputStrLen >= MAX_COMMAND_LEN)
    {
        updateResponse("Command too long!");
        return;
    }
    
    if (inputStrLen != 0)   
    {
        // we received a potential command...

        // break it down
        memset(scratchpad, 0, MAX_RESPONSE_LEN);
        memset(scratchpad2, 0, MAX_RESPONSE_LEN);
        strcpy(scratchpad, str);
        strcpy(scratchpad2, str);
        char* token = strtok(scratchpad, " ");
        commandPhrase = scratchpad2;
        commandPhrase += strlen(scratchpad) + 1;
        
        // look for it...
        int cmdIndex = findCommand(token);
        if (cmdIndex != -1)
        {
            // invoke it...
            commands[cmdIndex].fn();
        }
        else
        {
            sprintf(cmdResponse, "Command doesn't exist: %s", token);
        }
    }
    else
    {
        updateResponse("Received no commands.");
    }
}

const char* MyProcessor_GetResponseMessage(void)
{
    return cmdResponse;
}

/************** The commands ***************/

static void cmdHelp(void)
{
    char* token = strtok(NULL, " ");
    if (token != NULL)
    {
        // help for some command
        int cmdIndex = findCommand(token);
        if (cmdIndex != -1)
        {
            updateResponse(commands[cmdIndex].help);
        }
        else
        {
            // command not found
            sprintf(cmdResponse, "No such command exists: %s", token);
        }
    }
    else
    {
        // help for the "help" command
        updateResponse(commands[CMD_HELP].help);
    }
}

static void cmdSay(void)
{
    char* token = strtok(NULL, " ");
    if (token != NULL)
    {
        sprintf(cmdResponse, "STM32 would like to say %s", token);
    }
    else
    {
        updateResponse("Missing parameter!");
    }
}

static void cmdSeeAll(void)
{
    scratchpad[0] = 0;
    for (int i = 0; i < CMD_COUNT; i++)
    {
        strcat(scratchpad, commands[i].str);
        if (i < CMD_COUNT - 1)
        {
            strcat(scratchpad, " ");
        }
    }
    updateResponse(scratchpad);
}

static void cmdWriteSD(void)
{
    FRESULT err = MySD_Init("cli.txt");
    if (err == FR_OK)
    {
        err = MySD_Write(commandPhrase);
        if (err == FR_OK)
        {
            sprintf(cmdResponse, "\"%s\" has been written to SD card.", commandPhrase);
        }
        else
        {
            sprintf(cmdResponse, "Failed to write to SD card.");
        }
    }
    else
    {
        sprintf(cmdResponse, "Could not initialise SD card.");
    }
}

static void cmdReadDip(void)
{
    MyDIP_Init();
    uint8_t res = MyDIP_Read();
    sprintf(cmdResponse, "DIP Switch: %d", res);
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

void updateResponse(char* newResponse)
{
    // TODO, cut response if it's too long?
    strcpy(cmdResponse, newResponse);
}
