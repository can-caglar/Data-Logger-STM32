#ifndef BAUD_DECIDER_H
#define BAUD_DECIDER_H

#include <stdint.h>

void BaudDecider_Init(void);
void BaudDecider_Close(void);
uint32_t BaudDecider_GetBR(void);

#endif
