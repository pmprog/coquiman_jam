
#include "boot.h"
#include <kit.h>
#include "../resources/res_palette.h"

StageRecord bootstage = { bootstage_start, bootstage_update, bootstage_finish };

void bootstage_start()
{
    input_keyboard_as_controller(1);
    palette_setscreencolours(0, palette_game_len, palette_game);
    palette_setspritecolours(0, palette_game_len, palette_game);

    palette_setscreencolours(0, 1, &palette_game[32]);
    palette_setspritecolours(0, 1, &palette_game[32]);

    //tilemap_characterset_set(0, tilesheet, 0, tilesheet_tile_count, 8);  // Load up the map characters
    //sprites_characterset_set(tilesheet, 0, 24, 8);
    sprites_clear();

    music_play(0);
}

void bootstage_finish()
{

}

void bootstage_update()
{
    if (input_controller_button_pressed(0, BUTTON_QUIT))
    {
        kit_change_stage(null);
        return;
    }
    if(kit_stage_time >= 160)
    {
        return;
    }


    switch(kit_stage_time)
    {
        case 64:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 13, 6);
                tilemap_map_settile_ex(0, 1, x, 14, 6, MIRROR_VERTICAL);
            }
            break;
        case 66:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 13, 7);
                tilemap_map_settile_ex(0, 1, x, 14, 7, MIRROR_VERTICAL);
            }
            break;
        case 68:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 13, 8);
                tilemap_map_settile_ex(0, 1, x, 14, 8, MIRROR_VERTICAL);
            }
            break;
        case 70:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 13, 9);
                tilemap_map_settile_ex(0, 1, x, 14, 9, MIRROR_VERTICAL);
            }
            break;
        case 72:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 13, 10);
                tilemap_map_settile_ex(0, 1, x, 14, 10, MIRROR_VERTICAL);
            }
            break;
        case 74:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 13, 11);
                tilemap_map_settile_ex(0, 1, x, 14, 11, MIRROR_VERTICAL);
            }
            break;
        case 76:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 13, 12);
                tilemap_map_settile_ex(0, 1, x, 14, 12, MIRROR_VERTICAL);
            }
            break;
        case 78:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 12, 9);
                tilemap_map_settile(0, 1, x, 13, 13);
                tilemap_map_settile(0, 1, x, 14, 13);
                tilemap_map_settile_ex(0, 1, x, 15, 9, MIRROR_VERTICAL);
            }
            break;
        case 79:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 12, 10);
                tilemap_map_settile_ex(0, 1, x, 15, 10, MIRROR_VERTICAL);
            }
            break;
        case 80:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 12, 11);
                tilemap_map_settile_ex(0, 1, x, 15, 11, MIRROR_VERTICAL);
            }
            break;
        case 81:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 11, 9);
                tilemap_map_settile(0, 1, x, 12, 13);
                tilemap_map_settile(0, 1, x, 15, 13);
                tilemap_map_settile_ex(0, 1, x, 16, 9, MIRROR_VERTICAL);
            }
            break;
        case 82:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 11, 10);
                tilemap_map_settile_ex(0, 1, x, 16, 10, MIRROR_VERTICAL);
            }
            break;
        case 83:
            for(int x = 0; x < 32; x++)
            {
                tilemap_map_settile(0, 1, x, 11, 11);
                tilemap_map_settile_ex(0, 1, x, 16, 11, MIRROR_VERTICAL);
            }
            break;
    }

    if(kit_stage_time >= 128 && (kit_stage_time - 128) < 32)
    {
        palette_setscreencolours(0, 1, &palette_game[32 + ((kit_stage_time - 128) >> 2)]);
    }

}
