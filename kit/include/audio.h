
#pragma once

#include "typedefs.h"

extern u8 music_playing;

void music_play(u16 TrackIndex);
void music_stop();
u8 music_volume_get();
void music_volume_set(u8 Volume);

void sfx_play(u16 EffectIndex, u8 Volume);
