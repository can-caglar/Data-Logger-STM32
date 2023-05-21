#ifndef FAKE_MY_TIMER_H
#define FAKE_MY_TIMER_H

#include "MyTimer.h"

void fake_mytimer_reset(void);
void fake_mytimer_pretendToReceiveString(const char* data, uint32_t baudRate);

#endif
