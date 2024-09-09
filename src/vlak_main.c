#include <libdragon.h>

#include "vlak_game.h"
#include "vlak_sprites.h"
#include "vlak_sfx.h"
#include "vlak_render.h"

int main()
{   
    joypad_init();
    dfs_init(DFS_DEFAULT_LOCATION);
    rdpq_init();

    audio_init(44100, 4);
    mixer_init(8);

    display_init((resolution_t) {.width = 320, .height = 240}, DEPTH_32_BPP, 3, GAMMA_NONE, FILTERS_DISABLED);

    display_set_fps_limit(12);

    vlak_sprite_init();

    vlak_sfx_init();

    vlak_text_init();

    vlak_game_init();

    while (1)
    {
        if (g.should_load_level)
        {
            vlak_load_level();
        }

        mixer_try_play();

        vlak_process_input();

        if (g.title_screen_playing && (g.anim_counter % 5 == 0))
        {
            vlak_title_screen_tick();
        }

        if (g.train_moving && (g.anim_counter % 5 == 0))
        {
            vlak_collision_check();
        }

        vlak_render();

        mixer_try_play();

        g.anim_counter++;
    }
}