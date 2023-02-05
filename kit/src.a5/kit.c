
#include "../include/kit.h"
#include "allegro.h"
#include <stdio.h>

ALLEGRO_VOICE* audio_voice;
ALLEGRO_MIXER* audio_mixer;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_TIMER* update_timer;
ALLEGRO_MUTEX* update_mutex;
u16 update_required = 0;


void kit_init(Queue* KitQueue)
{
    kit_internal_queue = KitQueue;
    if(kit_internal_queue == nullptr)
    {
        kit_internal_queue = queue_create(20);
    }

    al_init();
    al_install_keyboard();
    al_install_joystick();
    al_install_audio();
    al_init_acodec_addon();
    al_init_primitives_addon();
    al_init_image_addon();

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_joystick_event_source());

    al_reserve_samples(20);
    audio_voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    if (audio_voice != NULL)
    {
        audio_mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
        if(audio_mixer == NULL)
        {
            al_destroy_voice(audio_voice);
            audio_voice = NULL;
        }
        else
        {
            if (!al_attach_mixer_to_voice(audio_mixer, audio_voice))
            {
                al_destroy_mixer(audio_mixer);
                audio_mixer = NULL;
                al_destroy_voice(audio_voice);
                audio_voice = NULL;
            }
        }
    }

    update_mutex = al_create_mutex();

    update_timer = al_create_timer(1.0 / 60.0);
    al_register_event_source( event_queue, al_get_timer_event_source(update_timer));
    al_start_timer(update_timer);

    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

}

void kit_shutdown()
{
    al_stop_timer(update_timer);
    al_unregister_event_source(event_queue, al_get_timer_event_source(update_timer));
    al_destroy_timer(update_timer);
    al_destroy_mutex(update_mutex);

    if (audio_voice != NULL)
    {
        music_stop();
        al_detach_voice(audio_voice);
        al_destroy_mixer(audio_mixer);
        audio_mixer = NULL;
        al_destroy_voice(audio_voice);
        audio_voice = NULL;
    }

    al_unregister_event_source(event_queue, al_get_joystick_event_source());
    al_unregister_event_source(event_queue, al_get_keyboard_event_source());
    al_destroy_event_queue(event_queue);

    al_uninstall_audio();
    al_uninstall_joystick();
    al_uninstall_keyboard();
    al_uninstall_system();
}

void kit_log_message(char* Message)
{
	printf(Message);
	printf("\n");
}

void kit_system_update()
{
    ALLEGRO_EVENT ae;
    while (al_get_next_event(event_queue, &ae))
    {
        switch (ae.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                for(int i = 0; i < MAX_DISPLAYS; i++)
                {
                    if(display_obj[i] == ae.display.source)
                    {
                        display_close(i);
                        break;
                    }
                }
                if(display_count() == 0)
                {
                    kit_change_stage(nullptr);
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                if( ae.timer.source == update_timer )
                {
                    al_lock_mutex(update_mutex);
                    update_required++;
                    al_unlock_mutex(update_mutex);
                }
                break;
            case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
                al_reconfigure_joysticks();
                break;
            default:
                break;
        }
    }
}

u16 kit_system_ticks()
{
    u16 ticks = 0;
    al_lock_mutex(update_mutex);
    ticks = update_required;
    update_required = 0;
    al_unlock_mutex(update_mutex);
    return ticks;
}
