#ifndef MY_PROCESSOR_H
#define MY_PROCESSOR_H

#define MAX_COMMAND_LEN 10
#define MAX_HELP_LEN 15
#define MAX_INPUT_LEN 21
#define CMD_STR_HELP "help"

void MyProcessor_HandleCommandWithString(char* str);
const char* MyProcessor_GetResponseMessage(void);

#endif
