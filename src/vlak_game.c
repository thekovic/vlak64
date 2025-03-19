#include "vlak_game.h"
#include "vlak_sfx.h"

// global game state
vlak_game_t g = {0};

void vlak_game_init()
{
    // check if libdragon API changed (this might very paranoid)
    assertf((JOYPAD_8WAY_DOWN - JOYPAD_8WAY_UP) == 4 && (JOYPAD_8WAY_LEFT - JOYPAD_8WAY_RIGHT) == 4, "Libdragon API changed - can't check for opposite directions like this anymore");
    
    g.title_screen_playing = true;

    g.train_direction_queued = JOYPAD_8WAY_RIGHT;
    g.train_direction = JOYPAD_8WAY_RIGHT;

    g.should_load_level = true;
    g.current_level_id = LEVEL_ID_TITLE_SCREEN;

    g.game_border = BORDER_NONE;
    g.game_speed = SPEED_NORMAL;
}

void vlak_set_game_speed()
{
    int game_speed = 12;
    switch (g.game_speed)
    {
        case SPEED_SLOW:
            game_speed = 10;
            break;
        case SPEED_NORMAL:
            game_speed = 12;
            break;
        case SPEED_FAST:
            game_speed = 15;
            break;
        case SPEED_VERY_FAST:
            game_speed = 18;
            break;
        case SPEED_NIGHTMARE:
            game_speed = 20;
            break;
        case SPEED_LUDICROUS:
            game_speed = 30;
            break;
    }

    display_set_fps_limit(game_speed);
}

void vlak_set_display()
{
    static bool ran_once = false;

    if (!ran_once)
    {
        ran_once = true;
        goto create_display;
    }

    rspq_wait();
    display_close();

create_display:
    float margin = 0.025 * g.game_border;
    display_init(
        (resolution_t) {.width = 320, .height = 240, .overscan_margin = margin},
        DEPTH_32_BPP, 3, GAMMA_NONE,
        (g.game_border) ? FILTERS_RESAMPLE : FILTERS_DISABLED
    );

    vlak_set_game_speed();
}

bool vlak_set_level(int level_id)
{
    if ((level_id < 0) || (level_id >= LEVEL_MAX))
    {
        return false;
    }

    if (level_id == LEVEL_ID_TITLE_SCREEN)
    {
        g.title_screen_playing = true;
    }

    g.current_level_id = level_id;
    
    return true;
}

void vlak_load_level()
{
    g.should_load_level = false;

    // load level data
    memcpy(&g.current_level, vlak_level_array[g.current_level_id], sizeof(vlak_level_t));

    // reset global state
    g.train_moving = false;
    g.train_direction = JOYPAD_8WAY_RIGHT;
    g.train_explosion_anim = ANIM_NOT_STARTED;
    g.door_opening_anim = ANIM_NOT_STARTED;
    g.items_to_collect = 0;
    g.items_collected = 0;
    memset(g.wagon_array, 0, sizeof(vlak_wagon_t) * LEVEL_PLAYABLE_SPACE_LEN);

    // count items to prepare level exit condition
    for (int tile = 0; tile < LEVEL_LEN; tile++)
    {
        vlak_element_t tile_id = g.current_level.content[tile];
        if (tile_id > 0 && tile_id <= LET)
        {
            g.items_to_collect++;
        }
    }
}

static void vlak_move(int train_pos, int next_pos, vlak_element_t append_type)
{
    // only train
    if (g.items_collected == 0)
    {
        g.current_level.content[train_pos] = append_type;
        g.current_level.content[next_pos] = LOK;
        return;
    }

    // store position of last wagon
    int last_pos = g.wagon_array[g.items_collected - 1].pos;

    // move wagons 2 to N to position 1 to (N-1) 
    for (int i = g.items_collected - 1; i > 0; i--)
    {
        g.wagon_array[i].pos = g.wagon_array[i - 1].pos;
        g.wagon_array[i].direction = g.wagon_array[i - 1].direction;
        g.current_level.content[g.wagon_array[i].pos] = g.wagon_array[i].type;
    }

    // move 1st wagon to train pos
    g.wagon_array[0].pos = train_pos;
    g.wagon_array[0].direction = g.train_direction;
    g.current_level.content[g.wagon_array[0].pos] = g.wagon_array[0].type;

    // move train
    g.current_level.content[next_pos] = LOK;

    // append new wagon to the end (if collecting) or make the place empty
    g.current_level.content[last_pos] = append_type;
}

void vlak_title_screen_tick()
{
    int train_pos = vlak_level_get_element_pos(&g.current_level, LOK);
    // move train forwards
    if (train_pos > 0 && train_pos < 179)
    {
        vlak_move(train_pos, train_pos + 1, NIC);
    }
    // remove train at the end of screen to prevent wrapping
    if (train_pos == 179)
    {
        g.current_level.content[train_pos] = NIC;
    }

    if (!g.title_screen_leaving)
    {
        g.title_screen_timer++;
    }
    // play fake level transition screen
    if (g.title_screen_timer == 30)
    {
        g.level_transit_anim = ANIM_GOING;
    }
    // spawn train again and reset title screen cycle
    if (g.title_screen_timer == 40)
    {
        g.current_level.content[160] = LOK;
        g.title_screen_timer = 0;
    }
}

void vlak_collision_check()
{
    int train_pos = vlak_level_get_element_pos(&g.current_level, LOK);
    int next_pos = -1;
    
    switch (g.train_direction_queued)
    {
        case JOYPAD_8WAY_UP:
            next_pos = train_pos - LEVEL_WIDTH;
            break;
        case JOYPAD_8WAY_RIGHT:
            next_pos = train_pos + 1;
            break;
        case JOYPAD_8WAY_DOWN:
            next_pos = train_pos + LEVEL_WIDTH;
            break;
        case JOYPAD_8WAY_LEFT:
            next_pos = train_pos - 1;
            break;
        
        default:
            next_pos = -1;
            break;
    }

    assertf(next_pos != -1, "Invalid vlak direction.");

    vlak_element_t tile_id = g.current_level.content[next_pos];

    // collide with solid and explode
    if ((tile_id == VRA && g.door_opening_anim == ANIM_NOT_STARTED) || tile_id >= ZED)
    {
        g.current_level.content[train_pos] = LOK_BOOM;
        g.train_explosion_anim = ANIM_GOING;
        g.train_explosion_time = g.anim_counter;
        g.train_moving = false;
        // score penalty upon explosion
        g.game_score -= (g.items_to_collect / 4);
        wav64_play(&vlak_sfx_boom, 3);
    }
    // enter door and exit level
    else if ((tile_id == VRA && g.door_opening_anim != ANIM_NOT_STARTED))
    {
        g.level_transit_anim = ANIM_GOING;
        g.train_moving = false;

        vlak_move(train_pos, next_pos, NIC);
        wav64_play(&vlak_sfx_win, 2);
    }
    // collect item
    else if (tile_id > NIC && tile_id <= LET)
    {
        vlak_element_t wagon_type = tile_id + LOK;
        // spawn wagon behind train
        if (g.items_collected == 0)
        {
            g.wagon_array[g.items_collected] = (vlak_wagon_t) {
                .pos = train_pos, 
                .direction = g.train_direction,
                .type = wagon_type
            };
        }
        // spawn wagon behind last wagon
        else
        {
            g.wagon_array[g.items_collected] = (vlak_wagon_t) {
                .pos = g.wagon_array[g.items_collected - 1].pos,
                .direction = g.wagon_array[g.items_collected - 1].direction,
                .type = wagon_type
            };
        }

        vlak_move(train_pos, next_pos, wagon_type);

        g.game_score++;
        g.items_collected++;
        if (g.items_collected >= g.items_to_collect)
        {
            g.door_opening_anim = ANIM_GOING;
            g.door_opening_time = g.anim_counter;
        }

        wav64_play(&vlak_sfx_collect, 1);
    }
    // move to empty tile
    else 
    {
        vlak_move(train_pos, next_pos, NIC);
        wav64_play(&vlak_sfx_step, 0);
    }

    g.train_direction = g.train_direction_queued;
}
