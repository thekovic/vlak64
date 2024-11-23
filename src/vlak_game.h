#ifndef VLAK_GAME_H
#define VLAK_GAME_H

#include <libdragon.h>

#include "vlak_elements.h"
#include "vlak_levels.h"

typedef enum
{
    MENU_BORDER,
    MENU_SPEED,
    MENU_LEVEL
} vlak_menu_option_t;
#define MENU_LEN (3)

typedef struct vlak_menu_item_s
{
    char name[10];
    // Function operating this setting.
    // choice = 0: left, 1: right
    void (*routine) (int choice);
    // Pointer to the value this menu item is modifying.
    int* value;
} vlak_menu_item_t;

extern vlak_menu_item_t menu[MENU_LEN];

typedef enum
{
    BORDER_NONE,
    BORDER_MILD,
    BORDER_SAFE
} vlak_border_margin_t;
#define BORDER_NUM (3)

typedef enum
{
    SPEED_SLOW,
    SPEED_NORMAL,
    SPEED_FAST
} vlak_game_speed_t;
#define SPEED_NUM (3)

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

    bool menu_open;
    int menu_index;
    vlak_border_margin_t game_border;
    vlak_game_speed_t game_speed;

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

void vlak_text_init();

void vlak_game_init();

void vlak_set_display();

void vlak_set_game_speed();

bool vlak_set_level(int level_id);

void vlak_load_level();

void vlak_process_input();

void vlak_title_screen_tick();

void vlak_collision_check();

void vlak_render();

const char* vlak_menu_get_option_name(int menu_index, int option_index);

#endif