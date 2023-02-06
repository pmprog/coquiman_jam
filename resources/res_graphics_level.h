
#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define gfx_level_len 65536
#define gfx_level_width 256
#define gfx_level_height 256
#define gfx_level_tile_width 8
#define gfx_level_tile_height 8
#define gfx_level_tile_count 1024
extern const uint8_t gfx_level[];

#ifdef __cplusplus
}
#endif
