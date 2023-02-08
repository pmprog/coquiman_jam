@echo off

embedconv2.exe -in pal_game.gpl -outc res_palette_game.c -outh res_palette_game.h -name palette_game -struct -structdef "kit.h"

embedconv2.exe -in gfx_titlebkg_tiled.png -outc res_graphics_title.c -outh res_graphics_title.h -palette pal_game.gpl -tile 8 8 -sprite 1 1 -8bpp -structdef "kit.h"
embedconv2.exe -in gfx_level.png -outc res_graphics_level.c -outh res_graphics_level.h -palette pal_game.gpl -tile 8 8 -sprite 1 1 -8bpp -structdef "kit.h"

embedconv2.exe -in gfx_titlebkg_tiled.tmx -outc res_map_title.c -outh res_map_title.h -name map_title
embedconv2.exe -in map_level.tmx -outc res_map_level.c -outh res_map_level.h -name map_level
