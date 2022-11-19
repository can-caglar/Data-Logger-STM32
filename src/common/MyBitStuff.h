#ifndef MY_BIT_STUFF_H
#define MY_BIT_STUFF_H

#define SET_BIT(bit, reg)   ((reg) |= (1UL << (bit)))
#define CONF_BITS(mask, reg, newVal) ((reg) &= ~(mask)); ((reg) |= ((newVal) & (mask)))
#define CLR_BIT(bit, reg)   ((reg) &= ~(1UL << (bit)))

#endif
