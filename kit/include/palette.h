
#pragma once

#include "typedefs.h"

// Set Screen palette (256 colours)
void palette_setscreencolours(u8 StartIndex, u16 NumberOfColours, Colour* ColourArray);
// Rotate several colours in the screen palette. Use DirectionAndSpeed to change how quick and in which direction they rotate
void palette_rotatescreencolours(u8 StartIndex, u8 NumberOfColours, s8 DirectionAndSpeed);

// Set Screen palette (256 colours)
void palette_setspritecolours(u8 StartIndex, u16 NumberOfColours, Colour* ColourArray);
// Rotate several colours in the screen palette. Use DirectionAndSpeed to change how quick and in which direction they rotate
void palette_rotatespritecolours(u8 StartIndex, u8 NumberOfColours, s8 DirectionAndSpeed);

