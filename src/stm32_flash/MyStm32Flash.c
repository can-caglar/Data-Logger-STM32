#include "MyStm32Flash.h"
#include "stm32f3xx_hal_flash.h"
#include "stm32f3xx_hal_flash_ex.h"
#include <string.h>

HAL_StatusTypeDef write_flash_string(uint32_t address, const char *str)
{
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseInitStruct;
  uint32_t pageError = 0;

  // Calculate the number of half-words to write, including the null terminator
  uint32_t length = strlen(str) + 1;
  uint32_t half_word_length = (length + 1) / 2;

  // Unlock the Flash memory
  HAL_FLASH_Unlock();

  // Clear any existing flags
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

  // Define the erase configuration (erase one page that includes the desired address)
  eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseInitStruct.PageAddress = address;
  eraseInitStruct.NbPages = 1;

  // Erase the page
  status = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
  if (status != HAL_OK)
  {
    HAL_FLASH_Lock();
    return status;
  }

  // Write the string to the specified address, two bytes at a time
  for (uint32_t i = 0; i < half_word_length; i++) 
  {
    uint16_t half_word = str[i * 2] | (str[i * 2 + 1] << 8);
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + i * 2, half_word);
    if (status != HAL_OK)
    {
      HAL_FLASH_Lock();
      return status;
    }
  }

  // Lock the Flash memory again
  HAL_FLASH_Lock();

  return HAL_OK;
}

void read_flash_string(uint32_t address, char *buffer, uint32_t buffer_size) 
{
  uint32_t i = 0;
  char c;
  
  // Read the string from the specified address, byte by byte
  do {
    c = *(__IO char *)(address + i);
    buffer[i] = c;
    i++;
  } while (c != '\0' && i < buffer_size);

  // Ensure the buffer is null-terminated
  buffer[buffer_size - 1] = '\0';
}
