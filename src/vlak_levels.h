#ifndef VLAK_LEVELS_H
#define VLAK_LEVELS_H

#include <libdragon.h>

#include "vlak_elements.h"

#define LEVEL_NAME_LEN (6)

#define LEVEL_WIDTH (20)
#define LEVEL_HEIGHT (12)
#define LEVEL_LEN (LEVEL_WIDTH * LEVEL_HEIGHT)
#define LEVEL_PLAYABLE_SPACE_LEN ((LEVEL_WIDTH - 2) * (LEVEL_HEIGHT - 2))

typedef struct level_s
{
    char name[LEVEL_NAME_LEN];
    uint8_t content[LEVEL_LEN];
} vlak_level_t;

#define LEVEL_ID_TITLE_SCREEN (0)
#define LEVEL_ID_START (1)
#define LEVEL_MAX (52)

extern vlak_level_t* vlak_level_array[LEVEL_MAX];

int vlak_level_get_element_pos(vlak_level_t* level, vlak_element_t element);
#define vlak_level_pos_to_x(pos) ((pos) % LEVEL_WIDTH)
#define vlak_level_pos_to_y(pos) ((pos) / LEVEL_WIDTH)
#define vlak_level_xy_to_pos(x, y) (LEVEL_WIDTH * (y) + (x))

#endif
