#ifndef MY_COMMON_TYPES_H
#define MY_COMMON_TYPES_H

#ifdef TEST
#define STATIC
#define FOREVER 0
#else
#define STATIC static
#define FOREVER 1
#endif

#endif