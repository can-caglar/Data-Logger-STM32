#include "MyRCC.h"

/*
* @param reg - The register responsible for the gpio clock enable
* @param gpio_port - The bit position to enable clock for
*/
Error_Code_e MyRCC_GPIOClockEnable(rcc_register* reg, int gpio_port)
{
    *reg |= (1 << gpio_port);
    return ECODE_OK;
}
