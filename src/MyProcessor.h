#ifndef MY_PROCESSOR_H
#define MY_PROCESSOR_H

#define MAX_COMMAND_LEN 20
#define CMD_STR_SEE_ALL  "seeAll"
#define CMD_STR_HELP "help"
#define CMD_STR_WRITESD "writeSD"

void MyProcessor_HandleCommandWithString(char* str);
const char* MyProcessor_GetResponseMessage(void);

#endif
