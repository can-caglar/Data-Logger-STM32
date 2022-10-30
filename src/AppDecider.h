#ifndef APP_DECIDER_H_INCLUDED
#define APP_DECIDER_H_INCLUDED

typedef enum Applications
{
    APP_CLI,
    APP_SNOOPING
} Applications;

void AppDecider_Init();
Applications AppDecider_Decide();

#endif