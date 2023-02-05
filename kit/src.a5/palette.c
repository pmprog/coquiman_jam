
#include "../include/display.h"
#include "../include/kit.h"
#include "allegro.h"

Colour palette_screen[256];
Colour palette_sprites[256];
ALLEGRO_COLOR palette_screen_a5[256];
ALLEGRO_COLOR palette_sprites_a5[256];

void palette_setscreencolours(u8 StartIndex, u16 NumberOfColours, Colour* ColourArray)
{
    for(u16 i = 0; i < NumberOfColours; i++ )
    {
        u16 c = (u16)StartIndex + (u16)i;
        if( c < 256 )
        {
            palette_screen[c] = ColourArray[i];
            palette_screen_a5[c] = al_map_rgb( ColourArray[i].R, ColourArray[i].G, ColourArray[i].B );
        }
    }
}

// Rotate several colours in the screen palette. Use DirectionAndSpeed to change how quick and in which direction they rotate
void palette_rotatescreencolours(u8 StartIndex, u8 NumberOfColours, s8 DirectionAndSpeed)
{
    u8 endindex = StartIndex + NumberOfColours - 1;

    if( kit_stage_time % abs(DirectionAndSpeed) == 0 )
    {
        if(DirectionAndSpeed >= 0)
        {
            Colour colour = palette_screen[endindex];
            ALLEGRO_COLOR ca5 = palette_screen_a5[endindex];
            for(u16 i = endindex - 1; i >= StartIndex; i--)
            {
                palette_screen[i + 1] = palette_screen[i];
                palette_screen_a5[i + 1] = palette_screen_a5[i];
            }
            palette_screen[StartIndex] = colour;
            palette_screen_a5[StartIndex] = ca5;
        }
        else
        {
            Colour colour = palette_screen[StartIndex];
            ALLEGRO_COLOR ca5 = palette_screen_a5[StartIndex];
            for(u16 i = StartIndex + 1; i <= endindex; i++)
            {
                palette_screen[i - 1] = palette_screen[i];
                palette_screen_a5[i - 1] = palette_screen_a5[i];
            }
            palette_screen[endindex] = colour;
            palette_screen_a5[endindex] = ca5;
        }
    }
}

void palette_setspritecolours(u8 StartIndex, u16 NumberOfColours, Colour* ColourArray)
{
    for(u16 i = 0; i < NumberOfColours; i++ )
    {
        u16 c = (u16)StartIndex + (u16)i;
        if( c < 256 )
        {
            palette_sprites[c] = ColourArray[i];
            palette_sprites_a5[c] = al_map_rgb( ColourArray[i].R, ColourArray[i].G, ColourArray[i].B );
        }
    }
    sprites_refreshcache();
}

// Rotate several colours in the screen palette. Use DirectionAndSpeed to change how quick and in which direction they rotate
void palette_rotatespritecolours(u8 StartIndex, u8 NumberOfColours, s8 DirectionAndSpeed)
{
    u8 endindex = StartIndex + NumberOfColours - 1;

    if( kit_stage_time % abs(DirectionAndSpeed) == 0 )
    {
        if(DirectionAndSpeed >= 0)
        {
            Colour colour = palette_sprites[endindex];
            ALLEGRO_COLOR ca5 = palette_sprites_a5[endindex];
            for(u16 i = endindex - 1; i >= StartIndex; i--)
            {
                palette_sprites[i + 1] = palette_sprites[i];
                palette_sprites_a5[i + 1] = palette_sprites_a5[i];
            }
            palette_sprites[StartIndex] = colour;
            palette_sprites_a5[StartIndex] = ca5;
        }
        else
        {
            Colour colour = palette_sprites[StartIndex];
            ALLEGRO_COLOR ca5 = palette_sprites_a5[StartIndex];
            for(u16 i = StartIndex + 1; i <= endindex; i++)
            {
                palette_sprites[i - 1] = palette_sprites[i];
                palette_sprites_a5[i - 1] = palette_sprites_a5[i];
            }
            palette_sprites[endindex] = colour;
            palette_sprites_a5[endindex] = ca5;
        }
    }
}

