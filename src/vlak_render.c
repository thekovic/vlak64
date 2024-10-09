#include <libdragon.h>

#include "vlak_render.h"
#include "vlak_sprites.h"
#include "vlak_game.h"

#define TILE_SIZE (16)

typedef enum
{
    TEXT_WHITE,
    TEXT_GREEN,
    TEXT_YELLOW,
    TEXT_RED
} vlak_text_color_t;

static int level_transit_line_count = 0;

void vlak_text_init()
{
    rdpq_font_t* font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
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
    rdpq_set_mode_standard();

    sprite_t* zed_sprite = vlak_sprite_array[ZED]->anim_frames[vlak_sprite_array[ZED]->nframes - 1];

    // upload sprite manually so we can use texture_rect
    rdpq_sprite_upload(TILE0, zed_sprite, &(rdpq_texparms_t) {.s = {.repeats = REPEAT_INFINITE}});
    // top bar
    rdpq_texture_rectangle(TILE0, 0, 0, LEVEL_WIDTH * TILE_SIZE, TILE_SIZE, 0, 0);
    // bottom bar
    rdpq_texture_rectangle(TILE0, 0, (LEVEL_HEIGHT + 2) * TILE_SIZE, LEVEL_WIDTH * TILE_SIZE, (LEVEL_HEIGHT + 3) * TILE_SIZE, 0, 0);

    // tiles to the left and right of text bar
    rdpq_sprite_blit(zed_sprite, 0, (LEVEL_HEIGHT + 1) * TILE_SIZE, NULL);
    rdpq_sprite_blit(zed_sprite, (LEVEL_WIDTH - 1) * TILE_SIZE, (LEVEL_HEIGHT + 1) * TILE_SIZE, NULL);

    // render text bar
    if (g.train_explosion_anim == ANIM_FINISHED)
    {
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_GREEN}, FONT_BUILTIN_DEBUG_MONO, 4.75 * TILE_SIZE, 13.75 * TILE_SIZE,
            "Press ^03START ^01to restart level");
    }
    else
    {
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_GREEN}, FONT_BUILTIN_DEBUG_MONO, 1.25 * TILE_SIZE, 13.75 * TILE_SIZE,
            "SCORE: ^02%i0", g.game_score);
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_RED}, FONT_BUILTIN_DEBUG_MONO, 6.25 * TILE_SIZE, 13.75 * TILE_SIZE,
            "GET: ^02%02i ^03HAVE: ^02%02i", g.items_to_collect, g.items_collected);
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_GREEN}, FONT_BUILTIN_DEBUG_MONO, 13.25 * TILE_SIZE, 13.75 * TILE_SIZE,
            "LEVEL ^02%02i^01: ^02%s", g.current_level_id, g.current_level.name);
    }
}

void vlak_render_level(vlak_level_t* level)
{
    rdpq_set_mode_standard();

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
                    case ANIM_REVERSE:
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
                    case ANIM_REVERSE:
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
                    blitparms = (rdpq_blitparms_t) {.cx = sprite->width / 2, .cy = sprite->height / 2, .theta = 1.57079633f, .flip_y = true};
                    break;
                case JOYPAD_8WAY_DOWN:
                    tile_x = tile_x + (sprite->width / 2);
                    tile_y = tile_y  + (sprite->height / 2);
                    blitparms = (rdpq_blitparms_t) {.cx = sprite->width / 2, .cy = sprite->height / 2, .theta = -1.57079633f, .flip_y = true};
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

void vlak_render_title_screen()
{
    rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_WHITE}, FONT_BUILTIN_DEBUG_MONO, 1.25 * TILE_SIZE, 10.75 * TILE_SIZE,
        "VLAK 64 by the_kovic    Made with Libdragon SDK");
    rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_WHITE}, FONT_BUILTIN_DEBUG_MONO, 1.25 * TILE_SIZE, 11.75 * TILE_SIZE,
        "Based on free game by Miroslav Nemecek    ^02\u00A92024");
    rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_GREEN}, FONT_BUILTIN_DEBUG_MONO, 6.5 * TILE_SIZE, 13.75 * TILE_SIZE,
        "Press ^03START ^01button");
}

void vlak_render_transition()
{
    rdpq_set_mode_standard();

    sprite_t* zed_sprite = vlak_sprite_array[ZED]->anim_frames[vlak_sprite_array[ZED]->nframes - 1];

    // upload sprite manually so we can use texture_rect
    rdpq_sprite_upload(TILE0, zed_sprite, &(rdpq_texparms_t) {.s = {.repeats = REPEAT_INFINITE}, .t = {.repeats = REPEAT_INFINITE}});

    rdpq_texture_rectangle(TILE0, 0, 0, LEVEL_WIDTH * TILE_SIZE, TILE_SIZE * (level_transit_line_count + 1), 0, 0);

    if (g.level_transit_anim == ANIM_GOING)
    {
        level_transit_line_count++;
        if (level_transit_line_count >= (LEVEL_HEIGHT + 2))
        {
            if (g.title_screen_playing)
            {
                // don't render input box in title screen mode
                g.level_transit_anim = ANIM_REVERSE;
                // load next level if we're leaving title screen
                if (g.title_screen_leaving)
                {
                    g.title_screen_playing = false;
                    g.title_screen_leaving = false;
                    
                    g.current_level_id++;
                    g.should_load_level = true;
                }
            }
            else
            {
                g.level_transit_anim = ANIM_FINISHED;
            }
        }
    }

    if (g.level_transit_anim == ANIM_FINISHED)
    {
        rdpq_set_mode_fill(RGBA32(0, 0, 0, 255));
        rdpq_fill_rectangle(5 * TILE_SIZE, 6 * TILE_SIZE, 15 * TILE_SIZE, 7 * TILE_SIZE);
        rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_GREEN}, FONT_BUILTIN_DEBUG_MONO, 5.75 * TILE_SIZE, 6.75 * TILE_SIZE,
            "Press ^03START ^01to continue");
        if (g.current_level_id + 1 == LEVEL_MAX)
        {
            rdpq_set_mode_fill(RGBA32(0, 0, 0, 255));
            rdpq_fill_rectangle(5 * TILE_SIZE, 5 * TILE_SIZE, 15 * TILE_SIZE, 6 * TILE_SIZE);
            rdpq_text_printf(&(rdpq_textparms_t) {.style_id = TEXT_YELLOW}, FONT_BUILTIN_DEBUG_MONO, 5.25 * TILE_SIZE, 5.75 * TILE_SIZE,
                "Congratulations^01! ^02You win^01!");
        }
    }

    if (g.level_transit_anim == ANIM_REVERSE)
    {
        level_transit_line_count--;
        if (level_transit_line_count == 0)
        {
            g.level_transit_anim = ANIM_NOT_STARTED;
        }
    }
}

#if PROFILE
#include <malloc.h>

void profiler_draw()
{
    typedef struct mallinfo memory_info_t;
    memory_info_t mem_info = mallinfo();
    int width = display_get_width();
    int height = display_get_height();
    int ram_used = mem_info.uordblks - ((width * height * 2) - ((uint32_t) HEAP_START_ADDR) - 0x80000000 - 0x10000);

    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 20, 26, "FPS %2.2f", display_get_fps());
    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 196, 26, "RAM %iKB/%iKB", (ram_used / 1024), (get_memory_size() / 1024));
}
#else
void profiler_draw() {}
#endif

void vlak_render()
{
    rdpq_attach_clear(display_get(), NULL);

    rdpq_set_mode_standard();

    if (!g.title_screen_playing)
    {
        vlak_render_border();
    }

    vlak_render_level(&g.current_level);

    if (g.title_screen_playing)
    {
        vlak_render_title_screen();
    }

    if (g.level_transit_anim != ANIM_NOT_STARTED)
    {
        vlak_render_transition();
    }

    profiler_draw();

    rdpq_detach_show();
}