#include "helpers.h"

void VDP_SetScrollMode(u8 horizontal, u8 vertical)
{
    VDP_setReg(0x0b, horizontal|vertical);
}

u32 SOUND_GetTime(const u8* music)
{
    u32 pcm = (u32)music;
    Z80_requestBus(1);
    vu8 *pb = (u8*)(Z80_DRV_PARAMS+0x10);
    u8 a=pb[0];
    u8 b=pb[1];
    Z80_releaseBus();
    u32 cr = (u32)a<<8|(u32)b<<16;
    cr -= pcm;
    cr /= 16;
    return cr;
}
