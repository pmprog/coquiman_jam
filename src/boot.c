
#include "boot.h"
#include <kit.h>
#include "../resources/res_palette_game.h"
#include "../resources/res_graphics_title.h"
#include "../resources/res_map_title.h"

static const Colour colour_black = { 0, 0, 0 };

static u16 x = 0;
static u16 y = 0;
static u16 c = 1;

StageRecord bootstage = { bootstage_start, bootstage_update, bootstage_finish };

void bootstage_start()
{
    input_keyboard_as_controller(1);
    palette_setscreencolours(0, palette_game_len, palette_game);
    palette_setspritecolours(0, palette_game_len, palette_game);

    palette_setscreencolours(0, 1, &colour_black);
    palette_setspritecolours(0, 1, &colour_black);

    tilemap_characterset_set(0, gfx_titlebkg, 1, 510, 8);  // Load up the map characters

    sprites_clear();

    music_play(0);
}

void bootstage_finish()
{

}

void bootstage_update()
{

    if(kit_stage_time % 16 == 0)
    {
        tilemap_map_settile( 0, 0, x, y, map_title_gfx_titlebkg_map_data[ (y * map_title_gfx_titlebkg_map_width) + x ] );
        if( c < 510) {c++;}
        x = (x + 1) % 30;
        if(x == 0) { y++;}
    }
}
