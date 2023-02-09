
#include "intro.h"
#include <kit.h>

#include "../resources/res_graphics_titlef.h"
#include "../resources/res_map_titlef.h"
#include "../resources/res_graphics_title.h"

static const Colour colour_black = { 0, 0, 0 };

StageRecord introstage = { introstage_start, introstage_update, introstage_finish };

void introstage_start()
{
    palette_setscreencolours(0, 1, &colour_black);
    palette_setspritecolours(0, 1, &colour_black);

    tilemap_characterset_set(0, gfx_titlefgn_tiled, gfx_titlebkg_tiled_tile_count << 1, gfx_titlefgn_tiled_tile_count, 8);  // Load up the map characters
}

void introstage_finish()
{

}

void introstage_update()
{

}
