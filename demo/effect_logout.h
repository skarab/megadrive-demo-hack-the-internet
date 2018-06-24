#include "helpers.h"

void EFFECT_Logout(int stop)
{
    VDP_SetScrollMode(VDP_HSCROLL_Line, VDP_HSCROLL_Plane);

    int height = logout.h;
    s16 values[height];
    int i;
    for (i=0 ; i<height ; ++i)
        values[i] = logout.w;
    VDP_setHorizontalScrollLine(PLAN_A, 0, values, height, 0);

    VDP_setPaletteColors(0, logout.palette->data, 16);
    VDP_DrawBitmap(&logout, APLAN, PAL0);

    u32 time;
    u32 start = SOUND_GetTime(music);
    do
    {
        time = SOUND_GetTime(music);
        u8 data = music[time];

        int height = logout.h;
        s16 values[height];
		int i;
		for (i=0 ; i<height ; ++i)
		{
			if (i<height/2)
			{
                values[i] = logout.w-logout.w*3*(time-start)/(stop-start);
        	}
			else
			{
                values[i] = logout.w-logout.w*3*(time-(start+600))/(stop-(start+600));
        	}

			if (values[i]>logout.w) values[i] = logout.w;
			if (values[i]<-logout.w) values[i] = -logout.w;
		}
		VDP_setHorizontalScrollLine(PLAN_A, 0, values, height, 0);
    }
    while (time<stop);
}
