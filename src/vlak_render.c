#include <libdragon.h>

#include "vlak_render.h"
#include "vlak_sprites.h"
#include "vlak_game.h"

#define TILE_SIZE (16)

typedef enum
{
    TEXT_GREEN = 1,
    TEXT_YELLOW,
    TEXT_RED
} vlak_text_color_t;

void vlak_text_init()
{
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
}

void vlak_render_border()
{

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
            // special handling for elements with single-use animation
            case VRA:
                switch (g.door_opening_anim)
                {
                    case ANIM_NOT_STARTED:
                        anim_frame = 0;
                        break;
                    case ANIM_GOING:
                        anim_frame = g.anim_counter - g.door_opening_time;
                        if (anim_frame == (vlak_sprite->nframes - 1))
                        {
                            g.door_opening_anim = ANIM_FINISHED;
                        }
                        break;
                    case ANIM_FINISHED:
                        anim_frame = vlak_sprite->nframes - 1;
                        break;
                }
                break;
            case LOK_BOOM:
                switch (g.train_explosion_anim)
                {
                    case ANIM_NOT_STARTED:
                        anim_frame = 0;
                        break;
                    case ANIM_GOING:
                        anim_frame = g.anim_counter - g.train_explosion_time;
                        if (anim_frame == (vlak_sprite->nframes - 1))
                        {
                            g.train_explosion_anim = ANIM_FINISHED;
                            g.current_level.content[tile] = LOK_DED;
                        }
                        break;
                    case ANIM_FINISHED:
                        anim_frame = vlak_sprite->nframes - 1;
                        break;
                }
                break;
            // everything else
            default:
                anim_frame = (vlak_sprite->nframes == 1) ? 0 : g.anim_counter % vlak_sprite->nframes;
                break;
        }

        sprite_t* sprite = vlak_sprite->anim_frames[anim_frame];

        rdpq_blitparms_t blitparms = {0};
        // train and wagons are transformed based on direction
        if (tile_id >= LOK)
        {
            joypad_8way_t direction = -1;
            if (tile_id == LOK)
            {
                direction = g.train_direction;
            }
            else
            {
                // FIXME: this way of getting direction is O(n^2), surely there's a better way
                // (maybe embedding info about direction directly into the level map tiles)
                for (int i = g.items_collected; i >= 0; i--)
                {
                    vlak_wagon_t vagon = g.wagon_array[i];
                    if (vagon.pos == tile)
                    {
                        direction = vagon.direction;
                        break;
                    }
                }
            }

            switch (direction)
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

void vlak_render_text()
{
    if (g.train_explosion_anim == ANIM_FINISHED)
    {
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = 1}, FONT_BUILTIN_DEBUG_MONO, 4.75 * TILE_SIZE, 13.75 * TILE_SIZE,
            "Press ^03START ^01to restart level");
    }
    else
    {
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = 1}, FONT_BUILTIN_DEBUG_MONO, 1.25 * TILE_SIZE, 13.75 * TILE_SIZE,
            "SCORE: ^02%i0", g.game_score);
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = 3}, FONT_BUILTIN_DEBUG_MONO, 6.25 * TILE_SIZE, 13.75 * TILE_SIZE,
            "GET: ^02%02i ^03HAVE: ^02%02i", g.items_to_collect, g.items_collected);
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = 1}, FONT_BUILTIN_DEBUG_MONO, 13.25 * TILE_SIZE, 13.75 * TILE_SIZE,
            "LEVEL ^02%02i^01: ^02%s", g.current_level_id, g.current_level.name);
    }
}

void vlak_render_transition()
{

}

void vlak_render()
{
    rdpq_attach_clear(display_get(), NULL);

    rdpq_set_mode_standard();

    vlak_render_border();

    vlak_render_level(&g.current_level);

    vlak_render_text();

    rdpq_detach_show();
}