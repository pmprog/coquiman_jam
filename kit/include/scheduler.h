
#pragma once

#include "typedefs.h"

typedef void(*TaskCallback)(void* Data);

typedef struct TaskRecord
{
    u16             TaskInterval;               // Number of ticks between firing
    u16             TicksUntilTrigger;          // Current countdown to trigger
    u16             NumberOfTriggers;           // Schedule cancels after this number of triggers - set to 0xFFFF for "forever"
    TaskCallback    Callback;                   // Called when the task is triggered
    void*           CallbackData;               // Data pointer to be sent to the callback
} TaskRecord;

typedef struct Scheduler
{
    u16             MaximumTasks;               // Number of supported tasks
    TaskRecord*     Tasks;                      // Pointer to tasks list
} Scheduler;

// Allocate memory for a new scheduler setup
Scheduler* scheduler_create(u16 MaximumSize);

// Clears a schedule list, or can be used for statically positioned scheduler (ie, GBA)
void scheduler_clear(Scheduler* List);

// Add task to the scheduler, returns index in scheduler, returns 0xFFFF if no space
u16  scheduler_add(Scheduler* List, TaskCallback Callback, u16 Interval);

// Extended add task with all features
u16  scheduler_add_ex(Scheduler* List, TaskCallback Callback, u16 Interval, u16 Repeats, void* CallbackData);

// Remove a schedule by it's index
void scheduler_remove(Scheduler* List, u16 Index);

// Process tasks in a scheduler
void scheduler_update(Scheduler* List);
