
#include "allegro.h"
#include "../include/kit.h"

TileDisplay tilemap_display[MAX_DISPLAYS];

void tilemap_characterset_set(u8 DisplayIndex, u8* CharacterSetSource, u16 FirstCharacter, u16 NumberOfCharacters, u8 BitsPerPixel)
{
    size_t copylen = 0;
    switch(BitsPerPixel)
    {
        case 4:
            copylen = 32;
            break;
        case 8:
            copylen = 64;
            break;
    }
    memcpy(&tilemap_display[DisplayIndex].CharacterData[FirstCharacter * copylen], CharacterSetSource, NumberOfCharacters * copylen);
    for(u16 c = FirstCharacter; c < FirstCharacter + NumberOfCharacters; c++)
    {
        tilemap_display[DisplayIndex].CharacterDirty[c] = 1;
    }
}

u8   tilemap_characterset_getpixel(u8 DisplayIndex, u16 Character, u8 BitsPerPixel, s16 X, s16 Y)
{
    u8 colourindex = 0;
    if( Character > 1023 || X < 0 || X > 7 || Y < 0 || Y > 7) { return colourindex; }
    switch(BitsPerPixel)
    {
        case 4:
            colourindex = tilemap_display[DisplayIndex].CharacterData[(Character * 32) + (Y * 4) + X];
            // TODO: Finish parsing pixel data
            break;
        case 8:
            colourindex = tilemap_display[DisplayIndex].CharacterData[(Character * 64) + (Y * 8) + X];
            break;
    }
    return colourindex;
}

void tilemap_characterset_setpixel(u8 DisplayIndex, u16 Character, u8 BitsPerPixel, s16 X, s16 Y, u8 PaletteIndex)
{
    if( Character > 1023 || X < 0 || X > 7 || Y < 0 || Y > 7) { return; }
    switch(BitsPerPixel)
    {
        case 4:
            break;
        case 8:
            tilemap_display[DisplayIndex].CharacterData[(Character * 64) + (Y * 8) + X] = PaletteIndex;
            break;
    }
    tilemap_display[DisplayIndex].CharacterDirty[Character] = 1;
}

void tilemap_map_settile(u8 DisplayIndex, u8 Plane, u16 X, u16 Y, u16 Tile)
{
    if (X < 0 || Y < 0 || X > 31 || Y > 31)
    {
        return;
    }
    tilemap_display[DisplayIndex].Planes[Plane].TileIndex[(Y * 32) + X] = Tile;

    /*
    ALLEGRO_LOCKED_REGION* rgn = al_lock_bitmap_region(tilemap_display[DisplayIndex].Planes[Plane].Canvas, X * 8, Y * 8, 8, 8, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_WRITEONLY);

    u16 actualtile = (Tile & 0x03FF);
    u16 flipy = (Tile & 0x0800);
    u16 flipx = (Tile & 0x0400);
    u8* p = (u8*)rgn->data;

    for (int y = 0; y < 8; y++)
    {
        int ty = (flipy == 0 ? y : 7 - y);	// Mirror Y
        for (int x = 0; x < 8; x++)
        {
            int tx = (flipx == 0 ? x : 7 - x);	// Mirror X
            u8 palidx = 0;
            switch (tilemap_display[DisplayIndex].Planes[Plane].BitsPerPixel)
            {
                case 4:
                    palidx = tilemap_display[DisplayIndex].CharacterData[(actualtile * 20) + (ty * 4) + (tx >> 1)];
                    palidx = (tx % 2 == 0 ? palidx & 0x0F : palidx >> 4);
                    break;
                case 8:
                    palidx = tilemap_display[DisplayIndex].CharacterData[(actualtile * 64) + (ty * 8) + tx];
                    break;
            }
            int b = (rgn->pitch * y) + (rgn->pixel_size * x);
            p[b + 0] = palette_screen[palidx].R;
            p[b + 1] = palette_screen[palidx].G;
            p[b + 2] = palette_screen[palidx].B;
            p[b + 3] = (palidx == 0 ? 0 : 255);
        }
    }
    al_unlock_bitmap(tilemap_display[DisplayIndex].Planes[Plane].Canvas);
    */
}

u16  tilemap_map_gettile(u8 DisplayIndex, u8 Plane, u16 X, u16 Y)
{
    if (X < 0 || Y < 0 || X > 31 || Y > 31) { return 0; }
    return tilemap_display[DisplayIndex].Planes[Plane].TileIndex[(Y * 32) + X];
}

void tilemap_map_setposition(u8 DisplayIndex, u8 Plane, u16 X, u16 Y)
{
    tilemap_display[DisplayIndex].Planes[Plane].ScrollX = X & 0x00FF;
    tilemap_display[DisplayIndex].Planes[Plane].ScrollY = Y & 0x00FF;
}


void tilemap_map_refreshcache(u8 DisplayIndex, u8 Plane)
{
    ALLEGRO_LOCKED_REGION* rgn = al_lock_bitmap(tilemap_display[DisplayIndex].Planes[Plane].Canvas, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_WRITEONLY);
    for(int Y = 0; Y < 32; Y++)
    {
        for(int X = 0; X < 32; X++)
        {
            u16 Tile = tilemap_display[DisplayIndex].Planes[Plane].TileIndex[(Y * 32) + X];

            u16 actualtile = (Tile & 0x03FF);
            u16 flipy = (Tile & 0x0800);
            u16 flipx = (Tile & 0x0400);
            u8* p = (u8*)rgn->data;

            for (int y = 0; y < 8; y++)
            {
                int ty = (flipy == 0 ? y : 7 - y);	// Mirror Y
                for (int x = 0; x < 8; x++)
                {
                    int tx = (flipx == 0 ? x : 7 - x);	// Mirror X
                    u8 palidx = 0;
                    switch (tilemap_display[DisplayIndex].Planes[Plane].BitsPerPixel)
                    {
                        case 4:
                            palidx = tilemap_display[DisplayIndex].CharacterData[(actualtile * 20) + (ty * 4) + (tx >> 1)];
                            palidx = (tx % 2 == 0 ? palidx & 0x0F : palidx >> 4);
                            break;
                        case 8:
                            palidx = tilemap_display[DisplayIndex].CharacterData[(actualtile * 64) + (ty * 8) + tx];
                            break;
                    }
                    int b = (rgn->pitch * ((Y * 8) + y)) + (rgn->pixel_size * ((X * 8) + x));
                    p[b + 0] = palette_screen[palidx].R;
                    p[b + 1] = palette_screen[palidx].G;
                    p[b + 2] = palette_screen[palidx].B;
                    p[b + 3] = (palidx == 0 ? 0 : 255);
                }
            }
        }
    }
    al_unlock_bitmap(tilemap_display[DisplayIndex].Planes[Plane].Canvas);
}
