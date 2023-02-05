
#pragma once

#include "typedefs.h"

typedef enum OvermapTileSize
{
    SIZE_8x8          = 0x0000,
    SIZE_16x16        = 0x0001,
    SIZE_32x32        = 0x0002,
    SIZE_64x64        = 0x0003,
} OvermapTileSize;

typedef enum OvermapMirror
{
    MIRROR_NONE       = 0x0000,
    MIRROR_HORIZONTAL = 0x0001,
    MIRROR_VERTICAL   = 0x0002,
    MIRROR_BOTH       = 0x0003,
} OvermapMirror;

typedef struct OvermapTile
{
    u16 TileIndex;
    u8 Palette;     // For 4bpp maps
    OvermapMirror Mirror;
} OvermapTile;

typedef struct Overmap
{
    u16 MapWidth;
    u16 MapHeight;
    OvermapTileSize TileSize;
    OvermapTile* TileData;
} Overmap;

void tilemap_characterset_set(u8 DisplayIndex, u8* CharacterSetSource, u16 FirstCharacter, u16 NumberOfCharacters, u8 BitsPerPixel);
u8   tilemap_characterset_getpixel(u8 DisplayIndex, u16 Character, u8 BitsPerPixel, s16 X, s16 Y);
void tilemap_characterset_setpixel(u8 DisplayIndex, u16 Character, u8 BitsPerPixel, s16 X, s16 Y, u8 PaletteIndex);

void tilemap_map_settile(u8 DisplayIndex, u8 Plane, u16 X, u16 Y, u16 Tile);
void tilemap_map_settile_ex(u8 DisplayIndex, u8 Plane, u16 X, u16 Y, u16 Tile, OvermapMirror Flip);
u16  tilemap_map_gettile(u8 DisplayIndex, u8 Plane, u16 X, u16 Y);
void tilemap_map_setposition(u8 DisplayIndex, u8 Plane, u16 X, u16 Y);
void tilemap_map_print(u8 DisplayIndex, u8 Plane, u16 X, u16 Y, u8* Text);

void tilemap_map_configure(u8 Plane, Overmap* Map);
void tilemap_map_viewport_set(u8 Plane, u32 CameraX, u32 CameraY);
void tilemap_map_viewport_scroll(u8 Plane, s32 DeltaX, s32 DeltaY);
void tilemap_map_viewport_get(u8 Plane, u32* CameraX, u32* CameraY);
