#include "helpers.h"
#include "effect_title.h"
#include "effect_logout.h"
#include "effect_tube.h"

int main()
{
    VDP_setHInterrupt(0);
    VDP_setHilightShadow(0);
    VDP_setScreenWidth256();

    SND_startPlay_PCM(music, sizeof(music), SOUND_RATE_22050, SOUND_PAN_CENTER, 0);
    SOUND_LowPassFilterInitialize();

    EFFECT_Title(2000);
    EFFECT_Tube(8500);
    EFFECT_Logout(10500);
    EFFECT_Tube(17000);
    EFFECT_Title(19000);
    EFFECT_Tube(29000);
    EFFECT_Logout(31000);
    EFFECT_Tube(50000);
    EFFECT_Title(52000);
    EFFECT_Tube(56000);
    EFFECT_Logout(58000);

    while (1) {}
    SND_stopPlay_PCM();

    return 0;
}
