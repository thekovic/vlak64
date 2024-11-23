#ifndef VLAK_GAME_H
#define VLAK_GAME_H

#include <libdragon.h>

#include "vlak_elements.h"
#include "vlak_levels.h"

typedef enum
{
    ANIM_NOT_STARTED,
    ANIM_GOING,
    ANIM_FINISHED,
    ANIM_REVERSE
} vlak_anim_state_t;

typedef struct vlak_wagon_s
{
    int pos;
    joypad_8way_t direction;
    vlak_element_t type;
} vlak_wagon_t;

typedef struct vlak_game_s
{
    uint32_t anim_counter;

    bool title_screen_playing;
    bool title_screen_leaving;
    uint32_t title_screen_timer;

    vlak_anim_state_t level_transit_anim;

    bool train_moving;
    joypad_8way_t train_direction_queued;
    joypad_8way_t train_direction;

    vlak_anim_state_t train_explosion_anim;
    uint32_t train_explosion_time;

    vlak_anim_state_t door_opening_anim;
    uint32_t door_opening_time;

    bool should_load_level;
    int current_level_id;
    vlak_level_t current_level;

    int items_to_collect;
    int items_collected;
    vlak_wagon_t wagon_array[LEVEL_PLAYABLE_SPACE_LEN];

    int game_score;
} vlak_game_t;

extern vlak_game_t g;

void vlak_game_init();

bool vlak_change_level(int level_id);

void vlak_load_level();

void vlak_process_input();

void vlak_title_screen_tick();

void vlak_move(int train_pos, int next_pos, vlak_element_t append_type);

void vlak_collision_check();

#endif