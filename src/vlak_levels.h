#ifndef VLAK_LEVELS_H
#define VLAK_LEVELS_H

#include <libdragon.h>

#define LEVEL_WIDTH (20)
#define LEVEL_HEIGHT (12)
#define LEVEL_LEN (LEVEL_WIDTH * LEVEL_HEIGHT)
#define LEVEL_PLAYABLE_SPACE_LEN ((LEVEL_WIDTH - 2) * (LEVEL_HEIGHT - 2))

typedef struct level_s
{
    uint8_t content[LEVEL_LEN];
} vlak_level_t;

#define LEVEL_MAX (52)

extern vlak_level_t* vlak_level_array[LEVEL_MAX];

int vlak_level_get_vlak_pos(vlak_level_t* level);
#define vlak_level_pos_to_x(pos) ((pos) % LEVEL_WIDTH)
#define vlak_level_pos_to_y(pos) ((pos) / LEVEL_WIDTH)
int vlak_level_xy_to_pos(int x, int y);

#endif
