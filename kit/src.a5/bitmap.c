
#include "../include/display.h"
#include "../include/kit.h"
#include "allegro.h"

void bitmap_setpixel(u8 DisplayIndex, s16 X, s16 Y, u8 PaletteIndex)
{
    // Check dimensions of display's canvas
    if( X < 0 || X >= al_get_bitmap_width(display_canvas[DisplayIndex]) || Y < 0 || Y >= al_get_bitmap_height(display_canvas[DisplayIndex]))
    {
        return;
    }
    // Don't draw transparent colours
    if( PaletteIndex == 0 )
    {
        return;
    }
    char* p = (char*)display_bufferlock[DisplayIndex]->data;
    int b = (display_bufferlock[DisplayIndex]->pitch * Y) + (display_bufferlock[DisplayIndex]->pixel_size * X);
    p[b + 0] = palette_screen[PaletteIndex].R;
    p[b + 1] = palette_screen[PaletteIndex].G;
    p[b + 2] = palette_screen[PaletteIndex].B;
    p[b + 3] = 255;
}

void bitmap_line_horizontal(u8 DisplayIndex, s16 X1, s16 Y, s16 X2, u8 PaletteIndex)
{
    // Clamp to screen bounds
    s16 lx = (X1 < X2 ? X1 : X2);
    s16 rx = (X1 > X2 ? X1 : X2);
    lx = (lx < 0 ? 0 : lx);
    rx = (rx >= al_get_bitmap_width(display_canvas[DisplayIndex]) ? al_get_bitmap_width(display_canvas[DisplayIndex]) - 1 : rx);

    for (s16 x = lx; x <= rx; x++)
    {
        bitmap_setpixel(DisplayIndex, x, Y, PaletteIndex);
    }
}
