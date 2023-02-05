
#include "../include/audio.h"
#include "gba.h"

u8 music_playing = 0;
u8 music_volume = 255;

void music_play(u16 TrackIndex)
{
    mmStart( TrackIndex, MM_PLAY_LOOP );
    music_playing = 1;
}

void music_stop()
{
    mmStop();
    //music_playing = 0;
}

u8 music_volume_get()
{
    return music_volume;
}

void music_volume_set(u8 Volume)
{
    music_volume = Volume;
    mmSetModuleVolume( music_volume << 2 );
}

void sfx_play(u16 EffectIndex, u8 Volume)
{

}
