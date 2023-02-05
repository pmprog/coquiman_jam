
#include "../include/audio.h"
#include "allegro.h"
#include <stdio.h>


ALLEGRO_AUDIO_STREAM* audio_stream = nullptr;
u8 music_volume = 255;

static const int music_extensions_count = 3;
static const char* music_extensions[] = {
    "mod",
    "xm",
    "it",
};

static const int music_paths_count = 4;
static const char* music_paths[] = {
    "",
    "music/",
    "resource/",
    "resources/",
};


void music_play(u16 TrackIndex)
{
    if (audio_voice == nullptr) { return; }
    music_stop();

    char filename[200];

    for(int p = 0; p < music_paths_count; p++)
    {
        for(int x = 0; x < music_extensions_count; x++)
        {
            sprintf((char*)&filename, "%strack_%d.%s", music_paths[p], TrackIndex, music_extensions[x]);
            audio_stream = al_load_audio_stream((char*)&filename, 4, 2048);
            if (audio_stream != nullptr) { break; }
        }
        if (audio_stream != nullptr) { break; }
    }

    if (audio_stream == nullptr)
    {
        kit_log_message("Could not load audio track");
        return;
    }
    al_attach_audio_stream_to_mixer(audio_stream, audio_mixer);
    al_set_audio_stream_gain(audio_stream, (music_volume / 255.0f));
    al_set_audio_stream_playmode(audio_stream, ALLEGRO_PLAYMODE_LOOP);
}

void music_stop()
{
    if (audio_voice == nullptr) { return; }
    if (audio_stream == nullptr) { return; }
    al_set_audio_stream_playing(audio_stream, false);
    al_detach_audio_stream(audio_stream);
    al_destroy_audio_stream(audio_stream);
    audio_stream = nullptr;
}

u8 music_volume_get()
{
    return music_volume;
}

void music_volume_set(u8 Volume)
{
    music_volume = Volume;
    if(audio_stream != nullptr)
    {
        al_set_audio_stream_gain(audio_stream, (music_volume / 255.0f));
    }
}

void sfx_play(u16 EffectIndex, u8 Volume)
{

}
