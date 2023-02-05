
#pragma once

typedef void(*StageCallback)();

typedef struct StageRecord
{
    StageCallback Start;                // Called when the engine is changing to this stage
    StageCallback Update;               // Called every frame whilst the stage is active
    StageCallback Finish;               // Called when then engine is changing to a new stage
} StageRecord;

