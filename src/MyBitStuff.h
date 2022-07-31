#ifndef MY_BIT_STUFF_H
#define MY_BIT_STUFF_H

#define SET_BIT(bit, reg)   ((reg) |= (1UL << (bit)))
#define CONF_BITS(mask, reg, new) ((reg) &= ~(mask)); ((reg) |= (new))
#define CLR_BIT(bit, reg)   ((reg) &= ~(1UL << (bit)))

#endif