#include "unity.h"
#include "MyRTC.h"
#include "mock_stm32f0xx_hal_i2c.h"
#include <stdio.h>

// Helpers
static void expectReadRegAndReturn(uint8_t* reg, uint8_t* returned);

#define TO_BCD(dec, units) (((dec) << 4) | (units));

I2C_HandleTypeDef hi2c1;
static uint8_t tempVal = 0;

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

// Registers for testing
uint8_t reg_seconds = PCF_SECONDS;
uint8_t reg_minutes = PCF_MINUTES;
uint8_t reg_hours = PCF_HOURS;
uint8_t reg_days = PCF_DAYS;
uint8_t reg_weekdays = PCF_WEEKDAYS;
uint8_t reg_months = PCF_MONTHS;
uint8_t reg_years = PCF_YEARS;

void test_init(void)
{
    // do nothing for now
    MyRTC_Init();
}

void test_readTime(void)
{
    uint8_t secs_43 =  TO_BCD(4,3); // 43
    uint8_t mins_54 =  TO_BCD(5,4); // 54
    uint8_t hours_17 = TO_BCD(1,7); // 17
    uint8_t days_9 = TO_BCD(0,9);   // 9
    uint8_t months_10 = TO_BCD(1,0);   // 10
    uint8_t years_22 = TO_BCD(2,2);   // 22
    uint8_t weekday_0 = TO_BCD(0,0);   // 0, Sunday
    
    expectReadRegAndReturn(&reg_seconds, &secs_43);
    expectReadRegAndReturn(&reg_minutes, &mins_54);
    expectReadRegAndReturn(&reg_hours, &hours_17);
    expectReadRegAndReturn(&reg_days, &days_9);
    expectReadRegAndReturn(&reg_months, &months_10);
    expectReadRegAndReturn(&reg_years, &years_22);
    expectReadRegAndReturn(&reg_weekdays, &weekday_0);

    MyTime time = MyRTC_ReadTime();

    TEST_ASSERT_EQUAL_INT(43, time.second);
    TEST_ASSERT_EQUAL_INT(54, time.minute);
    TEST_ASSERT_EQUAL_INT(17, time.hour);
    TEST_ASSERT_EQUAL_INT(9, time.day);
    TEST_ASSERT_EQUAL_INT(10, time.month);
    TEST_ASSERT_EQUAL_INT(22, time.year);
    TEST_ASSERT_EQUAL_INT(0, time.weekday);
}

void expectReadRegAndReturn(uint8_t* reg, uint8_t* returned)
{
    HAL_I2C_Master_Transmit_ExpectAndReturn(
        &hi2c1, WRITE_ADDR, reg, 1, 500, 0);

    tempVal = 0;    // expect to pass in buf with 0 to receive
    HAL_I2C_Master_Receive_ExpectAndReturn(
        &hi2c1, READ_ADDR, &tempVal, 1, 500, 0);

    HAL_I2C_Master_Receive_ReturnThruPtr_pData(returned);
}