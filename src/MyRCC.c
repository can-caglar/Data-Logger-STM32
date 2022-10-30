#include "MyRCC.h"

/*
* @param reg - The register responsible for the gpio clock enable
* @param gpio_port - The bit position to enable clock for
*/
Error_Code_e MyRCC_ClockEnable(rcc_register* reg, unsigned int position)
{
    *reg |= (1 << position);
    return ECODE_OK;
}

