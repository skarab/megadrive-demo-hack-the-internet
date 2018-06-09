#include "genesis.h"
#include "helpers.h"
#include "music.h"
#include "res/resources.h"

int main()
{
    VDP_setHInterrupt(0);
    VDP_setHilightShadow(0);
    VDP_setScreenWidth256();

    SND_startPlay_PCM(music, sizeof(music), SOUND_RATE_22050, SOUND_PAN_CENTER, 0);

    u16 w = image.w;
    u16 h = image.h;
    VDP_loadBMPTileData((u32*)image.image, 1, w/8, h/8, w/8);
    int y;
    for (y=0 ; y<VDP_getPlanHeight()/(h/8); ++y)
        VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, y*(h/8), w/8, h/8);

    int RawData;
    signed long SmoothDataINT;
    signed long SmoothDataFP;
    int Beta = 6; // Length of the filter < 16

    while(1)
    {
        u32 time = SOUND_GetTime(music);
        u8 data = music[time];

        RawData = (data-127)*2;
        SmoothDataFP = (SmoothDataFP<< Beta)-SmoothDataFP;
        SmoothDataFP += RawData;
        SmoothDataFP >>= Beta;

        int m = SmoothDataFP;
        if (m<0) m = -m;
        m *= 4;
        if (m>255) m = 255;

        int k;
        for (k=0 ; k<16 ; ++k)
         {
            int r = ((u16*)image.palette->data)[k]&15;
            int g = (((u16*)image.palette->data)[k]>>4)&15;
            int b = (((u16*)image.palette->data)[k]>>8)&15;

            r = (r*m)/255;
            g = (g*m)/255;
            b = (b*m)/255;

            VDP_setPaletteColor(k, r|(g<<4)|(b<<8));
        }
    }

       SND_stopPlay_PCM();

       return 0;
}
