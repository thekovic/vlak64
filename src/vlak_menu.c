#include "vlak_game.h"

static void vlak_menu_change_border(int choice)
{
    vlak_border_margin_t new_border = g.game_border + choice;
    if (new_border >= 0 && new_border < BORDER_NUM)
    {
        g.game_border = new_border;
        vlak_set_display();
    }
}

static void vlak_menu_change_game_speed(int choice)
{
    vlak_game_speed_t new_speed = g.game_speed + choice;
    if (new_speed >= 0 && new_speed < SPEED_NUM)
    {
        g.game_speed = new_speed;
        vlak_set_game_speed();
    }
}

static void vlak_menu_change_level(int choice)
{
    g.should_load_level = vlak_set_level(g.current_level_id + choice);
}

vlak_menu_item_t menu[MENU_LEN] = {
    {.name = "CRT Border", .routine = vlak_menu_change_border, (int*) &g.game_border},
    {.name = "Game Speed", .routine = vlak_menu_change_game_speed, (int*) &g.game_speed},
    {.name = "Level", .routine = vlak_menu_change_level, &g.current_level_id}
};

const char* vlak_menu_get_option_name(int menu_index, int option_index)
{
    const char* menu_border_option_names[BORDER_NUM] = {"None", "Mild", "Safe"};
    const char* menu_speed_option_names[SPEED_NUM] = {"Slow", "Normal", "Fast"};

    if (menu_index == MENU_BORDER)
    {
        return menu_border_option_names[option_index];
    }
    if (menu_index == MENU_SPEED)
    {
        return menu_speed_option_names[option_index];
    }
    if (menu_index == MENU_LEVEL)
    {
        return vlak_level_array[option_index]->name;
    }

    return NULL;
}
