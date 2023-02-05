
#include "../include/display.h"
#include "../include/kit.h"
#include "allegro.h"

u16 display_cnt = 0;
ALLEGRO_DISPLAY* display_obj[MAX_DISPLAYS];
ALLEGRO_BITMAP* display_canvas[MAX_DISPLAYS];
ALLEGRO_LOCKED_REGION* display_region[MAX_DISPLAYS];
int display_scale[MAX_DISPLAYS];

DisplayMode display_mode[MAX_DISPLAYS];
ALLEGRO_LOCKED_REGION* display_bufferlock[MAX_DISPLAYS];

void display_mode_clean(u8 DisplayIndex);

u8 display_count()
{
    return display_cnt;
}

u8 display_new(u8 Monitor, u16 Width, u16 Height, DisplayFlags Flags)
{
    // Find the first empty display slot
    u8 targetdisplay = 255;
    for(u8 d = 0; d < MAX_DISPLAYS; d++)
    {
        if(display_obj[d] == null)
        {
            targetdisplay = d;
            break;
        }
    }
    if(targetdisplay == 255)
    {
        return targetdisplay;
    }

    ALLEGRO_MONITOR_INFO moninfo;
    int monitorw;
    int monitorh;
    if (!al_get_monitor_info(Monitor, &moninfo))
    {
        // Monitor is invalid, so use primary monitor
        al_get_monitor_info(0, &moninfo);
    }
    monitorw = moninfo.x2 - moninfo.x1;
    monitorh = moninfo.y2 - moninfo.y1;

    if((Flags & DISPLAY_FULLSCREEN) == DISPLAY_FULLSCREEN)
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    }
    else
    {
        monitorw = (int)(monitorw * 0.90f);
        monitorh = (int)(monitorh * 0.85f);
        if((Flags & DISPLAY_RESIZABLE) == DISPLAY_RESIZABLE)
        {
            al_set_new_display_flags(ALLEGRO_RESIZABLE | ((Flags & DISPLAY_MAXIMIZED) == DISPLAY_MAXIMIZED ? ALLEGRO_MAXIMIZED : 0));
        }
        else
        {
            al_set_new_display_flags(ALLEGRO_WINDOWED);
        }
    }

    display_scale[targetdisplay] = 1;
    while (Width * display_scale[targetdisplay] < monitorw && Height * display_scale[targetdisplay] < monitorh)
    {
        display_scale[targetdisplay]++;
    }

    display_scale[targetdisplay]--;
    float video_width = Width * display_scale[targetdisplay];
    float video_height = Height * display_scale[targetdisplay];

    al_set_new_window_position((monitorw - video_width) / 2, (monitorh - video_height) / 2);

    display_obj[targetdisplay] = al_create_display((int)video_width, (int)video_height);
    al_register_event_source(event_queue, al_get_display_event_source(display_obj[targetdisplay]));
    display_canvas[targetdisplay] = al_create_bitmap(Width, Height);
    al_set_target_bitmap(display_canvas[targetdisplay]);
    al_clear_to_color(palette_screen_a5[0]);

    display_mode[targetdisplay] = MODE_NONE;

    display_cnt++;
    return targetdisplay;
}

void display_close(u8 DisplayIndex)
{
    if(display_obj[DisplayIndex] == null)
    {
        return;
    }
    display_mode_clean(DisplayIndex);
    al_unregister_event_source(event_queue, al_get_display_event_source(display_obj[DisplayIndex]));
    al_destroy_display(display_obj[DisplayIndex]);
    display_obj[DisplayIndex] = null;
    al_destroy_bitmap(display_canvas[DisplayIndex]);
    display_canvas[DisplayIndex] = null;
    display_cnt--;
}

void display_flip(u8 DisplayIndex)
{

    switch(display_mode[DisplayIndex])
    {
        case MODE_BITMAP:
            // TODO: Handle sprites in bitmap mode!
            al_unlock_bitmap(display_canvas[DisplayIndex]);
            break;
        case MODE_TILEMAP:
            al_set_target_bitmap(display_canvas[DisplayIndex]);
            al_clear_to_color(palette_screen_a5[0]);

            for(u8 plane = 0; plane < 4; plane++)
            {
                if(tilemap_display[DisplayIndex].Planes[plane].Visible != 0)
                {
                    tilemap_map_refreshcache(DisplayIndex, plane);
                    al_draw_bitmap(tilemap_display[DisplayIndex].Planes[plane].Canvas, 0 - tilemap_display[DisplayIndex].Planes[plane].ScrollX, 0 - tilemap_display[DisplayIndex].Planes[plane].ScrollY, 0);
                    al_draw_bitmap(tilemap_display[DisplayIndex].Planes[plane].Canvas, 0 - tilemap_display[DisplayIndex].Planes[plane].ScrollX + 256, 0 - tilemap_display[DisplayIndex].Planes[plane].ScrollY, 0);
                    al_draw_bitmap(tilemap_display[DisplayIndex].Planes[plane].Canvas, 0 - tilemap_display[DisplayIndex].Planes[plane].ScrollX, 0 - tilemap_display[DisplayIndex].Planes[plane].ScrollY + 256, 0);
                    al_draw_bitmap(tilemap_display[DisplayIndex].Planes[plane].Canvas, 0 - tilemap_display[DisplayIndex].Planes[plane].ScrollX + 256, 0 - tilemap_display[DisplayIndex].Planes[plane].ScrollY + 256, 0);
                }

                for(int s = 127; s >= 0; s--)
                {
                    if( sprites[s].graphic_visible != 0 && sprites[s].cached_image != NULL && sprites[s].position_plane == plane )
                    {
                        al_draw_bitmap(sprites[s].cached_image, sprites[s].position_x, sprites[s].position_y, 0);
                    }
                }
            }
            break;
        default:
            break;
    }

    al_set_target_backbuffer(display_obj[DisplayIndex]);
    al_clear_to_color(palette_screen_a5[0]);
    // Draw canvas scaled, and in the middle
    int dispw = al_get_bitmap_width(display_canvas[DisplayIndex]);
    int disph = al_get_bitmap_height(display_canvas[DisplayIndex]);
    al_draw_scaled_bitmap(display_canvas[DisplayIndex], 0, 0, dispw, disph, 0, 0, dispw * display_scale[DisplayIndex], disph * display_scale[DisplayIndex], 0);
    al_flip_display();

    switch(display_mode[DisplayIndex])
    {
        case MODE_BITMAP:
            display_bufferlock[DisplayIndex] = al_lock_bitmap(display_canvas[DisplayIndex], ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_READWRITE);
            break;
        case MODE_TILEMAP:
            break;
        default:
            break;
    }
}

void display_mode_clean(u8 DisplayIndex)
{

    switch(display_mode[DisplayIndex])
    {
        case MODE_BITMAP:
            al_unlock_bitmap(display_canvas[DisplayIndex]);
            display_bufferlock[DisplayIndex] = nullptr;
            break;
        case MODE_TILEMAP:
            for(u8 plane = 0; plane < 4; plane++)
            {
                if(tilemap_display[DisplayIndex].Planes[plane].Canvas != nullptr)
                {
                    al_destroy_bitmap(tilemap_display[DisplayIndex].Planes[plane].Canvas);
                    tilemap_display[DisplayIndex].Planes[plane].Canvas = nullptr;
                }
                tilemap_display[DisplayIndex].Planes[plane].Visible = 0;
            }
            break;
        default:
            break;
    }
    display_mode[DisplayIndex] = MODE_NONE;
}

void display_mode_bitmap(u8 DisplayIndex, u8 DoubleBuffer)
{
    display_mode_clean(DisplayIndex);

    display_mode[DisplayIndex] = MODE_BITMAP;
    display_bufferlock[DisplayIndex] = al_lock_bitmap(display_canvas[DisplayIndex], ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_READWRITE);
}

void display_mode_tilemap(u8 DisplayIndex, u8 NumberOfPlanes, u8 BitsPerPixel)
{
    display_mode_clean(DisplayIndex);

    display_mode[DisplayIndex] = MODE_TILEMAP;

    for(u8 plane = 0; plane < NumberOfPlanes; plane++)
    {
        tilemap_display[DisplayIndex].Planes[plane].Canvas = al_create_bitmap(256, 256);
        al_set_target_bitmap(tilemap_display[DisplayIndex].Planes[plane].Canvas);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        tilemap_display[DisplayIndex].Planes[plane].BitsPerPixel = BitsPerPixel;
        tilemap_display[DisplayIndex].Planes[plane].Visible = 1;
    }
}
