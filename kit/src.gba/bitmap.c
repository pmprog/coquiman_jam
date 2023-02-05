
#include "../include/bitmap.h"
#include "../include/kit.h"
#include "gba.h"

void bitmap_setpixel(u8 DisplayIndex, s16 X, s16 Y, u8 PaletteIndex)
{
    // Check dimensions of display's canvas
    if( DisplayIndex != 0 || X < 0 || X >= 240 || Y < 0 || Y >= 160)
    {
        return;
    }
    u16* ptr = (u16*)&vid_page[ ( (Y * 240) + X) >> 1 ];  // Division by 2 due to s16/u16 pos16er mismatch!
    if( X & 1 )
    {
        *ptr = (*ptr & 0x00FF) | ((u16)PaletteIndex<<8);    // Right pixel
    }
    else
    {
        *ptr = (*ptr & 0xFF00) | PaletteIndex;        // Left pixel
    }
}

void bitmap_line_horizontal(u8 DisplayIndex, s16 X1, s16 Y, s16 X2, u8 PaletteIndex)
{
    // Clamp to screen bounds
    s16 lx = (X1 < X2 ? X1 : X2);
    s16 rx = (X1 > X2 ? X1 : X2);
    lx = (lx < 0 ? 0 : lx);
    rx = (rx >= 240 ? 239 : rx);

    for (s16 x = lx; x <= rx; x++)
    {
        bitmap_setpixel(DisplayIndex, x, Y, PaletteIndex);
    }
}
