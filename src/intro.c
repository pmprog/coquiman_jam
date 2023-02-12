
#include "intro.h"
#include <kit.h>

#include "../resources/res_graphics_title.h"
#include "../resources/res_graphics_titlef.h"
#include "../resources/res_graphics_titles.h"
#include "../resources/res_map_titlef.h"

static const Colour colour_black = { 0, 0, 0 };

StageRecord introstage = { introstage_start, introstage_update, introstage_finish };

#define animation_0_frame_count     48
static const u8 animation_0_frames[] = {
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8,
    9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14
};

#define animation_1_frame_count     8
static const u8 animation_1_frames[] = { 22, 22, 22, 22, 22, 22, 22, 22 };

#define animation_2_frame_count     7
static const u8 animation_2_frames[] = { 15, 16, 17, 18, 19, 20, 21 };

#define animation_3_frame_count     56
static const u8 animation_3_frames[] = { 
    23, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26,
    27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 29,
    30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 32, 32, 32, 32, 33, 33,
    34, 34, 35, 35, 35, 35, 36, 36, 37, 37,
    };

#define animation_play_count     24
static const u8 animation_play_order[] = {
    1, 1, 1, 1, 1, 2,
    1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 2,
    1, 1, 1, 1, 1, 3,
};

static u8 introstage_animation_play = 0;
static u8 introstage_animation = 0;
static u16 introstage_animation_time = 0;
static s16 introstage_text1y = -20;
static s16 introstage_text2y = 170;

void introstage_renderanimframe(u8 framenumber);
void introstage_playnextanim();

void introstage_start()
{
    palette_setscreencolours(0, 1, &colour_black);
    palette_setspritecolours(0, 1, &colour_black);

    tilemap_characterset_set(0, gfx_titlefgn_tiled, gfx_titlebkg_tiled_tile_count << 1, gfx_titlefgn_tiled_tile_count, 8);  // Load up the map characters
    sprites_characterset_set(gfx_titlesprites, 0, gfx_titlesprites_tile_count, 8);

    introstage_animation = 0;
    introstage_animation_time = 0;
    introstage_renderanimframe(0);
}

void introstage_finish()
{

}

void introstage_update()
{
    if( kit_stage_time % 2 != 0 ) {return;}

    switch(introstage_animation)
    {
        case 0:
            introstage_renderanimframe( animation_0_frames[introstage_animation_time] );
            introstage_animation_time++;
            if(introstage_animation_time >= animation_0_frame_count)
            { 
                introstage_animation_play = animation_play_count - 1;
                introstage_playnextanim();
            }

            introstage_text1y = -90;
            introstage_text2y = 170;

            for(int si = 0; si < 24; si++)
            {
                sprites_configure( si, si << 3, 8 , 0, SPRITE_SIZE_32x8 );


                if( si < 8 )
                {
                    sprites_position(si, 3, 1, 15 + ((si % 4) * 32), introstage_text1y + ((si & 0x04) << 1), SPRITE_MIRROR_NONE);
                }
                else
                {
                    sprites_position(si, 3, 1, 112 + ((si % 4) * 32), introstage_text2y + (((si-8) & 0x0C) << 1), SPRITE_MIRROR_NONE);
                }
            }

            break;

        case 1:
            introstage_renderanimframe( animation_1_frames[introstage_animation_time] );
            introstage_animation_time++;
            if(introstage_animation_time >= animation_1_frame_count) { introstage_playnextanim(); }
            break;

        case 2:
            introstage_renderanimframe( animation_2_frames[introstage_animation_time] );
            introstage_animation_time++;
            if(introstage_animation_time >= animation_2_frame_count) { introstage_playnextanim(); }
            break;

        case 3:
            introstage_renderanimframe( animation_3_frames[introstage_animation_time] );
            introstage_animation_time++;
            if(introstage_animation_time >= animation_3_frame_count) { introstage_playnextanim(); }
            break;
    }

    if(introstage_animation != 0)
    {
        if( introstage_text1y < 2 )
        {
            introstage_text1y++;
            for(int si = 0; si < 8; si++)
            {
                
                sprites_position(si, 3, 1, 15 + ((si % 4) * 32), introstage_text1y + ((si & 0x04) << 1), SPRITE_MIRROR_NONE);
            }
        }
        if( introstage_text2y > 124 )
        {
            introstage_text2y--;
            for(int si = 8; si < 24; si++)
            {
                
                sprites_position(si, 3, 1, 112 + ((si % 4) * 32), introstage_text2y + (((si-8) & 0x0C) << 1), SPRITE_MIRROR_NONE);
            }
        }

    }

}

void introstage_playnextanim()
{
    introstage_animation_play = (introstage_animation_play + 1) % animation_play_count;
    introstage_animation = animation_play_order[introstage_animation_play];
    introstage_animation_time = 0;
}

void introstage_renderanimframe(u8 framenumber)
{
    //map_titlef_gfx_titlefgn_map_width

    u16 basey = (framenumber * 20);
    u16 basec = gfx_titlebkg_tiled_tile_count; //(gfx_titlebkg_tiled_tile_count << 1);
    for(u16 y = 0; y < 20; y++)
    {
        for(u8 x = 0; x < 30; x++)
        {
            u16 tc = map_titlef_gfx_titlefgn_map_data[ ((basey + y) * map_titlef_gfx_titlefgn_map_width) + x ];
            tilemap_map_settile( 0, 1, x, y, basec + tc );
        }
    }

}
