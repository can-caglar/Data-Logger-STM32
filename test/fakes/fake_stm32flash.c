#include "fake_stm32flash.h"
#include <string.h>
#include <stdlib.h>

#define FAKE_MAX_ADDRESSES 5000

static struct
{
    char* addresses[FLASH_DATA_END];
} fakeFlash;

void fake_stm32flash_reset(void)
{
    for (int i = 0; i < FLASH_DATA_END; i++)
    {
        if (fakeFlash.addresses[i] != 0)
        {
            free(fakeFlash.addresses[i]);
        }
    }
    memset(&fakeFlash, 0, sizeof(fakeFlash));
}

HAL_StatusTypeDef write_flash_string(uint32_t address, const char *str)
{
    uint32_t newAddress = address % FAKE_MAX_ADDRESSES;
    int len = strlen(str) + 1;
    fakeFlash.addresses[newAddress] = (char*)calloc(len, 1);
    if (fakeFlash.addresses[newAddress])
    {
        strcpy(fakeFlash.addresses[newAddress], str);
    }
}

void read_flash_string(uint32_t address, 
    char *buffer, uint32_t buffer_size)
{
    uint32_t newAddress = address % FAKE_MAX_ADDRESSES;
    if (fakeFlash.addresses[newAddress])
    {
        strncpy(buffer, 
            fakeFlash.addresses[newAddress], buffer_size);
    }
}
