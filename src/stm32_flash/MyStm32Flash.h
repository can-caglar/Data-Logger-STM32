#ifndef MY_STM32_FLASH_H
#define MY_STM32_FLASH_H

#include "stm32f3xx_hal.h"

#define FLASH_DATA_END 0x08010000
#define PAGE_SIZE      2048

#define FLASH_DATA_PAGE_0 (FLASH_DATA_END - PAGE_SIZE)

HAL_StatusTypeDef write_flash_string(uint32_t address, const char *str);
void read_flash_string(uint32_t address, char *buffer, uint32_t buffer_size);

#endif
