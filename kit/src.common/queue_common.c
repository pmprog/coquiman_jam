
#include "../include/queue.h"
#include "../include/kit.h"
#include <stdlib.h>

Queue* queue_create(u16 MaximumSize)
{
    // Allocate space for the queue header
    Queue* que = (Queue*)calloc( sizeof(Queue), 1 );
    // Allocate space for the queue records
    que->Records = (QueueRecord*)calloc( sizeof(QueueRecord), MaximumSize );
    // Set the header information
    que->MaximumRecords = MaximumSize;
    que->Top = 0;
    que->Bottom = 0;
    return que;
}

void queue_clear(Queue* List)
{
    // Just set Top and Bottom to clear the list
    List->Top = 0;
    List->Bottom = 0;
}

u16 queue_add(Queue* List, QueueEventType EventType, QueueCallback Callback, void* CallbackData)
{
    // Calculate the position of the next item
    u16 newbottom = (List->Bottom + 1) % List->MaximumRecords;

    // If we've looped back round to the top, the queue is full
    if(newbottom == List->Top)
    {
        kit_log_message("Error: Cannot add to queue");
        return 0;
    }

    // Insert the queue data
    List->Records[List->Bottom].EventType = EventType;
    List->Records[List->Bottom].Callback = Callback;
    List->Records[List->Bottom].CallbackData = CallbackData;

    // Set the new bottom
    List->Bottom = newbottom;
    return 1;
}


u16 queue_process_record(Queue* List)
{
    // Nothing in the queue
    if( List->Top == List->Bottom )
    {
        return 0;
    }
    // Perform the callback with the event type and data
    List->Records[List->Top].Callback(List->Records[List->Top].EventType, List->Records[List->Top].CallbackData);
    // Move the top of the queue to the next item
    List->Top = (List->Top + 1) % List->MaximumRecords;
    // Return the number of items left in the queue
    return ( ( List->Bottom < List->Top ? List->Bottom + List->MaximumRecords : List->Bottom ) - List->Top );
}
