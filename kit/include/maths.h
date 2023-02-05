
#include "typedefs.h"

#ifndef range
#define range(a,b,c)    (a >= b ? (a <= c ? a : c) : b)
#endif

typedef struct randomconfig
{
    vu8 a;
    vu8 b;
    vu8 c;
    vu8 d;
} randomconfig;

// Random Number generation
void random_init( u8 seed );
u8 random_next();


// Fixed Point (8.8) maths in s16. 0 <= Degress < 360
s16 sini(s16 degrees);
s16 cosi(s16 degrees);
