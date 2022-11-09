#include "MyRTC.h"
#include "stm32f0xx_hal.h"

// I2C addr
#define READ_ADDR  0xD1
#define WRITE_ADDR 0xD0

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

static void readReg(uint8_t reg, uint8_t* ret);
static uint8_t bcdToInt(uint8_t bcd);

int MyRTC_Init(void)
{
}

MyTime MyRTC_ReadTime(void)
{
    MyTime time = { 0 };

    HAL_StatusTypeDef err = 0;

    readReg(PCF_SECONDS, &time.second);
    readReg(PCF_MINUTES, &time.minute);
    readReg(PCF_HOURS, &time.hour);
    readReg(PCF_DAYS, &time.day);
    readReg(PCF_MONTHS, &time.month);
    readReg(PCF_YEARS, &time.year);
    readReg(PCF_WEEKDAYS, &time.weekday);

    time.second = bcdToInt(time.second);
    time.minute = bcdToInt(time.minute);
    time.hour = bcdToInt(time.hour);
    time.day = bcdToInt(time.day);
    time.month = bcdToInt(time.month);
    time.year = bcdToInt(time.year);
    time.weekday = bcdToInt(time.weekday);

    return time;
}

// Helper functions

static void readReg(uint8_t reg, uint8_t* ret)
{
    HAL_StatusTypeDef err = 0;
    err = HAL_I2C_Master_Transmit(&hi2c1, WRITE_ADDR, &reg, 1, 500);
    err = HAL_I2C_Master_Receive(&hi2c1, READ_ADDR, ret, 1, 500);
}

static uint8_t bcdToInt(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0xF);
}
