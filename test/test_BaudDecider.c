#include "unity.h"
#include "mock_MyDipSwitch.h"
#include "BaudDecider.h"

#define MAX_BAUD_SETTINGS 16

int values[MAX_BAUD_SETTINGS] = 
{
    [0] = 460800,
    [1] = 230400,
    [2] = 115200,
    [3] = 76800,
    [4] = 57600,
    [5] = 38400,
    [6] = 19200,
    [7] = 14400,
    [8] = 9600,
    [9] = 4800,
    [10] = 2400,
    [11] = 1200,
    [12] = 600,
    [13] = 300,
    [14] = 110,
    [15] = 19200, // CLI
};

void test_getBR(void)
{
    MyDIP_Init_Expect();

    for (int i = 0; i < MAX_BAUD_SETTINGS; i++)
    {
        MyDIP_Read_ExpectAndReturn(i);
        TEST_ASSERT_EQUAL_INT(values[i], BaudDecider_GetBR());
    }
}
