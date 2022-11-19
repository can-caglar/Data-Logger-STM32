/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"
#include "MyRTC.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
    DWORD ret = 0;
    MyRTC_Init();
    MyTime time = MyRTC_ReadTime();
    ret =  (DWORD)((time.year + 2000) - 1980) << 25 |   // Year origin from the 1980 (0..127, e.g. 37 for 2017)
           (DWORD)(time.month) << 21 |      // Month (1..12)
           (DWORD)time.day     << 16 |      // Day of the month (1..31) 
           (DWORD)time.hour    << 11 |     // Hour (0..23)
           (DWORD)time.minute  << 5  |       // Minute (0..59)
           (DWORD)time.second  >> 1;        // Second / 2 (0..29, e.g. 25 for 50)
  return ret;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
