#ifndef __STM32F0xx_HAL_DEF
#define __STM32F0xx_HAL_DEF

#include "stm32f031x6.h"
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
