
#pragma once

#include "typedefs.h"

typedef enum QueueEventType
{
    EVENT_INTERNAL,                 // Internal messages
    EVENT_DISPLAY_OPENED,           // New display created
    EVENT_DISPLAY_RESIZED,          // Display was resized
    EVENT_DISPLAY_ACTIVATED,        // Display was activated
    EVENT_DISPLAY_DEACTIVATED,      // Display lost focus to another display
    EVENT_DISPLAY_CLOSED,           // Display was closed
    EVENT_KEYBOARD_KEY_DOWN,
    EVENT_KEYBOARD_KEY_UP,
    EVENT_KEYBOARD_KEY_PRESS,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_BUTTON_DOWN,
    EVENT_MOUSE_BUTTON_UP,
    EVENT_NETWORK_CONNECT_REQUEST,
    EVENT_NETWORK_CONNECTED,
    EVENT_NETWORK_RECEIVED,
    EVENT_NETWORK_DISCONNECT,
    EVENT_AUDIO_TRACK_STARTED,
    EVENT_AUDIO_TRACK_STOPPED,
    EVENT_TIMER_TICK,
    EVENT_CUSTOM,
} QueueEventType;

typedef void (*QueueCallback)(QueueEventType Type, void* Data);

typedef struct QueueRecord
{
    QueueEventType  EventType;                  // Event type, passed into callback
    QueueCallback   Callback;                   // Called when the task is triggered
    void*           CallbackData;               // Data pointer to be sent to the callback
} QueueRecord;

typedef struct Queue
{
    u16             MaximumRecords;
    u16             Top;
    u16             Bottom;
    QueueRecord*    Records;                    // Pointer to tasks list
} Queue;

// Allocate memory for a new scheduler setup
Queue* queue_create(u16 MaximumSize);

// Clears a schedule list, or can be used for statically positioned scheduler (ie, GBA)
void queue_clear(Queue* List);

// Add task to the scheduler, returns index in scheduler
u16  queue_add(Queue* List, QueueEventType EventType, QueueCallback Callback, void* CallbackData);

// Processes the top item in the list, returns number of items left in the queue
u16  queue_process_record(Queue* List);
