
#include "../include/palette.h"
#include "../include/kit.h"
#include "gba.h"

// Set Screen palette (256 colours)
void palette_setscreencolours(u8 StartIndex, u16 NumberOfColours, Colour* ColourArray)
{
    u16* bptr = pal_bg_mem;

    for(u16 i = 0; i < NumberOfColours; i++)
    {
        bptr[StartIndex + i] = RGB8macro(ColourArray[i].R, ColourArray[i].G, ColourArray[i].B);
    }
}

// Rotate several colours in the screen palette. Use DirectionAndSpeed to change how quick and in which direction they rotate
void palette_rotatescreencolours(u8 StartIndex, u8 NumberOfColours, s8 DirectionAndSpeed)
{
    u16* bptr = pal_bg_mem;
    u8 endindex = StartIndex + NumberOfColours - 1;

    if( kit_stage_time % abs(DirectionAndSpeed) == 0 )
    {
        if(DirectionAndSpeed >= 0)
        {
            u16 colour = bptr[endindex];
            for(u16 i = endindex - 1; i >= StartIndex; i--)
            {
                bptr[i + 1] = bptr[i];
            }
            bptr[StartIndex] = colour;
        }
        else
        {
            u16 colour = bptr[StartIndex];
            for(u16 i = StartIndex + 1; i <= endindex; i++)
            {
                bptr[i - 1] = bptr[i];
            }
            bptr[endindex] = colour;
        }
    }
}

// Set Screen palette (256 colours)
void palette_setspritecolours(u8 StartIndex, u16 NumberOfColours, Colour* ColourArray)
{
    u16* cptr = pal_obj_mem;

    for(u16 i = 0; i < NumberOfColours; i++)
    {
        cptr[StartIndex + i] = RGB8macro(ColourArray[i].R, ColourArray[i].G, ColourArray[i].B);
    }
}

// Rotate several colours in the screen palette. Use DirectionAndSpeed to change how quick and in which direction they rotate
void palette_rotatespritecolours(u8 StartIndex, u8 NumberOfColours, s8 DirectionAndSpeed)
{
    u16* bptr = pal_obj_mem;
    u8 endindex = StartIndex + NumberOfColours - 1;

    if( kit_stage_time % abs(DirectionAndSpeed) == 0 )
    {
        if(DirectionAndSpeed >= 0)
        {
            u16 colour = bptr[endindex];
            for(u16 i = endindex - 1; i >= StartIndex; i--)
            {
                bptr[i + 1] = bptr[i];
            }
            bptr[StartIndex] = colour;
        }
        else
        {
            u16 colour = bptr[StartIndex];
            for(u16 i = StartIndex + 1; i <= endindex; i++)
            {
                bptr[i - 1] = bptr[i];
            }
            bptr[endindex] = colour;
        }
    }
}

