#ifndef VLAK_SFX_H
#define VLAK_SFX_H

#include <libdragon.h>

// I'm putting each SFX to its own channel so they can play over each other.
// This means this is effectively the same as number of SFX.
#define SFX_CHANNEL_COUNT   (4)

extern wav64_t vlak_sfx_step;
extern wav64_t vlak_sfx_collect;
extern wav64_t vlak_sfx_win;
extern wav64_t vlak_sfx_boom;

// Number of channels for the music. For XM music, this is equal to the number
// of XM channels in the track (aka the number of instruments) which is a different
// constant for every song.
#define MUSIC_CHANNEL_COUNT (16)

extern xm64player_t vlak_sfx_bg_music;

void vlak_sfx_init();

#endif
