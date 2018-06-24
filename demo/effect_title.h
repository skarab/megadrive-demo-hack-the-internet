#include "helpers.h"

void EFFECT_Title(int stop)
{
    VDP_SetScrollMode(VDP_HSCROLL_Line, VDP_HSCROLL_Plane);

    int height = title.h;
    s16 values[height];
    int i;
    for (i=0 ; i<height ; ++i)
        values[i] = title.w;
    VDP_setHorizontalScrollLine(PLAN_A, 0, values, height, 0);

    VDP_setPaletteColors(0, title.palette->data, 16);
    VDP_DrawBitmap(&title, APLAN, PAL0);

    u32 time;
    u32 start = SOUND_GetTime(music);
    do
    {
        time = SOUND_GetTime(music);
        u8 data = music[time];

        int height = title.h;
        s16 values[height];
		int i;
		for (i=0 ; i<height ; ++i)
		{
			if (i<height/3)
			{
                values[i] = title.w-title.w*3*(time-start)/(stop-start);
        	}
			else if (i<height*2/3)
			{
                values[i] = title.w-title.w*3*(time-(start+700))/(stop-(start+700));
        	}
			else
			{
                values[i] = title.w-title.w*3*(time-(start+1400))/(stop-(start+1400));
        	}

			if (values[i]>title.w) values[i] = title.w;
			if (values[i]<-title.w) values[i] = -title.w;
		}
		VDP_setHorizontalScrollLine(PLAN_A, 0, values, height, 0);
    }
    while (time<stop);
}
