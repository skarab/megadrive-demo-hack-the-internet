#include "helpers.h"

void VDP_SetScrollMode(u8 horizontal, u8 vertical)
{
    VDP_setReg(0x0b, horizontal|vertical);
}

void VDP_DrawBitmap(const Bitmap* bitmap, u16 plan, u8 palette)
{
    u16 w = bitmap->w;
    u16 h = bitmap->h;
    VDP_loadBMPTileData((u32*)bitmap->image, 1, w/8, h/8, w/8);
    int y;
    for (y=0 ; y<VDP_getPlanHeight()/(h/8); ++y)
        VDP_fillTileMapRectInc(plan, TILE_ATTR_FULL(palette, 0, 0, 0, 1), 0, y*(h/8), w/8, h/8);
}

void VDP_FillBitmap(const Bitmap* bitmap, u16 plan, u8 palette, int offset)
{
    u16 w = bitmap->w;
    u16 h = bitmap->h;
    VDP_loadBMPTileData((u32*)bitmap->image, offset, w / 8, h / 8, w/8 );
    int xs, ys;
    for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
    for (xs=0 ; xs<VDP_getPlanWidth()/(w/8); ++xs)
        VDP_fillTileMapRectInc(plan, TILE_ATTR_FULL(palette, 0, 0, 0, offset), xs*(w/8), ys*(h/8), w / 8, h / 8);
}

void VDP_FadeBitmap(const Bitmap* bitmap, int palette, int value)
{
    int k;
    for (k=0 ; k<16 ; ++k)
    {
        int r = ((u16*)bitmap->palette->data)[k]&15;
        int g = (((u16*)bitmap->palette->data)[k]>>4)&15;
        int b = (((u16*)bitmap->palette->data)[k]>>8)&15;

        r = (r*value)/255;
        g = (g*value)/255;
        b = (b*value)/255;

        VDP_setPaletteColor(k+palette*16, r|(g<<4)|(b<<8));
    }
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

SOUND_LowPassFilterStruct SOUND_LowPassFilter;

void SOUND_LowPassFilterInitialize()
{
    SOUND_LowPassFilter.Beta = 6;
    SOUND_LowPassFilter.RawData = 0;
    SOUND_LowPassFilter.SmoothDataFP = 0;
    SOUND_LowPassFilter.SmoothDataINT = 0;
    SOUND_LowPassFilter.Value = 0;
}

void SOUND_LowPassFilterUpdate(u8 data)
{
    SOUND_LowPassFilter.RawData = (data-127)*2;
    SOUND_LowPassFilter.SmoothDataFP = (SOUND_LowPassFilter.SmoothDataFP<<SOUND_LowPassFilter.Beta)-SOUND_LowPassFilter.SmoothDataFP;
    SOUND_LowPassFilter.SmoothDataFP += SOUND_LowPassFilter.RawData;
    SOUND_LowPassFilter.SmoothDataFP >>= SOUND_LowPassFilter.Beta;

    SOUND_LowPassFilter.Value = SOUND_LowPassFilter.SmoothDataFP;
    if (SOUND_LowPassFilter.Value<0) SOUND_LowPassFilter.Value = -SOUND_LowPassFilter.Value;
    SOUND_LowPassFilter.Value *= 4;
    if (SOUND_LowPassFilter.Value>255) SOUND_LowPassFilter.Value = 255;
}

u16 INPUT_Value;
u8 INPUT_States[INPUT_COUNT*2];

void INPUTS_Initialize()
{
    memset(INPUT_States, 0, INPUT_COUNT*2);
}

void INPUT_CheckStatus(u8 button, u8 id)
{
    INPUT_States[button*2+1] = INPUT_States[button*2];
    INPUT_States[button*2] = INPUT_Value&id;
}

void INPUTS_Update()
{
    INPUT_Value = JOY_readJoypad(0);
    INPUT_CheckStatus(INPUT_UP, BUTTON_UP);
    INPUT_CheckStatus(INPUT_DOWN, BUTTON_DOWN);
    INPUT_CheckStatus(INPUT_LEFT, BUTTON_LEFT);
    INPUT_CheckStatus(INPUT_RIGHT, BUTTON_RIGHT);
    INPUT_CheckStatus(INPUT_A, BUTTON_A);
    INPUT_CheckStatus(INPUT_B, BUTTON_B);
    INPUT_CheckStatus(INPUT_C, BUTTON_C);
    INPUT_CheckStatus(INPUT_START, BUTTON_START);
}

u8 INPUT_IsPressed(u8 button)
{
    return INPUT_States[button*2];
}

u8 INPUT_IsJustPressed(u8 button)
{
    return INPUT_States[button*2] && !INPUT_States[button*2+1];
}

u8 INPUT_IsJustReleased(u8 button)
{
    return !INPUT_States[button*2] && INPUT_States[button*2+1];
}

