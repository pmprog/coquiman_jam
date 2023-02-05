
#include "../include/monitor.h"
#include "../include/kit.h"
#include "allegro.h"

u8 monitor_count()
{
    return al_get_num_video_adapters();
}

u16 monitor_width(u8 MonitorIndex)
{
    ALLEGRO_MONITOR_INFO mi;
    al_get_monitor_info(MonitorIndex, &mi);
    return (mi.x2 - mi.x1);
}

u16 monitor_height(u8 MonitorIndex)
{
    ALLEGRO_MONITOR_INFO mi;
    al_get_monitor_info(MonitorIndex, &mi);
    return (mi.y2 - mi.y1);
}
