#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "genesis.h"
#include "music.h"
#include "res/resources.h"

#define VDP_HSCROLL_Plane  0
#define VDP_HSCROLL_Row    2
#define VDP_HSCROLL_Line   3
#define VDP_VSCROLL_Plane  0
#define VDP_VSCROLL_Column 4

void VDP_SetScrollMode(u8 horizontal, u8 vertical);
void VDP_DrawBitmap(const Bitmap* bitmap, u16 plan, u8 palette);
void VDP_FillBitmap(const Bitmap* bitmap, u16 plan, u8 palette, int offset);
void VDP_FadeBitmap(const Bitmap* bitmap, int palette, int value);

u32 SOUND_GetTime(const u8* pcm);

typedef struct {
    int RawData;
    signed long SmoothDataINT;
    signed long SmoothDataFP;
    int Beta;
    int Value;
} SOUND_LowPassFilterStruct;

extern SOUND_LowPassFilterStruct SOUND_LowPassFilter;

void SOUND_LowPassFilterInitialize();
void SOUND_LowPassFilterUpdate(u8 data);

#define INPUT_UP       0
#define INPUT_DOWN     1
#define INPUT_LEFT     2
#define INPUT_RIGHT    3
#define INPUT_A        4
#define INPUT_B        5
#define INPUT_C        6
#define INPUT_START    7
#define INPUT_COUNT    8

void INPUTS_Initialize();
void INPUTS_Update();

u8 INPUT_IsPressed(u8 button);
u8 INPUT_IsJustPressed(u8 button);
u8 INPUT_IsJustReleased(u8 button);

#endif
