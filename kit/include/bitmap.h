
#pragma once

#include "typedefs.h"

void bitmap_setpixel(u8 DisplayIndex, s16 X, s16 Y, u8 PaletteIndex);
void bitmap_line_horizontal(u8 DisplayIndex, s16 X1, s16 Y, s16 X2, u8 PaletteIndex);
void bitmap_line(u8 DisplayIndex, s16 X1, s16 Y1, s16 X2, s16 Y2, u8 PaletteIndex);
void bitmap_rectangle(u8 DisplayIndex, s16 X, s16 Y, s16 Width, s16 Height, u8 PaletteIndex);
void bitmap_triangle(u8 DisplayIndex, s16 X1, s16 Y1, s16 X2, s16 Y2, s16 X3, s16 Y3, u8 PaletteIndex);
void bitmap_circle(u8 DisplayIndex, s16 X, s16 Y, u8 Radius, u8 PaletteIndex);
void bitmap_polygon(u8 DisplayIndex, s16* X, s16* Y, s16 PointCount, u8 PaletteIndex);

void bitmap_blit256(u8 DisplayIndex, u8* ImageData, u16 ImageWidth, u16 ImageHeight, u16 DestinationX, u16 DestinationY);
void bitmap_blit256sub(u8 DisplayIndex, u8* ImageData, u16 ImageWidth, u16 ImageHeight, u16 DestinationX, u16 DestinationY, u16 SourceX, u16 SourceY, u16 SourceWidth, u16 SourceHeight);
