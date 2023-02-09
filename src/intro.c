
#include "intro.h"
#include <kit.h>

static const Colour colour_black = { 0, 0, 0 };

StageRecord introstage = { introstage_start, introstage_update, introstage_finish };

void introstage_start()
{
    palette_setscreencolours(0, 1, &colour_black);
    palette_setspritecolours(0, 1, &colour_black);
}

void introstage_finish()
{

}

void introstage_update()
{

}
