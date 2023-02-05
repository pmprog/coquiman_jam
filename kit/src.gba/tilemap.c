
#include "../include/tilemap.h"
#include "../include/kit.h"
#include "gba.h"


typedef struct MapConfiguration
{
    u32 MapWidthPixels;
    u32 MapHeightPixels;
    u8  PlaneX;                  // Plane X/Y scroll (pixels)
    u8  PlaneY;
    u16 PlaneRowTile[32];
    u8  PlaneRowSubTile[32];
    u16 PlaneColumnTile[32];
    u8  PlaneColumnSubTile[32];
    u32 ViewX;                  // View offset (pixels)
    u32 ViewY;
} MapConfiguration;

/*
typedef struct TileSetPosition
{
    TileSet* Set;
    u16      StartIndex;
} TileSetPosition;
*/

static const u32 mapex_submasks[] = { 0x00, 0x01, 0x03 };
static const u8 mapex_subadvance[] = { 0x00, 0x02, 0x04 };
static const u8 mapex_subtiles[] = { 0x01, 0x04, 0x10 };

Overmap* map_data[4];
MapConfiguration map_config[4];
//TileSetPosition set_config[MAX_ANIMATED_TILESETS];
u8 set_config_index = 0;

void tilemap_overmap_settile(u8 Plane, u16 X, u16 Y, OvermapTile* UseTile, u8 SubTileX, u8 SubTileY);
void tilemap_overmap_reversetile(u8 Plane, u32* Tile, u32* SubTile, int MaxTile);
static void tilemap_overmap_advancetile(u8 Plane, u32* Tile, u32* SubTile, int MaxTile);
void tilemap_overmap_fillrow(u8 Plane, u8 RenderRow, u16 MapRow, u8 MapSubRow);
void tilemap_overmap_fillcol(u8 Plane, u8 RenderCol, u16 MapCol, u8 MapSubCol);


void tilemap_characterset_set(u8 DisplayIndex, u8* CharacterSetSource, u16 FirstCharacter, u16 NumberOfCharacters, u8 BitsPerPixel)
{
    memcpy32((u16*)(MEM_VRAM + (FirstCharacter * 32)), (u16*)CharacterSetSource, (NumberOfCharacters * (BitsPerPixel * 2) ));
}

static u16* tilemap_characterset_getptr(u8 DisplayIndex, u16 Character, u8 BitsPerPixel)
{
    return (u16*)(MEM_VRAM + (Character * (BitsPerPixel * 8)));
}

u8   tilemap_characterset_getpixel(u8 DisplayIndex, u16 Character, u8 BitsPerPixel, s16 X, s16 Y)
{
    if( X < 0 || X >= 8 || Y < 0 || Y >= 8)
    {
        return 0;
    }
    u8 targetindex = ((Y * 8) + X);
    u16* targetchar = &tilemap_characterset_getptr(DisplayIndex, Character, BitsPerPixel)[targetindex];
    if( X & 1 )
    {
        return (*targetchar & 0xFF00) >> 8;  // Right pixel
    }
    return (*targetchar & 0x00FF);  // Left pixel
}

void tilemap_characterset_setpixel(u8 DisplayIndex, u16 Character, u8 BitsPerPixel, s16 X, s16 Y, u8 PaletteIndex)
{
    if( X < 0 || X >= 8 || Y < 0 || Y >= 8)
    {
        return;
    }
    u8 targetindex = ((Y * 8) + X) >> 1;
    u16* targetchar = &tilemap_characterset_getptr(DisplayIndex, Character, BitsPerPixel)[targetindex];
    if( X & 1 )
    {
        *targetchar = (*targetchar & 0x00FF) | (PaletteIndex<<8);    // Right pixel
    }
    else
    {
        *targetchar = (*targetchar & 0xFF00) | PaletteIndex;        // Left pixel
    }
}

void tilemap_map_settile(u8 DisplayIndex, u8 Plane, u16 X, u16 Y, u16 Tile)
{
    u16* base = (u16*)MAP_BASE_ADR((Plane * 1) + 26);
    base[(Y * 32) + X] = Tile;
}

u16  tilemap_map_gettile(u8 DisplayIndex, u8 Plane, u16 X, u16 Y)
{
    u16* base = (u16*)MAP_BASE_ADR((Plane * 1) + 26) + (Y * 32);
    return base[X];
}

void tilemap_map_setposition(u8 DisplayIndex, u8 Plane, u16 X, u16 Y)
{
    switch( Plane )
    {
        case 0:
            REG_BG0HOFS = X;
            REG_BG0VOFS = Y;
            break;
        case 1:
            REG_BG1HOFS = X;
            REG_BG1VOFS = Y;
            break;
        case 2:
            REG_BG2HOFS = X;
            REG_BG2VOFS = Y;
            break;
        case 3:
            REG_BG3HOFS = X;
            REG_BG3VOFS = Y;
            break;
    }
}

void tilemap_map_configure(u8 Plane, Overmap* Map)
{
    map_data[Plane] = Map;
    map_config[Plane].MapWidthPixels = (map_data[Plane]->MapWidth * ((map_data[Plane]->TileSize + 1) * 8));
    map_config[Plane].MapHeightPixels = (map_data[Plane]->MapHeight * ((map_data[Plane]->TileSize + 1) * 8));
    tilemap_map_viewport_set(Plane, 0, 0);
}

void tilemap_map_viewport_set(u8 Plane, u32 CameraX, u32 CameraY)
{
    // ViewXY points to the camera pixel position
    map_config[Plane].ViewX = CameraX % map_config[Plane].MapWidthPixels;
    map_config[Plane].ViewY = CameraY % map_config[Plane].MapHeightPixels;
    // PlaneXY as the scroll position
    map_config[Plane].PlaneX = (u8)(CameraX & 0x00FF);
    map_config[Plane].PlaneY = (u8)(CameraY & 0x00FF);

    u8 currentcol_render = map_config[Plane].PlaneX >> 3;
    currentcol_render = (currentcol_render == 0 ? 31 : currentcol_render - 1);
    u32 currentcol_map = map_config[Plane].ViewX >> (3 + map_data[Plane]->TileSize);
    u32 currentcol_mapsub = (map_config[Plane].ViewX >> 3) & mapex_submasks[map_data[Plane]->TileSize];
    tilemap_overmap_reversetile(Plane, &currentcol_map, &currentcol_mapsub, map_data[Plane]->MapWidth);
    for (u8 x = 0; x < 32; x++)
    {
        map_config[Plane].PlaneColumnTile[currentcol_render] = currentcol_map;
        map_config[Plane].PlaneColumnSubTile[currentcol_render] = currentcol_mapsub;
        currentcol_render = (u8)((currentcol_render + 1) % 32);
        tilemap_overmap_advancetile(Plane, &currentcol_map, &currentcol_mapsub, map_data[Plane]->MapWidth);
    }

    // Subtracting a row first rotates through the u8 to start at the bottom row
    u8 currentrow_render = map_config[Plane].PlaneY >> 3;
    currentrow_render = (currentrow_render == 0 ? 31 : currentrow_render - 1);
    u32 currentrow_map = map_config[Plane].ViewY >> (3 + map_data[Plane]->TileSize);
    u32 currentrow_mapsub = (map_config[Plane].ViewY >> 3) & mapex_submasks[map_data[Plane]->TileSize];
    tilemap_overmap_reversetile(Plane, &currentrow_map, &currentrow_mapsub, map_data[Plane]->MapHeight);


    for (u8 y = 0; y < 32; y++)
    {
        tilemap_overmap_fillrow(Plane, currentrow_render, currentrow_map, currentrow_mapsub);
        currentrow_render = ((currentrow_render + 1) % 32);
        tilemap_overmap_advancetile(Plane, &currentrow_map, &currentrow_mapsub, map_data[Plane]->MapHeight);
    }
    tilemap_map_setposition(0, Plane, map_config[Plane].PlaneX, map_config[Plane].PlaneY);
}

void tilemap_map_viewport_scroll(u8 Plane, s32 DeltaX, s32 DeltaY)
{
    s32 newx = ((s32)map_config[Plane].ViewX + DeltaX);
    if (newx < 0) { newx += map_config[Plane].MapWidthPixels; }
    newx = newx % map_config[Plane].MapWidthPixels;
    s32 newy = ((s32)map_config[Plane].ViewY + DeltaY);
    if (newy < 0) { newy += map_config[Plane].MapHeightPixels; }
    newy = newy % map_config[Plane].MapHeightPixels;

    if (abs(DeltaX) > 240 || abs(DeltaY) > 160)
    {
        tilemap_map_viewport_set(Plane, (u32)newx, (u32)newy);
        return;
    }
    map_config[Plane].ViewX = newx;
    map_config[Plane].ViewY = newy;

    if (DeltaY != 0)
    {
        u8 row_current = ((map_config[Plane].PlaneY + (DeltaY < 0 ? 0 : 160)) >> 3) % 32;
        s32 plane_target = (s32)map_config[Plane].PlaneY + (DeltaY < 0 ? 0 : 160) + DeltaY;
        if (plane_target < 0) { plane_target += 256; }
        u8 row_target = (u8)(plane_target >> 3) % 32;

        u32 row_map = map_config[Plane].PlaneRowTile[row_current];
        u32 row_mapsub = map_config[Plane].PlaneRowSubTile[row_current];
        if (DeltaY < 0)
        {
            // Cycle backwards to fill in the new rows
            while(row_current != row_target)
            {
                row_current = (row_current + 31) % 32;
                tilemap_overmap_reversetile(Plane, &row_map, &row_mapsub, map_data[Plane]->MapHeight);
                tilemap_overmap_fillrow(Plane, row_current, row_map, row_mapsub);
            }
        }
        else
        {
            // Cycle forwards to fill in the new rows
            while(row_current != row_target)
            {
                row_current = (row_current + 1) % 32;
                tilemap_overmap_advancetile(Plane, &row_map, &row_mapsub, map_data[Plane]->MapHeight);
                tilemap_overmap_fillrow(Plane, row_current, row_map, row_mapsub);
            }
        }
        map_config[Plane].PlaneY = (u8)plane_target - (DeltaY < 0 ? 0 : 160);
        tilemap_map_setposition(0, Plane, map_config[Plane].PlaneX, map_config[Plane].PlaneY);
    }

    if (DeltaX != 0)
    {
        u8 col_current = ((map_config[Plane].PlaneX + (DeltaX < 0 ? 0 : 240)) >> 3) % 32;
        s32 plane_target = (s32)map_config[Plane].PlaneX + (DeltaX < 0 ? 0 : 240) + DeltaX;
        if (plane_target < 0) { plane_target += 256; }
        u8 col_target = (u8)(plane_target >> 3) % 32;

        u32 col_map = map_config[Plane].PlaneColumnTile[col_current];
        u32 col_mapsub = map_config[Plane].PlaneColumnSubTile[col_current];
        if (DeltaX < 0)
        {
            // CXcle backwards to fill in the new cols
            while(col_current != col_target)
            {
                col_current = (col_current + 31) % 32;
                tilemap_overmap_reversetile(Plane, &col_map, &col_mapsub, map_data[Plane]->MapWidth);
                tilemap_overmap_fillcol(Plane, col_current, col_map, col_mapsub);
            }
        }
        else
        {
            // CXcle forwards to fill in the new cols
            while(col_current != col_target)
            {
                col_current = (col_current + 1) % 32;
                tilemap_overmap_advancetile(Plane, &col_map, &col_mapsub, map_data[Plane]->MapWidth);
                tilemap_overmap_fillcol(Plane, col_current, col_map, col_mapsub);
            }
        }
        map_config[Plane].PlaneX = (u8)plane_target - (DeltaX < 0 ? 0 : 240);
        tilemap_map_setposition(0, Plane, map_config[Plane].PlaneX, map_config[Plane].PlaneY);
    }
}

void tilemap_map_viewport_get(u8 Plane, u32* CameraX, u32* CameraY)
{
    *CameraX = map_config[Plane].ViewX;
    *CameraY = map_config[Plane].ViewY;
}

void tilemap_overmap_settile(u8 Plane, u16 X, u16 Y, OvermapTile* UseTile, u8 SubTileX, u8 SubTileY)
{
    u16* base = (u16*)MAP_BASE_ADR((Plane * 2) + 28);
    base[(Y * 32) + X] = UseTile->TileIndex;
}

void tilemap_overmap_reversetile(u8 Plane, u32* Tile, u32* SubTile, int MaxTile)
{
    u32 t = *Tile;
    u32 st = *SubTile;
    if (st == 0)
    {
        t = ((t + MaxTile - 1) % MaxTile);
        st = mapex_submasks[map_data[Plane]->TileSize];
    }
    else
    {
        st--;
    }
    *Tile = t;
    *SubTile = st;
}

static void tilemap_overmap_advancetile(u8 Plane, u32* Tile, u32* SubTile, int MaxTile)
{
    u32 t = *Tile;
    u32 st = *SubTile;
    st = ((st + 1) & mapex_submasks[map_data[Plane]->TileSize]);
    if (st == 0)
    {
        t = ((t + 1) % MaxTile);
    }
    *Tile = t;
    *SubTile = st;
}

void tilemap_overmap_fillrow(u8 Plane, u8 RenderRow, u16 MapRow, u8 MapSubRow)
{
    map_config[Plane].PlaneRowTile[RenderRow] = MapRow;
    map_config[Plane].PlaneRowSubTile[RenderRow] = MapSubRow;

    for (u8 x = 0; x < 32; x++)
    {
        u32 tileoffset = ((MapRow * map_data[Plane]->MapWidth) + map_config[Plane].PlaneColumnTile[x]);
        tilemap_overmap_settile(Plane, x, RenderRow, &map_data[Plane]->TileData[tileoffset], map_config[Plane].PlaneColumnSubTile[x], MapSubRow);
    }
}

void tilemap_overmap_fillcol(u8 Plane, u8 RenderCol, u16 MapCol, u8 MapSubCol)
{
    map_config[Plane].PlaneColumnTile[RenderCol] = MapCol;
    map_config[Plane].PlaneColumnSubTile[RenderCol] = MapSubCol;

    for (u8 x = 0; x < 32; x++)
    {
        u32 tileoffset = ((map_config[Plane].PlaneRowTile[x] * map_data[Plane]->MapWidth) + MapCol);
        tilemap_overmap_settile(Plane, RenderCol, x, &map_data[Plane]->TileData[tileoffset], MapSubCol, map_config[Plane].PlaneRowSubTile[x]);
    }
}
