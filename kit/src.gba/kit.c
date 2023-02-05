
#include "../include/kit.h"
#include "gba.h"

void kit_init(Queue* KitQueue)
{
    kit_internal_queue = KitQueue;
    irq_init(null);
    irq_set(II_VBLANK, mmVBlank, ISR_DEF);
    irq_enable(II_VBLANK);
    mmInitDefault( (mm_addr)soundbank_bin, 8 );
    oam_init(OAM, 128);
}

void kit_shutdown()
{

}

void kit_log_message(char* Message)
{

}

void kit_system_update()
{

}

u16 kit_system_ticks()
{
    return 1;
}
