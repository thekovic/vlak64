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

void vlak_sfx_init();

#endif
