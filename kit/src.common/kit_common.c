
#include "../include/kit.h"

void kit_change_stage_internal(QueueEventType Type, void* Next);

StageRecord* kit_stage_active;
u16 kit_stage_time;
Queue* kit_internal_queue;       // Internal queue for system events

void kit_change_stage(StageRecord* Next)
{
    // Use queue to change event so it's not shutdown mid-process
    queue_add(kit_internal_queue, EVENT_INTERNAL, kit_change_stage_internal, (void*)Next);
}

void kit_change_stage_internal(QueueEventType Type, void* Next)
{
    if(kit_stage_active != NULL)
    {
        kit_stage_active->Finish();
    }
    kit_stage_time = 0;
    kit_stage_active = (StageRecord*)Next;
    if(kit_stage_active != NULL)
    {
        kit_stage_active->Start();
    }
}

void kit_execute(StageRecord* StartingStage)
{
    u8 shutdown = 0;

    sprites_clear();
    kit_change_stage_internal(EVENT_INTERNAL, (void*)StartingStage);

    while( shutdown == 0 )
    {
        // Allow the system to update
        kit_system_update();

        // All audio data to process
        //audio_update();

        // Process internal queue
        while(queue_process_record(kit_internal_queue) != 0)
        {
            // we're due to quit at any point, leave the loop to shutdown
            if(kit_stage_active == NULL)
            {
                shutdown = 1;
                break;
            }
        }

        if(kit_stage_active == NULL)
        {
            shutdown = 1;
        }
        else
        {
            // Get number of frames we need to process
            u16 updatestoprocess = kit_system_ticks();
            while(updatestoprocess > 0)
            {
                // Update network state
                //network_update();
                // Update controller state(s)
                input_update();

                // Call the game stage to update
                kit_stage_active->Update();
                updatestoprocess--;
                kit_stage_time++;
            }
        }
        // Update the displays
        for(u8 d = 0; d < display_count(); d++)
        {
            display_flip(d);
        }
    }
}
