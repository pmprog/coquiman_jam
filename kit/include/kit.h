
#pragma once

#include "typedefs.h"
#include "display.h"
#include "input.h"
#include "monitor.h"
#include "multiplayer.h"
#include "palette.h"
#include "queue.h"
#include "scheduler.h"
#include "sprites.h"
#include "stage.h"
#include "tilemap.h"
#include "audio.h"
#include "maths.h"

extern StageRecord* kit_stage_active;   // Active stage
extern u16 kit_stage_time;              // Stage execution time, 16 bit counter on how many frames the stage has been running
extern Queue* kit_internal_queue;       // Internal queue for system events

// Configure the kit for running on the platform
void kit_init(Queue* KitQueue);

// Shutdown and deallocate the kit
void kit_shutdown();

// Start the kit program loop, running with a particular stage
void kit_execute(StageRecord* StartingStage);

// Set a new stage
void kit_change_stage(StageRecord* Next);

// Log a message out
void kit_log_message(char* Message);

// Process platform/system events
void kit_system_update();

// Get the number of ticks since the last call (runs at 60FPS)
u16  kit_system_ticks();
