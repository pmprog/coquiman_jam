
#include "../include/scheduler.h"
#include "../include/kit.h"
#include <stdlib.h>

Scheduler* scheduler_create(u16 MaximumSize)
{
    Scheduler* sched = (Scheduler*)calloc( sizeof(Scheduler), 1 );
    sched->Tasks = (TaskRecord*)calloc( sizeof(TaskRecord), MaximumSize );
    sched->MaximumTasks = MaximumSize;
    return sched;
}

void scheduler_clear(Scheduler* List)
{
    for(u16 i = 0; i < List->MaximumTasks; i++)
    {
        List->Tasks[i].Callback = nullptr;
    }
}

u16 scheduler_add(Scheduler* List, TaskCallback Callback, u16 Interval)
{
    return scheduler_add_ex(List, Callback, Interval, 0xFFFF, nullptr);
}

u16 scheduler_add_ex(Scheduler* List, TaskCallback Callback, u16 Interval, u16 Repeats, void* CallbackData)
{
    for(u16 i = 0; i < List->MaximumTasks; i++)
    {
        if( List->Tasks[i].Callback == nullptr )
        {
            List->Tasks[i].TaskInterval = Interval;
            List->Tasks[i].TicksUntilTrigger = Interval;
            List->Tasks[i].NumberOfTriggers = Repeats;
            List->Tasks[i].Callback = Callback;
            List->Tasks[i].CallbackData = CallbackData;
            return i;
        }
    }
    return 0xFFFF;
}

void scheduler_remove(Scheduler* List, u16 Index)
{
    List->Tasks[Index].Callback = nullptr;
}

void scheduler_update(Scheduler* List)
{
    for(u16 i = 0; i < List->MaximumTasks; i++)
    {
        if( List->Tasks[i].Callback != nullptr )
        {
            // Decrease time till trigger
            List->Tasks[i].TicksUntilTrigger--;
            if(List->Tasks[i].TicksUntilTrigger == 0)
            {
                // We're triggering now
                List->Tasks[i].Callback(List->Tasks[i].CallbackData);
                // If we're on limited triggers...
                if(List->Tasks[i].NumberOfTriggers != 0xFFFF)
                {
                    // Check if we've expired
                    List->Tasks[i].NumberOfTriggers--;
                    if(List->Tasks[i].NumberOfTriggers == 0)
                    {
                        scheduler_remove(List, i);
                    }
                }
            }
        }
    }
}
