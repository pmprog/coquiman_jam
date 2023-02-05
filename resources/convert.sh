#!/bin/bash

mono embedconv2.exe -in game_palette.gpl -outc res_palette.c -outh res_palette.h -name palette_game -struct -structdef "kit.h"

mono embedconv2.exe -in tilesheet.png -outc res_tiles.c -outh res_tiles.h -palette game_palette.gpl -tile 8 8 -sprite 1 1 -8bpp -structdef "kit.h"
mono embedconv2.exe -in spritesheet.png -outc res_sprites.c -outh res_sprites.h -palette game_palette.gpl -tile 8 8 -sprite 2 2 -8bpp -structdef "kit.h"

mono embedconv2.exe -in wip/header.tmx -outc res_ui_header.c -outh res_ui_header.h -name ui_header
mono embedconv2.exe -in wip/level_1.tmx -outc res_level_1.c -outh res_level_1.h -name level_1