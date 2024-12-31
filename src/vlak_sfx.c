#include "vlak_sfx.h"

wav64_t vlak_sfx_step;
wav64_t vlak_sfx_collect;
wav64_t vlak_sfx_win;
wav64_t vlak_sfx_boom;

xm64player_t vlak_sfx_bg_music;

void vlak_sfx_init()
{
    wav64_open(&vlak_sfx_step, "rom:/sfx_vlak_step.wav64");
    wav64_open(&vlak_sfx_collect, "rom:/sfx_vlak_collect.wav64");
    wav64_open(&vlak_sfx_win, "rom:/sfx_win.wav64");
    wav64_open(&vlak_sfx_boom, "rom:/sfx_boom.wav64");
    
    //xm64player_open(&vlak_sfx_bg_music, "rom:/music_AbsoluteSilence.xm64");
    //xm64player_play(&vlak_sfx_bg_music, SFX_CHANNEL_COUNT);
}