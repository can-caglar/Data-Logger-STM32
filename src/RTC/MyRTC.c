#include "MyRTC.h"
#include "stm32f3xx_hal.h"
#include "main.h"
#include <string.h>

// Datasheet:
// https://www.nxp.com/docs/en/data-sheet/PCF8523.pdf

// I2C addr
#define READ_ADDR  0xD1
#define WRITE_ADDR 0xD0

// Bitmasks for date time registers
#define BM_SECONDS  0x7F    // bits 0-6
#define BM_MINUTES  0x7F    // bits 0-6
#define BM_HOURS    0x3F    // bits 0-5
#define BM_DAYS     0x3F    // bits 0-5
#define BM_MONTHS   0x1F    // bits 0-4
#define BM_WEEKDAYS 0x07    // bits 0-2
#define BM_YEARS    0xFF    // bits 0-7

// Bits
#define CTRL1_BM_STOP (1 << 5)

// Static variables
static uint32_t timeLastCalled = 0;
static MyTime time = { 0 };

// Consts
#define ONE_SECOND  1000

// Registers
enum
{
    PCF_CTRL_1,  
    PCF_CTRL_2,  
    PCF_CTRL_3,  
    PCF_SECONDS, 
    PCF_MINUTES, 
    PCF_HOURS, 
    PCF_DAYS, 
    PCF_WEEKDAYS, 
    PCF_MONTHS, 
    PCF_YEARS,
};

extern I2C_HandleTypeDef hi2c1;

// Helpers
static HAL_StatusTypeDef readReg(uint8_t reg, uint8_t* ret);
static HAL_StatusTypeDef writeReg(uint8_t reg, uint8_t val);
static uint8_t bcdToInt(uint8_t bcd);
uint8_t intToBCD(uint8_t num);

void MyRTC_Init(void)
{
    CubeMX_SystemInit(CMX_I2C);
    memset(&time, 0, sizeof(time));
    timeLastCalled = 0;
    
    // as per documentation:
    // battery switch-over function is enabled in direct switching mode;
    // battery low detection function is disabled
    // this helps with RTC data getting corrupted
    // when power is disconnected
    writeReg(PCF_CTRL_3, (0b101 << 5));
}

MyTime MyRTC_ReadTime(void)
{
    uint32_t timeNow = HAL_GetTick();

    if ((timeNow >= (timeLastCalled + ONE_SECOND)) || timeLastCalled == 0)
    {
        timeLastCalled = timeNow;

        readReg(PCF_SECONDS, &time.second);
        readReg(PCF_MINUTES, &time.minute);
        readReg(PCF_HOURS, &time.hour);
        readReg(PCF_DAYS, &time.day);
        readReg(PCF_MONTHS, &time.month);
        readReg(PCF_YEARS, &time.year);
        readReg(PCF_WEEKDAYS, &time.weekday);

        time.second = bcdToInt(time.second & BM_SECONDS) ;
        time.minute = bcdToInt(time.minute & BM_MINUTES);
        time.hour = bcdToInt(time.hour & BM_HOURS);
        time.day = bcdToInt(time.day & BM_DAYS);
        time.month = bcdToInt(time.month & BM_MONTHS);
        time.year = timeNow & 0x3F; // pseudo-random year //bcdToInt(time.year & BM_YEARS);
        time.weekday = bcdToInt(time.weekday & BM_WEEKDAYS);
    }
    return time;
}

int MyRTC_WriteTime(const MyTime* newTime)
{
    HAL_StatusTypeDef err = HAL_OK;
    uint8_t send[8];
    uint8_t stopOsc[2] = {PCF_CTRL_1};

    send[0] = PCF_SECONDS;
    send[1] = intToBCD(newTime->second);
    send[2] = intToBCD(newTime->minute);
    send[3] = intToBCD(newTime->hour);
    send[4] = intToBCD(newTime->day);
    send[5] = intToBCD(newTime->weekday);
    send[6] = intToBCD(newTime->month);
    send[7] = intToBCD(newTime->year);

    err = readReg(stopOsc[0], &(stopOsc[1]));
    stopOsc[1] |= CTRL1_BM_STOP;

    if (err == HAL_OK)
    {
        err = writeReg(PCF_CTRL_1, stopOsc[1]);
        err = HAL_I2C_Master_Transmit(&hi2c1, WRITE_ADDR, send, 8, 500);
        stopOsc[1] &= ~CTRL1_BM_STOP;
        writeReg(PCF_CTRL_1, stopOsc[1]);
    }
    if (err != HAL_OK)
    {
        return -1;
    }
    return 0;
}

// Helper functions

static HAL_StatusTypeDef readReg(uint8_t reg, uint8_t* ret)
{
    HAL_StatusTypeDef err = HAL_I2C_Master_Transmit(&hi2c1, WRITE_ADDR, &reg, 1, 500);
    if (err == HAL_OK)
    {
        err = HAL_I2C_Master_Receive(&hi2c1, READ_ADDR, ret, 1, 500);
    }
    return err;
}

static HAL_StatusTypeDef writeReg(uint8_t reg, uint8_t val)
{
    uint8_t data[2] = {reg, val};
    HAL_StatusTypeDef err = HAL_I2C_Master_Transmit(&hi2c1, WRITE_ADDR, data, 2, 500);
    return err;
}

static uint8_t bcdToInt(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0xF);
}

uint8_t intToBCD(uint8_t num)
{
    // For 2 digit numbers only
    uint8_t units = num % 10;
    uint8_t tens = num / 10;

    return (tens << 4) | (units);
}
