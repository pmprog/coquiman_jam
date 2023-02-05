
#include "../include/sprites.h"
#include "../include/kit.h"
#include "allegro.h"

const u8 sprite_size_width[]  = {  8, 16, 32, 64, 16, 32, 32 };
const u8 sprite_size_height[] = {  8, 16, 32, 64,  8,  8, 16 };

SpriteData sprites[128];
u8 sprite_character_data[0x20000];

void sprites_redrawsprite(u8 Index);

void sprites_clear()
{
    for(u8 i = 0; i < 128; i++)
    {
        sprites[i].graphic_visible = 0;
    }
}

void sprites_characterset_set(u8* CharacterSetSource, u16 FirstCharacter, u16 NumberOfCharacters, u8 BitsPerPixel)
{
    size_t bytestocopy = NumberOfCharacters;
    switch(BitsPerPixel)
    {
        case 4:
            memcpy(&sprite_character_data[(FirstCharacter * 32)], CharacterSetSource, (bytestocopy * 32));
            break;
        case 8:
            memcpy(&sprite_character_data[(FirstCharacter * 64)], CharacterSetSource, (bytestocopy * 64));
            break;
    }
    sprites_refreshcache();
}

void sprites_configure(u8 Index, u16 Tile, u8 BitsPerPixel, u8 PaletteIndex, SpriteSize Size)
{
    sprites[Index].graphic_size = Size;
    sprites[Index].graphic_bpp = BitsPerPixel;
    if(sprites[Index].cached_image != NULL)
    {
        al_destroy_bitmap(sprites[Index].cached_image);
    }
    sprites[Index].cached_image = al_create_bitmap(sprite_size_width[Size], sprite_size_height[Size]);
    sprites_animate(Index, Tile, PaletteIndex);
}

void sprites_position(u8 Index, u8 Plane, u8 Visible, s16 X, s16 Y, SpriteMirror Flip)
{
    sprites[Index].position_x = X;
    sprites[Index].position_y = Y;
    sprites[Index].position_plane = Plane;
    sprites[Index].graphic_visible = Visible;
    sprites[Index].graphic_flip = Flip;
}

void sprites_animate(u8 Index, u16 Tile, u8 PaletteIndex)
{
    sprites[Index].graphic_palette = PaletteIndex;
    sprites[Index].graphic_tile = Tile;
    sprites_redrawsprite(Index);
}

void sprites_position_get(u8 Index, u8* Plane, s16* X, s16* Y)
{
    *Plane = sprites[Index].position_plane;
    *X = sprites[Index].position_x;
    *Y = sprites[Index].position_y;
}

void sprites_refreshcache()
{
    for(u8 i = 0; i < 128; i++)
    {
        if(sprites[i].graphic_visible != 0)
        {
            sprites_redrawsprite(i);
        }
    }
}

void sprites_redrawsprite(u8 Index)
{
    u8 tilewidth = sprite_size_width[sprites[Index].graphic_size] >> 3;
    u8 tilehigh = sprite_size_height[sprites[Index].graphic_size] >> 3;
    // Render Cached Image
    ALLEGRO_LOCKED_REGION* rgn = al_lock_bitmap(sprites[Index].cached_image, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_WRITEONLY);
    u16 curtile = sprites[Index].graphic_tile >> 1;
    u8* p = (u8*)rgn->data;

    u8 hflip = sprites[Index].graphic_flip & SPRITE_MIRROR_HORIZONTAL;
    u8 vflip = sprites[Index].graphic_flip & SPRITE_MIRROR_VERTICAL;

    for(int ty = 0; ty < tilehigh; ty++)
    {
        int tyu = (vflip == 0 ? ty : tilehigh - 1 - ty);
        for(int tx = 0; tx < tilewidth; tx++)
        {
            int txu = (hflip == 0 ? tx : tilewidth - 1 - tx);
            for(int y = 0; y < 8; y++)
            {
                int yu = (vflip == 0 ? y : 7 - y);
                for(int x = 0; x < 8; x++)
                {
                    int xu = (hflip == 0 ? x : 7 - x);
                    u8 palidx = 0;
                    switch (sprites[Index].graphic_bpp)
                    {
                        case 4:
                            palidx = sprite_character_data[(curtile * 32) + (yu * 4) + (xu >> 1)];
                            palidx = (txu % 2 == 0 ? palidx & 0x0F : palidx >> 4);
                            break;
                        case 8:
                            palidx = sprite_character_data[(curtile * 64) + (yu * 8) + xu];
                            break;
                    }
                    int b = (rgn->pitch * ((tyu * 8) + y)) + (rgn->pixel_size * ((txu * 8) + x));
                    p[b + 0] = (palidx == 0 ? 0 : palette_sprites[palidx].R);
                    p[b + 1] = (palidx == 0 ? 0 : palette_sprites[palidx].G);
                    p[b + 2] = (palidx == 0 ? 0 : palette_sprites[palidx].B);
                    p[b + 3] = (palidx == 0 ? 0 : 255);
                }
            }
            curtile++;
        }
    }

    al_unlock_bitmap(sprites[Index].cached_image);
}
