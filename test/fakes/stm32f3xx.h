#ifndef MY_FAKE_STM32F4XX_H
#define MY_FAKE_STM32F4XX_H

#include "stm32f303x8.h"

typedef enum 
{
  RESET = 0U, 
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum 
{
  DISABLE = 0U, 
  ENABLE = !DISABLE
} FunctionalState;

typedef enum
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;

#endif