
#pragma once

#include "typedefs.h"
#include "bitmap.h"
#include "tilemap.h"

typedef enum DisplayFlags
{
    DISPLAY_NONE        = 0x0000,
    DISPLAY_FULLSCREEN  = 0x0001,
    DISPLAY_FIXEDWINDOW = 0x0002,
    DISPLAY_RESIZABLE   = 0x0004,
    DISPLAY_MAXIMIZED   = 0x0008,   // Only applies when DISPLAY_RESIZABLE is set
} DisplayFlags;

// Get the number of displays
u8   display_count();

// Create a new display (window)
u8   display_new(u8 Monitor, u16 Width, u16 Height, DisplayFlags Flags);

// Close the display
void display_close(u8 DisplayIndex);

// Update all displays by switching buffer
void display_flip(u8 DisplayIndex);

// Set the display to support bitmap access
void display_mode_bitmap(u8 DisplayIndex, u8 DoubleBuffer);

// Set the display to use tilemaps
void display_mode_tilemap(u8 DisplayIndex, u8 NumberOfPlanes, u8 BitsPerPixel);
