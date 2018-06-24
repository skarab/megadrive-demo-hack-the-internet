#include "helpers.h"

void EFFECT_Tube(int stop)
{
    u16* scroll_table;
    u8 interactive = 0;

    void init_table()
    {
        scroll_table = (u16*)MEM_alloc(21*42*sizeof(u16));
        int k, h;
        int center = 8;
        for (k=0 ; k<42 ; ++k)
        {
            int count = (k*k)/10;
            for (h=0 ; h<21 ; ++h)
            {
				int z = (h-8)*(h-8);
                int xv;
                if (k<center) xv = z*((center-k)*(center-k))/60;
                else xv = -z*((k-center)*(k-center))/60;
                scroll_table[k*21+h] = 0x840-count+xv;
            }
        }
    }

    int vertical_scroll = 8;
    int position_y = 0;
    u16* scroll_data = NULL;
	int roll = 0;

    void vblank()
    {
        position_y += vertical_scroll;
        scroll_data = scroll_table;

        if (!interactive)
            roll += 2;
    }

    void hblank()
    {
        vu16 *pw = (u16 *) GFX_DATA_PORT;
        vu32 *pl = (u32 *) GFX_CTRL_PORT;

		int rolling = cosFix16(roll);
        int x;
        for (x=0 ; x<21 ; ++x)
        {
			int scroll = -position_y+(x-10)*rolling/32+10;

            u16 addr = (x&0x1F)*4;
            *pl = GFX_WRITE_VSRAM_ADDR(addr);
            *pw = scroll+scroll_data[x];
            *pl = GFX_WRITE_VSRAM_ADDR(addr+2);
            *pw = scroll/2+scroll_data[x];
        }

        scroll_data += 21;
    }

    VDP_SetScrollMode(VDP_HSCROLL_Plane, VDP_VSCROLL_Column);

    init_table();

    VDP_setPaletteColors(16, tube_back.palette->data, 16);
    VDP_setPaletteColors(32, tube.palette->data, 16);

    VDP_FillBitmap(&tube, APLAN, PAL2, 1);
    VDP_FillBitmap(&tube_back, BPLAN, PAL1, 257);

    SYS_setVIntCallback(vblank);
    SYS_setHIntCallback(hblank);
    VDP_setHIntCounter(1);
    VDP_setHInterrupt(1);

    INPUTS_Initialize();

    u32 time;
    u32 horizontal_scroll = 0;
    do
    {
        time = SOUND_GetTime(music);
        u8 data = music[time];
        SOUND_LowPassFilterUpdate(data);

        INPUTS_Update();

        if (INPUT_IsJustPressed(INPUT_START))
            interactive = 1;

        if (interactive)
        {
            vertical_scroll = 1;

            if (INPUT_IsPressed(INPUT_UP)) vertical_scroll += 4;
            if (INPUT_IsPressed(INPUT_DOWN)) vertical_scroll -= 4;
            if (INPUT_IsPressed(INPUT_LEFT)) { horizontal_scroll += 2; roll += 2; }
            if (INPUT_IsPressed(INPUT_RIGHT)) { horizontal_scroll -= 2; roll -= 2; }

            VDP_setHorizontalScroll(PLAN_A, horizontal_scroll);
            VDP_setHorizontalScroll(PLAN_B, horizontal_scroll);
        }
        else
        {
            VDP_setHorizontalScroll(PLAN_A, sinFix16(horizontal_scroll)*2);
            VDP_setHorizontalScroll(PLAN_B, sinFix16(horizontal_scroll)*2);
            horizontal_scroll += 4;
        }
    }
    while (time<stop || interactive);

    SYS_setVIntCallback(0);
    SYS_setHIntCallback(0);
	VDP_SetScrollMode(VDP_HSCROLL_Plane, VDP_HSCROLL_Plane);
    VDP_setHorizontalScroll(PLAN_A, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
    VDP_setHorizontalScroll(PLAN_B, 0);
   	VDP_setVerticalScroll(PLAN_B, 0);
   	VDP_clearPlan(APLAN, 0);
   	VDP_clearPlan(BPLAN, 0);
   	MEM_free(scroll_table);
}

