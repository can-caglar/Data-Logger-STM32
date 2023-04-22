#include "unity.h"

void setUp(void)
{

}

void test_auto_baud_rate_detection(void)
{
    TEST_IGNORE();

    // do all of the "wiring up" of the hardware
    // such that the timer interrupts are set up
    // to trigger during a LOW

    // therefore, this interrupt shall be called
    // at varying HAL_GetTick() intervals
    // at various amounts.

    // this interrupt shall update a global value
    // for the baud rate once it thinks it is ready
    
    // once ready, the interrupt shall be turned off.

    // can we fake all this?
}