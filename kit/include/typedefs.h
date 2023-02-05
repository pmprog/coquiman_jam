
#pragma once

#ifndef size_t
#include <stddef.h>
#endif

// Make sure we have nulls defined in a variety of manners. They are all technically the same though
#ifndef NULL
#ifdef null
#define NULL        null
#else
#define NULL        0
#endif
#endif
#ifndef null
#define null        NULL
#endif
#ifndef nullptr
#define nullptr     NULL
#endif

typedef unsigned char                   u8;
typedef volatile unsigned char          vu8;
typedef unsigned short                  u16;
typedef volatile unsigned short         vu16;
typedef unsigned int                    u32;
typedef volatile unsigned int           vu32;
typedef signed char                     s8;
typedef volatile signed char            vs8;
typedef signed short                    s16;
typedef volatile signed short           vs16;
typedef signed int                      s32;
typedef volatile signed int             vs32;

typedef struct Colour
{
    u8 R;
    u8 G;
    u8 B;
} Colour;
