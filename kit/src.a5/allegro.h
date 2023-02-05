
#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../include/typedefs.h"
#include "../include/sprites.h"

#define MAX_DISPLAYS    16

#define IWRAM_CODE
#define IWRAM_DATA
#define IWRAM_BSS
#define EWRAM_CODE
#define EWRAM_DATA

#ifndef min
#define min(a,b)    (a <= b ? a : b)
#endif
#ifndef max
#define max(a,b)    (a >= b ? a : b)
#endif


typedef enum DisplayMode
{
    MODE_NONE           = 0x0000,
    MODE_BITMAP         = 0x0001,
    MODE_TILEMAP        = 0x0002,
} DisplayMode;

typedef struct TilePlane
{
    ALLEGRO_BITMAP* Canvas;
    u16 TileIndex[32 * 32];
    u8 ScrollX;
    u8 ScrollY;
    u8 BitsPerPixel;
    u8 Visible;
} TilePlane;

typedef struct TileDisplay
{
    TilePlane Planes[4];
    u8 CharacterData[0x20000];
    u8 CharacterDirty[1024];
} TileDisplay;

typedef struct SpriteData
{
    ALLEGRO_BITMAP* cached_image;
    s16             position_x;
    s16             position_y;
    u8              position_plane;
    SpriteSize      graphic_size;
    u8              graphic_bpp;
    u16             graphic_tile;
    u8              graphic_palette;
    u8              graphic_visible;
    SpriteMirror    graphic_flip;
} SpriteData;

extern ALLEGRO_VOICE* audio_voice;
extern ALLEGRO_MIXER* audio_mixer;
extern ALLEGRO_EVENT_QUEUE* event_queue;

extern Colour palette_screen[256];
extern Colour palette_sprites[256];
extern ALLEGRO_COLOR palette_screen_a5[256];
extern ALLEGRO_COLOR palette_sprites_a5[256];

extern ALLEGRO_DISPLAY* display_obj[MAX_DISPLAYS];
extern ALLEGRO_BITMAP* display_canvas[MAX_DISPLAYS];
extern ALLEGRO_LOCKED_REGION* display_region[MAX_DISPLAYS];
extern int display_scale[MAX_DISPLAYS];
extern DisplayMode display_mode[MAX_DISPLAYS];
extern ALLEGRO_LOCKED_REGION* display_bufferlock[MAX_DISPLAYS];

extern TileDisplay tilemap_display[MAX_DISPLAYS];
extern SpriteData sprites[128];

void tilemap_map_refreshcache(u8 DisplayIndex, u8 Plane);
void sprites_refreshcache();