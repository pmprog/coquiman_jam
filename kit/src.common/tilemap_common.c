
#include "../include/tilemap.h"
#include "../include/kit.h"

typedef struct OvermapInternal
{
    Overmap* Map;
    u8  PlaneX;                  // Plane X/Y scroll (pixels)
    u8  PlaneY;
    u16 PlaneRowTile[32];
    u8  PlaneRowSubTile[32];
    u16 PlaneColumnTile[32];
    u8  PlaneColumnSubTile[32];
    u32 ViewX;                  // View offset (pixels)
    u32 ViewY;
} OvermapInternal;

void tilemap_map_settile_ex(u8 DisplayIndex, u8 Plane, u16 X, u16 Y, u16 Tile, OvermapMirror Flip)
{
    u16 tile = (Tile & 0x3FF) | (Flip << 10);
    tilemap_map_settile(DisplayIndex, Plane, X, Y, tile);
}

void tilemap_map_print(u8 DisplayIndex, u8 Plane, u16 X, u16 Y, u8* Text)
{
    u8* chrptr = Text;
    u8 chr = *chrptr;
    u16 x = X;
    while(chr != 0)
    {
        tilemap_map_settile(DisplayIndex, Plane, x, Y, (u16)chr);
        chrptr++; x++;
        chr = *chrptr;
    }
}
