
#include "boot.h"
#include <kit.h>
#include "../resources/res_palette_game.h"
#include "../resources/res_graphics_title.h"
#include "../resources/res_map_title.h"
#include "intro.h"

static const Colour colour_black = { 0, 0, 0 };
static const Colour colour_white = { 255, 255, 255 };

static u16 x = 0;
static u16 y = 0;
static u16 c = 1;

static s8 drawy[30] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

StageRecord bootstage = { bootstage_start, bootstage_update, bootstage_finish };

void bootstage_start()
{
    input_keyboard_as_controller(1);
    palette_setscreencolours(0, palette_game_len, palette_game);
    palette_setspritecolours(0, palette_game_len, palette_game);

    palette_setscreencolours(0, 1, &colour_white);
    palette_setspritecolours(0, 1, &colour_white);

    tilemap_characterset_set(0, gfx_titlebkg_tiled, 0, gfx_titlebkg_tiled_tile_count, 8);  // Load up the map characters

    sprites_clear();

    music_play(0);
}

void bootstage_finish()
{

}

void bootstage_update()
{
    if(kit_stage_time % 4 != 0) {return;}

    u8 completed = 0;

    for(u8 x = 0; x < 30; x++)
    {
        if(drawy[x] < 20)
        {
            u8 targety = drawy[x] + ((random_next() & 0x0001) + 1);
            for(s8 y = drawy[x]; y <= targety; y++ )
            {
                if( y >= 0 && y < 20 )
                {
                    tilemap_map_settile( 0, 0, x, y, map_title_gfx_titlebkg_map_data[ (y * map_title_gfx_titlebkg_map_width) + x ] );
                }
            }
            drawy[x] = targety;
        }
        else
        {
            completed++;
        }
    }

    if(completed == 30)
    {
        kit_change_stage(&introstage);
    }
}
