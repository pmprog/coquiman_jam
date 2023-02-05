
#include "../include/bitmap.h"

#ifndef abs
#define abs(v)      (v < 0 ? v * -1 : v)
#endif

void bitmap_line(u8 DisplayIndex, s16 X1, s16 Y1, s16 X2, s16 Y2, u8 PaletteIndex)
{
    if (Y1 == Y2)
    {
        bitmap_line_horizontal(DisplayIndex, X1, Y1, X2, PaletteIndex);
        return;
    }
    if (X1 == X2)
    {
        for (s16 y = (Y1 < Y2 ? Y1 : Y2); y < (Y1 > Y2 ? Y1 : Y2); y++)
        {
            bitmap_setpixel(DisplayIndex, X1, y, PaletteIndex);
        }
        return;
    }

    s16 steep = 0;
    if (abs(X2 - X1) < abs(Y2 - Y1))
    {
        s16 t;
        t = X1; X1 = Y1; Y1 = t;
        t = X2; X2 = Y2; Y2 = t;
        steep = 1;
    }
    if (X1 > X2)
    {
        s16 t;
        t = X1; X1 = X2; X2 = t;
        t = Y1; Y1 = Y2; Y2 = t;
    }
    s16 dx = X2 - X1;
    s16 dy = Y2 - Y1;
    s16 derror2 = abs(dy) * 2;
    s16 error2 = 0;
    s16 y = Y1;
    for (s16 x = X1; x <= X2; x++)
    {
        if (steep == 1)
        {
            bitmap_setpixel(DisplayIndex, y, x, PaletteIndex);
        }
        else
        {
            bitmap_setpixel(DisplayIndex, x, y, PaletteIndex);
        }
        error2 += derror2;
        if (error2 > dx)
        {
            y += (Y2 > Y1 ? 1 : -1);
            error2 -= dx * 2;
        }
    }

}

void bitmap_rectangle(u8 DisplayIndex, s16 X, s16 Y, s16 Width, s16 Height, u8 PaletteIndex)
{
    bitmap_line_horizontal(DisplayIndex, X, Y, X + Width, PaletteIndex);
    bitmap_line_horizontal(DisplayIndex, X, Y + Height, X + Width, PaletteIndex);
    bitmap_line(DisplayIndex, X, Y, X, Y + Height, PaletteIndex);
    bitmap_line(DisplayIndex, X + Width, Y, X + Width, Y + Height, PaletteIndex);
}

void bitmap_triangle(u8 DisplayIndex, s16 X1, s16 Y1, s16 X2, s16 Y2, s16 X3, s16 Y3, u8 PaletteIndex)
{
    bitmap_line(DisplayIndex, X1, Y1, X2, Y2, PaletteIndex);
    bitmap_line(DisplayIndex, X1, Y1, X3, Y3, PaletteIndex);
    bitmap_line(DisplayIndex, X3, Y3, X2, Y2, PaletteIndex);
}

void bitmap_circle(u8 DisplayIndex, s16 X, s16 Y, u8 Radius, u8 PaletteIndex)
{
    s16 u = 1;
    s16 v = 2 * Radius - 1;
    s16 E = 0;
    s16 x = 0;
    s16 y = Radius;

    while (x < y)
    {
        bitmap_setpixel(DisplayIndex, X + x, Y + y, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X + x, Y - y, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X - x, Y - y, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X - x, Y + y, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X + y, Y + x, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X + y, Y - x, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X - y, Y - x, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X - y, Y + x, PaletteIndex);
        x++; E += u; u += 2;
        if (v < 2 * E)
        {
            y--;
            E -= v;
            v -= 2;
        }
        if (x > y) break;
        bitmap_setpixel(DisplayIndex, X + x, Y + y, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X + x, Y - y, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X - x, Y - y, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X - x, Y + y, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X + y, Y + x, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X + y, Y - x, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X - y, Y - x, PaletteIndex);
        bitmap_setpixel(DisplayIndex, X - y, Y + x, PaletteIndex);
    }
}

void bitmap_polygon(u8 DisplayIndex, s16* X, s16* Y, s16 PointCount, u8 PaletteIndex)
{
    for (s16 current = 0; current < PointCount; current++)
    {
        s16 next = (current + 1) % PointCount;
        bitmap_line(DisplayIndex, X[current], Y[current], X[next], Y[next], PaletteIndex);
    }
}

void bitmap_blit256(u8 DisplayIndex, u8* ImageData, u16 ImageWidth, u16 ImageHeight, u16 DestinationX, u16 DestinationY)
{

}

void bitmap_blit256sub(u8 DisplayIndex, u8* ImageData, u16 ImageWidth, u16 ImageHeight, u16 DestinationX, u16 DestinationY, u16 SourceX, u16 SourceY, u16 SourceWidth, u16 SourceHeight)
{

}
