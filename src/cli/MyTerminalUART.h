#ifndef MY_TERMINAL_UART_H_INCLUDED

void MyTerminalUART_Init(void);
void MyTerminalUART_Write(char val);
void MyTerminalUART_WriteString(const char* str);
char MyTerminalUART_Read(void);

#endif
