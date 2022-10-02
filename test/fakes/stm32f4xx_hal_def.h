#ifndef MY_FAKE_STM32F4xx_HAL_DEF
#define MY_FAKE_STM32F4xx_HAL_DEF

#include "stm32f4xx.h"
#include <stddef.h>

typedef enum 
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef enum 
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U  
} HAL_LockTypeDef;

#endif
