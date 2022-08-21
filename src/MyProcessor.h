#ifndef MY_PROCESSOR_H
#define MY_PROCESSOR_H

// typedef void(*CmdHandler)(void);

void MyProcessor_HandleCommandWithString(char* str);
char* MyProcessor_GetResponseMessage(void);

#endif