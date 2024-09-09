#include <libdragon.h>

#include "vlak_levels.h"
#include "vlak_sprites.h"

#define TILE_SIZE (16)

typedef enum
{
    TEXT_GREEN = 1,
    TEXT_YELLOW,
    TEXT_RED
} vlak_text_color_t;

typedef enum
{
    ANIM_NOT_STARTED,
    ANIM_GOING,
    ANIM_FINISHED
} vlak_animation_state_t;

static uint32_t animation_counter = 0;

static bool vlak_moving = false;
static joypad_8way_t vlak_direction_queued = JOYPAD_8WAY_RIGHT;
static joypad_8way_t vlak_direction = JOYPAD_8WAY_RIGHT;

static vlak_animation_state_t vlak_explosion_anim = ANIM_NOT_STARTED;
static uint32_t vlak_explosion_time = 0;

static vlak_animation_state_t vrata_opening_anim = ANIM_NOT_STARTED;
static uint32_t vrata_opening_time = 0;

static bool should_load_level = true;
static int current_level_id = 1;
static vlak_level_t current_level;

void vlak_load_level()
{
    memcpy(&current_level, vlak_level_array[current_level_id], sizeof(vlak_level_t));

    vlak_moving = false;
    vlak_direction = JOYPAD_8WAY_RIGHT;
    vlak_explosion_anim = ANIM_NOT_STARTED;
    vrata_opening_anim = ANIM_NOT_STARTED;
    should_load_level = false;
}

void vlak_process_input()
{
    joypad_poll();
    joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if (pressed.a)
    {
        current_level_id++;
        should_load_level = true;
    }
    if (pressed.b)
    {
        current_level_id--;
        should_load_level = true;
    }

    joypad_8way_t direction = joypad_get_direction(JOYPAD_PORT_1, JOYPAD_2D_ANY);
    // start moving vlak if stopped and not exploded
    if (!vlak_moving && vlak_explosion_anim == ANIM_NOT_STARTED && direction != JOYPAD_8WAY_NONE)
    {
        vlak_moving = true;
    }

    switch (direction)
    {
        case JOYPAD_8WAY_UP:
        case JOYPAD_8WAY_RIGHT:
        case JOYPAD_8WAY_DOWN:
        case JOYPAD_8WAY_LEFT:
            vlak_direction_queued = direction;
            break;
        case JOYPAD_8WAY_UP_LEFT:
            if (vlak_direction == JOYPAD_8WAY_UP)
            {
                vlak_direction_queued = JOYPAD_8WAY_LEFT;
            }
            else if (vlak_direction == JOYPAD_8WAY_LEFT)
            {
                vlak_direction_queued = JOYPAD_8WAY_UP;
            }
            break;
        case JOYPAD_8WAY_UP_RIGHT:
            if (vlak_direction == JOYPAD_8WAY_UP)
            {
                vlak_direction_queued = JOYPAD_8WAY_RIGHT;
            }
            else if (vlak_direction == JOYPAD_8WAY_RIGHT)
            {
                vlak_direction_queued = JOYPAD_8WAY_UP;
            }
            break;
        case JOYPAD_8WAY_DOWN_LEFT:
            if (vlak_direction == JOYPAD_8WAY_DOWN)
            {
                vlak_direction_queued = JOYPAD_8WAY_LEFT;
            }
            else if (vlak_direction == JOYPAD_8WAY_LEFT)
            {
                vlak_direction_queued = JOYPAD_8WAY_DOWN;
            }
            break;
        case JOYPAD_8WAY_DOWN_RIGHT:
            if (vlak_direction == JOYPAD_8WAY_DOWN)
            {
                vlak_direction_queued = JOYPAD_8WAY_RIGHT;
            }
            else if (vlak_direction == JOYPAD_8WAY_RIGHT)
            {
                vlak_direction_queued = JOYPAD_8WAY_DOWN;
            }
            break;
        default:
            break;
    }
}

void vlak_move()
{
    int vlak_pos = vlak_level_get_element_pos(&current_level, LOK);
    int next_pos;
    vlak_direction = vlak_direction_queued;

    switch (vlak_direction)
    {
        case JOYPAD_8WAY_UP:
            next_pos = vlak_pos - LEVEL_WIDTH;
            break;
        case JOYPAD_8WAY_RIGHT:
            next_pos = vlak_pos + 1;
            break;
        case JOYPAD_8WAY_DOWN:
            next_pos = vlak_pos + LEVEL_WIDTH;
            break;
        case JOYPAD_8WAY_LEFT:
            next_pos = vlak_pos - 1;
            break;
        
        default:
            next_pos = -1;
            break;
    }

    assertf(next_pos != -1, "Invalid vlak direction.");

    vlak_element_t tile_id = current_level.content[next_pos];

    if (tile_id == ZED)
    {
        current_level.content[vlak_pos] = LOK_BOOM;
        vlak_explosion_anim = ANIM_GOING;
        vlak_explosion_time = animation_counter;
        vlak_moving = false;
    }
    else 
    {
        current_level.content[vlak_pos] = 0;
        current_level.content[next_pos] = LOK;
    }
}

void vlak_render_level(vlak_level_t* level)
{
    for (int tile = 0; tile < LEVEL_LEN; tile++)
    {
        int tile_x = (vlak_level_pos_to_x(tile)) * TILE_SIZE;
        int tile_y = (vlak_level_pos_to_y(tile) + 1) * TILE_SIZE;
        vlak_element_t tile_id = level->content[tile];

        vlak_sprite_t* vlak_sprite = vlak_sprite_array[tile_id];
        // skip when no sprite defined
        if (vlak_sprite == NULL)
        {
            continue;
        }

        int anim_frame = 0;
        switch (tile_id)
        {
            case VRA:
                switch (vrata_opening_anim)
                {
                    case ANIM_NOT_STARTED:
                        anim_frame = 0;
                        break;
                    case ANIM_GOING:
                        anim_frame = animation_counter - vrata_opening_time;
                        if (anim_frame == (vlak_sprite->nframes - 1))
                        {
                            vrata_opening_anim = ANIM_FINISHED;
                        }
                        break;
                    case ANIM_FINISHED:
                        anim_frame = vlak_sprite->nframes - 1;
                        break;
                }
                break;
            case LOK_BOOM:
                switch (vlak_explosion_anim)
                {
                    case ANIM_NOT_STARTED:
                        anim_frame = 0;
                        break;
                    case ANIM_GOING:
                        anim_frame = animation_counter - vlak_explosion_time;
                        if (anim_frame == (vlak_sprite->nframes - 1))
                        {
                            vlak_explosion_anim = ANIM_FINISHED;
                            current_level.content[tile] = LOK_DED;
                        }
                        break;
                    case ANIM_FINISHED:
                        anim_frame = vlak_sprite->nframes - 1;
                        break;
                }
                break;
            default:
                anim_frame = (vlak_sprite->nframes == 1) ? 0 : animation_counter % vlak_sprite->nframes;
                break;
        }

        sprite_t* sprite = vlak_sprite->anim_frames[anim_frame];

        rdpq_blitparms_t blitparms = {0};
        if (tile_id == LOK)
        {
            switch (vlak_direction)
            {
                case JOYPAD_8WAY_UP:
                    tile_x = tile_x + (sprite->width / 2);
                    tile_y = tile_y  + (sprite->height / 2);
                    blitparms = (rdpq_blitparms_t) {.cx = sprite->width / 2, .cy = sprite->height / 2, .theta = 1.57079633f};
                    break;
                case JOYPAD_8WAY_DOWN:
                    tile_x = tile_x + (sprite->width / 2);
                    tile_y = tile_y  + (sprite->height / 2);
                    blitparms = (rdpq_blitparms_t) {.cx = sprite->width / 2, .cy = sprite->height / 2, .theta = -1.57079633f};
                    break;
                case JOYPAD_8WAY_LEFT:
                    blitparms = (rdpq_blitparms_t) {.flip_x = true};
                    break;
                default:
                    break;
            }
        }

        rdpq_sprite_blit(sprite, tile_x, tile_y, &blitparms);
    }
}

int main()
{   
    joypad_init();
    dfs_init(DFS_DEFAULT_LOCATION);
    rdpq_init();

    display_init((resolution_t) {.width = 320, .height = 240}, DEPTH_32_BPP, 3, GAMMA_NONE, FILTERS_DISABLED);

    display_set_fps_limit(10);

    vlak_sprite_init();

    rdpq_font_t* font = __rdpq_font_load_builtin_1();
    rdpq_text_register_font(FONT_BUILTIN_DEBUG_MONO, font);

    rdpq_font_style(font, TEXT_GREEN, &(rdpq_fontstyle_t) { 
        .color = RGBA32(85, 255, 85, 255)
    });
    rdpq_font_style(font, TEXT_YELLOW, &(rdpq_fontstyle_t) { 
        .color = RGBA32(255, 255, 85, 255)
    });
    rdpq_font_style(font, TEXT_RED, &(rdpq_fontstyle_t) { 
        .color = RGBA32(255, 85, 85, 255)
    });

    while (1)
    {
        if (should_load_level)
        {
            vlak_load_level();
        }

        vlak_process_input();

        if (vlak_moving && (animation_counter % 5 == 0))
        {
            vlak_move();
        }

        rdpq_attach_clear(display_get(), NULL);

        rdpq_set_mode_standard();

        vlak_render_level(&current_level);

        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = 1}, FONT_BUILTIN_DEBUG_MONO, 13.5 * TILE_SIZE, 13.75 * TILE_SIZE, "LEVEL ^02%i^01: ^02%s", current_level_id, current_level.name);

        rdpq_detach_show();

        animation_counter++;
    }
}