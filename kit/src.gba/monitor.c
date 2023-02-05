
#include "../include/monitor.h"
#include "../include/kit.h"

u8 monitor_count()
{
    return 1;
}

u16 monitor_width(u8 MonitorIndex)
{
    return (MonitorIndex == 0 ? 240 : 0);
}

u16 monitor_height(u8 MonitorIndex)
{
    return (MonitorIndex == 0 ? 160 : 0);
}
