#include "unity.h"
#include "MyRTC.h"
#include "mock_stm32f0xx_hal_i2c.h"
#include "mock_main.h"
#include "mock_stm32f0xx_hal.h"
#include <stdio.h>

// Helpers
static void expectReadRegAndReturn(uint8_t* reg, uint8_t* returned);
static void ignoreReadRegAndReturn(void);

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
uint8_t reg3 = PCF_CTRL_3;
uint8_t reg_seconds = PCF_SECONDS;
uint8_t reg_minutes = PCF_MINUTES;
uint8_t reg_hours = PCF_HOURS;
uint8_t reg_days = PCF_DAYS;
uint8_t reg_weekdays = PCF_WEEKDAYS;
uint8_t reg_months = PCF_MONTHS;
uint8_t reg_years = PCF_YEARS;

void setUp(void)
{
    CubeMX_SystemInit_Ignore();
    HAL_I2C_Master_Transmit_IgnoreAndReturn(0);

    MyRTC_Init();

    HAL_I2C_Master_Transmit_StopIgnore();
    CubeMX_SystemInit_StopIgnore();
}

void test_init(void)
{
    CubeMX_SystemInit_Expect(CMX_I2C);

    // Read CTRL reg 3, shall return 0x3
    uint8_t reg = reg3;
    uint8_t regbuf[2] = {reg3, (5 << 5)};

    // We shall write to CTRL reg 3 to stop the RTC
    HAL_I2C_Master_Transmit_ExpectAndReturn(
        &hi2c1, WRITE_ADDR, &regbuf, 2, 500, 0);

    MyRTC_Init();
}

void test_readTime(void)
{
    uint8_t secs_43 =  TO_BCD(4,3);    // 43
    uint8_t mins_54 =  TO_BCD(5,4);    // 54
    uint8_t hours_17 = TO_BCD(1,7);    // 17
    uint8_t days_9 = TO_BCD(0,9);      // 9
    uint8_t months_10 = TO_BCD(1,0);   // 10
    uint8_t years_99 = TO_BCD(9,9);    // 22
    uint8_t weekday_0 = TO_BCD(0,0);   // 0, Sunday

    // messing up the bits that shouldn't be used in the calculation
    secs_43 |= 0x80; // bit 7
    mins_54 |= 0x80; // bit 7
    hours_17 |= 0xC0; // bits 6-7
    days_9 |= 0xC0; // bits 6-7
    months_10 |= 0xE0; // bits 5-7
    years_99 |= 0;  // all bits to be used in calculation
    weekday_0 |= 0xF8; // bit 3-7

    HAL_GetTick_ExpectAndReturn(999); // first time

    expectReadRegAndReturn(&reg_seconds, &secs_43);
    expectReadRegAndReturn(&reg_minutes, &mins_54);
    expectReadRegAndReturn(&reg_hours, &hours_17);
    expectReadRegAndReturn(&reg_days, &days_9);
    expectReadRegAndReturn(&reg_months, &months_10);
    expectReadRegAndReturn(&reg_years, &years_99);
    expectReadRegAndReturn(&reg_weekdays, &weekday_0);

    MyTime time = MyRTC_ReadTime();

    TEST_ASSERT_EQUAL_INT(43, time.second);
    TEST_ASSERT_EQUAL_INT(54, time.minute);
    TEST_ASSERT_EQUAL_INT(17, time.hour);
    TEST_ASSERT_EQUAL_INT(9, time.day);
    TEST_ASSERT_EQUAL_INT(10, time.month);
    TEST_ASSERT_EQUAL_INT(99, time.year);
    TEST_ASSERT_EQUAL_INT(0, time.weekday);

    // shortly after should not do anything
    HAL_GetTick_ExpectAndReturn(1500); // too early
    time = MyRTC_ReadTime();
}

void test_readTimeWithTimeouts(void)
{
    uint8_t secs_43 =  0;
    uint8_t mins_54 =  0;
    uint8_t hours_17 = 0;
    uint8_t days_9 = 0;
    uint8_t months_10 = 0;
    uint8_t years_99 = 0;
    uint8_t weekday_0 = 0;

    HAL_GetTick_ExpectAndReturn(999); // first time

    // shall expect read reg
    expectReadRegAndReturn(&reg_seconds, &secs_43);
    expectReadRegAndReturn(&reg_minutes, &mins_54);
    expectReadRegAndReturn(&reg_hours, &hours_17);
    expectReadRegAndReturn(&reg_days, &days_9);
    expectReadRegAndReturn(&reg_months, &months_10);
    expectReadRegAndReturn(&reg_years, &years_99);
    expectReadRegAndReturn(&reg_weekdays, &weekday_0);

    MyRTC_ReadTime();

    // shortly after should not do anything
    HAL_GetTick_ExpectAndReturn(1500); // too early
    MyRTC_ReadTime();

    // 1 second later shall
    HAL_GetTick_ExpectAndReturn(1999); // just right

    expectReadRegAndReturn(&reg_seconds, &secs_43);
    expectReadRegAndReturn(&reg_minutes, &mins_54);
    expectReadRegAndReturn(&reg_hours, &hours_17);
    expectReadRegAndReturn(&reg_days, &days_9);
    expectReadRegAndReturn(&reg_months, &months_10);
    expectReadRegAndReturn(&reg_years, &years_99);
    expectReadRegAndReturn(&reg_weekdays, &weekday_0);

    MyRTC_ReadTime();
}

void test_writeTime(void)
{
    MyTime time =
    {
        .year = 22,
        .month = 11,
        .day = 10,
        .hour = 18,
        .minute = 56,
        .second = 26,
        .weekday = 4,
    };

    uint8_t rbuf[8];

    rbuf[0] = PCF_SECONDS;
    rbuf[1] = TO_BCD(2,6)
    rbuf[2] = TO_BCD(5,6)
    rbuf[3] = TO_BCD(1,8)
    rbuf[4] = TO_BCD(1,0)
    rbuf[5] = TO_BCD(0,4)
    rbuf[6] = TO_BCD(1,1)
    rbuf[7] = TO_BCD(2,2)

    uint8_t stopbuf[2];
    uint8_t returned = 0x3; // pretend that ctrl reg is like so
    stopbuf[0] = PCF_CTRL_1;
    stopbuf[1] = (1 << 5) | returned;

    // Read CTRL reg 1, shall return 0x3
    expectReadRegAndReturn(&(stopbuf[0]), &returned);

    // We shall write to CTRL reg 1 to stop the RTC
    HAL_I2C_Master_Transmit_ExpectAndReturn(
        &hi2c1, WRITE_ADDR, stopbuf, 2, 500, 0);

    // Write new values
    HAL_I2C_Master_Transmit_ExpectAndReturn(
        &hi2c1, WRITE_ADDR, rbuf, 8, 500, 0);

    // Start RTC again
    stopbuf[2] &= ~(1 << 5);
    HAL_I2C_Master_Transmit_ExpectAndReturn(
        &hi2c1, WRITE_ADDR, stopbuf, 2, 500, 0);

    TEST_ASSERT_EQUAL(0, MyRTC_WriteTime(&time));
}

void test_write_time_fail_1(void)
{
    MyTime time = {0};
    uint8_t r1 = PCF_SECONDS;

    HAL_I2C_Master_Receive_IgnoreAndReturn(1);
    HAL_I2C_Master_Transmit_IgnoreAndReturn(0);

    int err = MyRTC_WriteTime(&time);

    TEST_ASSERT_EQUAL_INT(-1, err);

    HAL_I2C_Master_Receive_IgnoreAndReturn(0);
    HAL_I2C_Master_Transmit_IgnoreAndReturn(1);

    err = MyRTC_WriteTime(&time);

    TEST_ASSERT_EQUAL_INT(-1, err);
}


// Helpers
void expectReadRegAndReturn(uint8_t* reg, uint8_t* returned)
{
    HAL_I2C_Master_Transmit_ExpectAndReturn(
        &hi2c1, WRITE_ADDR, reg, 1, 500, 0);

    tempVal = 0;    // expect to pass in buf with 0 to receive
    HAL_I2C_Master_Receive_ExpectAndReturn(
        &hi2c1, READ_ADDR, &tempVal, 1, 500, 0);

    HAL_I2C_Master_Receive_ReturnThruPtr_pData(returned);
}

void ignoreReadRegAndReturn(void)
{
    HAL_I2C_Master_Transmit_IgnoreAndReturn(0);

    HAL_I2C_Master_Receive_IgnoreAndReturn(0);
}
