#ifndef MY_OTHER_STM32F4XX_HAL_H
#define MY_OTHER_STM32F4XX_HAL_H

// Including this to make the code compile
// Compiler issues occur when
// trying to grab this from the stm32 toolchain
// due to the existence of other files.

#include "stm32f3xx_hal_conf.h"

typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;

/* Initialization and Configuration functions  ******************************/
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
void HAL_MspInit(void);
void HAL_MspDeInit(void);
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority);
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
uint32_t HAL_GetTickPrio(void);
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq);
HAL_TickFreqTypeDef HAL_GetTickFreq(void);
void HAL_Delay(uint32_t Delay);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);

uint32_t            HAL_GetHalVersion(void);
uint32_t            HAL_GetREVID(void);
uint32_t            HAL_GetDEVID(void);
uint32_t            HAL_GetUIDw0(void);
uint32_t            HAL_GetUIDw1(void);
uint32_t            HAL_GetUIDw2(void);
void                HAL_DBGMCU_EnableDBGStopMode(void);
void                HAL_DBGMCU_DisableDBGStopMode(void);
void                HAL_DBGMCU_EnableDBGStandbyMode(void);
void                HAL_DBGMCU_DisableDBGStandbyMode(void);

#endif
