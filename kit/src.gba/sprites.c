
#include "../include/sprites.h"
#include "../include/kit.h"
#include "gba.h"

void sprites_clear()
{
    for(u8 i = 0; i < 128; i++)
    {
        OAM[i].attr0 |= ATTR0_HIDE;
    }
}

void sprites_characterset_set(u8* CharacterSetSource, u16 FirstCharacter, u16 NumberOfCharacters, u8 BitsPerPixel)
{
    switch(BitsPerPixel)
    {
        case 4:
            memcpy32((u16*)(MEM_OVRAM + (FirstCharacter * 32)), (u16*)CharacterSetSource, (NumberOfCharacters * 8));
            break;
        case 8:
            memcpy32((u16*)(MEM_OVRAM + (FirstCharacter * 64)), (u16*)CharacterSetSource, (NumberOfCharacters * 16));
            break;
        default:
            break;
    }
}

void sprites_configure(u8 Index, u16 Tile, u8 BitsPerPixel, u8 PaletteIndex, SpriteSize Size)
{
    u16 bppmap = ATTR0_8BPP;
    switch(BitsPerPixel)
    {
        case 4:
            bppmap = ATTR0_4BPP;
            break;
        case 8:
            bppmap = ATTR0_8BPP;
            break;
        default:
            bppmap = ATTR0_8BPP;
            break;
    }
    switch(Size)
    {
        case SPRITE_SIZE_8x8:
            OAM[Index].attr0 = (OAM[Index].attr0 & 0x00FF) | bppmap | ATTR0_SQUARE;
            OAM[Index].attr1 = (OAM[Index].attr1 & 0x3FFF) | ATTR1_SIZE_8;
            break;
        case SPRITE_SIZE_16x16:
            OAM[Index].attr0 = (OAM[Index].attr0 & 0x00FF) | bppmap | ATTR0_SQUARE;
            OAM[Index].attr1 = (OAM[Index].attr1 & 0x3FFF) | ATTR1_SIZE_16;
            break;
        case SPRITE_SIZE_32x32:
            OAM[Index].attr0 = (OAM[Index].attr0 & 0x00FF) | bppmap | ATTR0_SQUARE;
            OAM[Index].attr1 = (OAM[Index].attr1 & 0x3FFF) | ATTR1_SIZE_32;
            break;
        case SPRITE_SIZE_64x64:
            OAM[Index].attr0 = (OAM[Index].attr0 & 0x00FF) | bppmap | ATTR0_SQUARE;
            OAM[Index].attr1 = (OAM[Index].attr1 & 0x3FFF) | ATTR1_SIZE_64;
            break;
        case SPRITE_SIZE_16x8:
            OAM[Index].attr0 = (OAM[Index].attr0 & 0x00FF) | bppmap | ATTR0_WIDE;
            OAM[Index].attr1 = (OAM[Index].attr1 & 0x3FFF) | ATTR1_SIZE_8;
            break;
        case SPRITE_SIZE_32x8:
            OAM[Index].attr0 = (OAM[Index].attr0 & 0x00FF) | bppmap | ATTR0_WIDE;
            OAM[Index].attr1 = (OAM[Index].attr1 & 0x3FFF) | ATTR1_SIZE_16;
            break;
        case SPRITE_SIZE_32x16:
            OAM[Index].attr0 = (OAM[Index].attr0 & 0x00FF) | bppmap | ATTR0_WIDE;
            OAM[Index].attr1 = (OAM[Index].attr1 & 0x3FFF) | ATTR1_SIZE_32;
            break;
    }
    OAM[Index].attr2 = (OAM[Index].attr2 & 0x0C00) | ATTR2_ID(Tile) | ATTR2_PALBANK(PaletteIndex);
}

void sprites_position(u8 Index, u8 Plane, u8 Visible, s16 X, s16 Y, SpriteMirror Flip)
{
    OAM[Index].attr0 = (OAM[Index].attr0 & 0xFE00) | (ATTR0_Y( Y ) & ATTR0_Y_MASK) | (Visible == 0 ? ATTR0_HIDE : 0);
    OAM[Index].attr1 = (OAM[Index].attr1 & 0xCE00) | (ATTR1_X( X ) & ATTR1_X_MASK) | ATTR1_FLIP( Flip );
    OAM[Index].attr2 = (OAM[Index].attr2 & 0xF3FF) | (ATTR2_PRIO(3 - Plane) & ATTR2_PRIO_MASK);
}

void sprites_animate(u8 Index, u16 Tile, u8 PaletteIndex)
{
    OAM[Index].attr2 = (OAM[Index].attr2 & 0x0C00) | ATTR2_ID(Tile) | ATTR2_PALBANK(PaletteIndex);
}

void sprites_position_get(u8 Index, u8* Plane, s16* X, s16* Y)
{
    *Plane = (OAM[Index].attr2 & ATTR2_PRIO_MASK) >> ATTR2_PRIO_SHIFT;
    *X = (OAM[Index].attr1 & ATTR1_X_MASK);
    *Y = (OAM[Index].attr0 & ATTR0_Y_MASK);
}
