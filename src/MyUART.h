#ifndef MY_UART_H_INCLUDED
#define MY_UART_H_INCLUDED

#include <stdint.h>

typedef struct MyUART_Config
{
    uint32_t baudrate;
} MyUART_Config;

void MyUART_init(MyUART_Config* config);

#endif
