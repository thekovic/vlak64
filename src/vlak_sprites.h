#ifndef VLAK_SPRITES_H
#define VLAK_SPRITES_H

#include <libdragon.h>

#include "vlak_elements.h"

typedef struct vlak_sprite_s
{
    uint8_t nframes;
    sprite_t* animation_frames[6];
} vlak_sprite_t;

extern vlak_sprite_t* vlak_sprite_array[NUM_LEVEL_ELEMENTS];

void vlak_sprite_init();

#endif
