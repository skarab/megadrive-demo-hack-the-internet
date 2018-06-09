#include "genesis.h"

#define VDP_HSCROLL_Plane  0
#define VDP_HSCROLL_Row    2
#define VDP_HSCROLL_Line   3
#define VDP_VSCROLL_Plane  0
#define VDP_VSCROLL_Column 4

void VDP_SetScrollMode(u8 horizontal, u8 vertical);

u32 SOUND_GetTime(const u8* pcm);
