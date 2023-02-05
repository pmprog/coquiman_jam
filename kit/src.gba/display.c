
#include "../include/display.h"
#include "../include/kit.h"
#include "gba.h"

static u8 display_cnt = 0;
static u8 display_doublebuffered = 0;

u8 display_count()
{
    return display_cnt;
}

u8 display_new(u8 Monitor, u16 Width, u16 Height, DisplayFlags Flags)
{
    if(display_cnt == 0)
    {
        display_cnt = 1;
        return 0;
    }
    return 255;
}

void display_close(u8 DisplayIndex)
{
    if(DisplayIndex == 0 && display_cnt != 0)
    {
        display_cnt--;
    }
}

void display_flip(u8 DisplayIndex)
{
    // Check if we're in a bitmap mode with page flip
    if((REG_DISPCNT & DCNT_MODE4) == DCNT_MODE4 && display_doublebuffered != 0)
    {
        vid_page = (COLOR*)((u32)vid_page ^ VRAM_PAGE_SIZE);
        REG_DISPCNT ^= DCNT_PAGE;
    }
    VBlankIntrWait();
    if(music_playing != 0)
    {
        mmFrame();
    }
}


void display_mode_bitmap(u8 DisplayIndex, u8 DoubleBuffer)
{
    REG_DISPCNT = DCNT_MODE4 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;
    display_doublebuffered = DoubleBuffer;
    vid_page = (COLOR*)((u32)vid_page & ~VRAM_PAGE_SIZE);
    REG_DISPCNT &= ~DCNT_PAGE;

}


void display_mode_tilemap(u8 DisplayIndex, u8 NumberOfPlanes, u8 BitsPerPixel)
{
    u16 bitdepth = BG_8BPP;
    switch(BitsPerPixel)
    {
        case 4:
            bitdepth = BG_4BPP;
            break;
        case 8:
            bitdepth = BG_8BPP;
            break;
        default:
            bitdepth = BG_8BPP;
            break;
    }
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
    REG_BG0CNT = BG_WRAP | BG_SIZE0 | BG_SBB(26) | BG_PRIO(3) | bitdepth;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    if( NumberOfPlanes > 1 )
    {
        REG_DISPCNT |= DCNT_BG1;
        REG_BG1CNT = BG_WRAP | BG_SIZE0 | BG_SBB(27) | BG_PRIO(2) | bitdepth;
        REG_BG1HOFS = 0;
        REG_BG1VOFS = 0;
    }
    if( NumberOfPlanes > 2 )
    {
        REG_DISPCNT |= DCNT_BG2;
        REG_BG2CNT = BG_WRAP | BG_SIZE0 | BG_SBB(28) | BG_PRIO(1) | bitdepth;
        REG_BG2HOFS = 0;
        REG_BG2VOFS = 0;
    }
    if( NumberOfPlanes > 3 )
    {
        REG_DISPCNT |= DCNT_BG3;
        REG_BG3CNT = BG_WRAP | BG_SIZE0 | BG_SBB(29) | BG_PRIO(0) | bitdepth;
        REG_BG3HOFS = 0;
        REG_BG3VOFS = 0;
    }
}
