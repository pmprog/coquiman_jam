#!/bin/bash

mono embedconv2.exe -in pal_game.gpl -outc res_palette_game.c -outh res_palette_game.h -name palette_game -struct -structdef "kit.h"

mono embedconv2.exe -in gfx_level.png -outc res_graphics_level.c -outh res_graphics_level.h -palette pal_game.gpl -tile 8 8 -sprite 1 1 -8bpp -structdef "kit.h"

mono embedconv2.exe -in map_level.tmx -outc res_map_level.c -outh res_map_level.h -name map_level
