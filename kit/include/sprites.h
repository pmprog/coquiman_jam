
#pragma once

#include "typedefs.h"

typedef enum SpriteSize
{
    SPRITE_SIZE_8x8          = 0x0000,
    SPRITE_SIZE_16x16        = 0x0001,
    SPRITE_SIZE_32x32        = 0x0002,
    SPRITE_SIZE_64x64        = 0x0003,
    SPRITE_SIZE_16x8         = 0x0004,
    SPRITE_SIZE_32x8         = 0x0005,
    SPRITE_SIZE_32x16        = 0x0006,
} SpriteSize;

typedef enum SpriteMirror
{
    SPRITE_MIRROR_NONE       = 0x0000,
    SPRITE_MIRROR_HORIZONTAL = 0x0001,
    SPRITE_MIRROR_VERTICAL   = 0x0002,
    SPRITE_MIRROR_BOTH       = 0x0003,
} SpriteMirror;

void sprites_clear();
void sprites_characterset_set(u8* CharacterSetSource, u16 FirstCharacter, u16 NumberOfCharacters, u8 BitsPerPixel);
void sprites_configure(u8 Index, u16 Tile, u8 BitsPerPixel, u8 PaletteIndex, SpriteSize Size);
void sprites_position(u8 Index, u8 Plane, u8 Visible, s16 X, s16 Y, SpriteMirror Flip);
void sprites_animate(u8 Index, u16 Tile, u8 PaletteIndex);
void sprites_position_get(u8 Index, u8* Plane, s16* X, s16* Y);
