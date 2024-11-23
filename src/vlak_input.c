#include "vlak_game.h"

static void vlak_process_title_screen_input()
{
    joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if (pressed.start)
    {
        if (g.title_screen_playing)
        {
            g.title_screen_leaving = true;
            g.level_transit_anim = ANIM_GOING;
        }
    }
}

static void vlak_process_menu_input()
{
    joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    if (pressed.d_up || pressed.c_up)
    {
        g.menu_index = (g.menu_index - 1 < 0) ? 0 : g.menu_index - 1;
    }
    else if (pressed.d_down || pressed.c_down)
    {
        g.menu_index = (g.menu_index + 1 >= MENU_LEN) ? (MENU_LEN - 1) : g.menu_index + 1;
    }
    else if (pressed.d_left || pressed.c_left)
    {
        menu[g.menu_index].routine(-1);
    }
    else if (pressed.d_right || pressed.c_right)
    {
        menu[g.menu_index].routine(1);
    }

    // Close menu
    if (pressed.l)
    {
        g.menu_open = false;
    }
}

void vlak_process_input()
{
    joypad_poll();

    if (g.title_screen_playing)
    {
        vlak_process_title_screen_input();
        return;
    }

    if (g.menu_open)
    {
        vlak_process_menu_input();
        return;
    }

    joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    if (pressed.start)
    {
        if (g.train_explosion_anim == ANIM_FINISHED)
        {
            // Reset current level after train exploded
            g.should_load_level = vlak_set_level(g.current_level_id);
        }
        if (g.level_transit_anim == ANIM_FINISHED)
        {
            g.level_transit_anim = ANIM_REVERSE;
            int next_level_id = (g.current_level_id + 1 == LEVEL_MAX) ? LEVEL_ID_TITLE_SCREEN : g.current_level_id + 1;
            g.should_load_level = vlak_set_level(next_level_id);
        }
    }

    // don't accept inputs if level transition animation is playing
    if (g.level_transit_anim != ANIM_NOT_STARTED)
    {
        return;
    }

    if (pressed.l)
    {
        g.menu_open = true;
        g.menu_index = 0;
        return;
    }

    joypad_8way_t direction = joypad_get_direction(JOYPAD_PORT_1, JOYPAD_2D_ANY);
    // start moving train if stopped and not exploded
    if (!g.train_moving && g.train_explosion_anim == ANIM_NOT_STARTED && direction != JOYPAD_8WAY_NONE)
    {
        g.train_moving = true;
    }

    switch (direction)
    {
        // simple ordinal movement
        case JOYPAD_8WAY_UP:
        case JOYPAD_8WAY_RIGHT:
        case JOYPAD_8WAY_DOWN:
        case JOYPAD_8WAY_LEFT:
            g.train_direction_queued = direction;
            break;
        // something to handle diagonal inputs (still recommend to not use them)
        case JOYPAD_8WAY_UP_LEFT:
            switch (g.train_direction)
            {
                case JOYPAD_8WAY_UP:
                case JOYPAD_8WAY_DOWN:
                    g.train_direction_queued = JOYPAD_8WAY_LEFT;
                    break;
                case JOYPAD_8WAY_RIGHT:
                case JOYPAD_8WAY_LEFT:
                    g.train_direction_queued = JOYPAD_8WAY_UP;
                    break;
                default:
                    break;
            }
            break;
        case JOYPAD_8WAY_UP_RIGHT:
            switch (g.train_direction)
            {
                case JOYPAD_8WAY_UP:
                case JOYPAD_8WAY_DOWN:
                    g.train_direction_queued = JOYPAD_8WAY_RIGHT;
                    break;
                case JOYPAD_8WAY_RIGHT:
                case JOYPAD_8WAY_LEFT:
                    g.train_direction_queued = JOYPAD_8WAY_UP;
                    break;
                default:
                    break;
            }
            break;
        case JOYPAD_8WAY_DOWN_LEFT:
            switch (g.train_direction)
            {
                case JOYPAD_8WAY_UP:
                case JOYPAD_8WAY_DOWN:
                    g.train_direction_queued = JOYPAD_8WAY_LEFT;
                    break;
                case JOYPAD_8WAY_RIGHT:
                case JOYPAD_8WAY_LEFT:
                    g.train_direction_queued = JOYPAD_8WAY_DOWN;
                    break;
                default:
                    break;
            }
            break;
        case JOYPAD_8WAY_DOWN_RIGHT:
            switch (g.train_direction)
            {
                case JOYPAD_8WAY_UP:
                case JOYPAD_8WAY_DOWN:
                    g.train_direction_queued = JOYPAD_8WAY_RIGHT;
                    break;
                case JOYPAD_8WAY_RIGHT:
                case JOYPAD_8WAY_LEFT:
                    g.train_direction_queued = JOYPAD_8WAY_DOWN;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    // prevent moving into the wagon immediately behind the train
    if (g.items_collected > 0 && abs(g.train_direction_queued - g.train_direction) == 4)
    {
        g.train_direction_queued = g.train_direction;
    }
}
