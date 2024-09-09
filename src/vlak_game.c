#include "vlak_game.h"

vlak_game_t g = {0};

void vlak_game_init()
{
    g.should_load_level = true;
    g.current_level_id = 1;
}

void vlak_load_level()
{
    memcpy(&g.current_level, vlak_level_array[g.current_level_id], sizeof(vlak_level_t));

    g.train_moving = false;
    g.train_direction = JOYPAD_8WAY_RIGHT;
    g.train_explosion_anim = ANIM_NOT_STARTED;
    g.door_opening_anim = ANIM_NOT_STARTED;
    g.items_to_collect = 0;
    g.items_collected = 0;
    memset(g.wagon_array, 0, sizeof(vlak_wagon_t) * LEVEL_PLAYABLE_SPACE_LEN);

    for (int tile = 0; tile < LEVEL_LEN; tile++)
    {
        vlak_element_t tile_id = g.current_level.content[tile];
        if (tile_id > 0 && tile_id <= LET)
        {
            g.items_to_collect++;
        }
    }

    g.should_load_level = false;
}

void vlak_process_input()
{
    joypad_poll();
    joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if (pressed.a)
    {
        g.current_level_id++;
        g.should_load_level = true;
    }
    if (pressed.b)
    {
        g.current_level_id--;
        g.should_load_level = true;
    }
    if (pressed.start && g.train_explosion_anim == ANIM_FINISHED)
    {
        g.should_load_level = true;
    }

    joypad_8way_t direction = joypad_get_direction(JOYPAD_PORT_1, JOYPAD_2D_ANY);
    // start moving train if stopped and not exploded
    if (!g.train_moving && g.train_explosion_anim == ANIM_NOT_STARTED && direction != JOYPAD_8WAY_NONE)
    {
        g.train_moving = true;
    }

    switch (direction)
    {
        case JOYPAD_8WAY_UP:
        case JOYPAD_8WAY_RIGHT:
        case JOYPAD_8WAY_DOWN:
        case JOYPAD_8WAY_LEFT:
            g.train_direction_queued = direction;
            break;
        case JOYPAD_8WAY_UP_LEFT:
            if (g.train_direction == JOYPAD_8WAY_UP)
            {
                g.train_direction_queued = JOYPAD_8WAY_LEFT;
            }
            else if (g.train_direction == JOYPAD_8WAY_LEFT)
            {
                g.train_direction_queued = JOYPAD_8WAY_UP;
            }
            break;
        case JOYPAD_8WAY_UP_RIGHT:
            if (g.train_direction == JOYPAD_8WAY_UP)
            {
                g.train_direction_queued = JOYPAD_8WAY_RIGHT;
            }
            else if (g.train_direction == JOYPAD_8WAY_RIGHT)
            {
                g.train_direction_queued = JOYPAD_8WAY_UP;
            }
            break;
        case JOYPAD_8WAY_DOWN_LEFT:
            if (g.train_direction == JOYPAD_8WAY_DOWN)
            {
                g.train_direction_queued = JOYPAD_8WAY_LEFT;
            }
            else if (g.train_direction == JOYPAD_8WAY_LEFT)
            {
                g.train_direction_queued = JOYPAD_8WAY_DOWN;
            }
            break;
        case JOYPAD_8WAY_DOWN_RIGHT:
            if (g.train_direction == JOYPAD_8WAY_DOWN)
            {
                g.train_direction_queued = JOYPAD_8WAY_RIGHT;
            }
            else if (g.train_direction == JOYPAD_8WAY_RIGHT)
            {
                g.train_direction_queued = JOYPAD_8WAY_DOWN;
            }
            break;
        default:
            break;
    }
}

void vlak_move(int train_pos, int next_pos, vlak_element_t append_type)
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
    }
    // enter door and exit level
    else if ((tile_id == VRA && g.door_opening_anim != ANIM_NOT_STARTED))
    {
        g.current_level_id++;
        g.should_load_level = true;

        vlak_move(train_pos, next_pos, NIC);
    }
    // collect item
    else if (tile_id > NIC && tile_id <= LET)
    {
        vlak_element_t wagon_type = tile_id + LOK;
        if (g.items_collected == 0)
        {
            g.wagon_array[g.items_collected] = (vlak_wagon_t) {
                .pos = train_pos, 
                .direction = g.train_direction,
                .type = wagon_type
            };
        }
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
    }
    // move to empty tile
    else 
    {
        vlak_move(train_pos, next_pos, NIC);
    }

    g.train_direction = g.train_direction_queued;
}
