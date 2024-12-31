#include <libdragon.h>

#include "vlak_game.h"
#include "vlak_sprites.h"
#include "vlak_sfx.h"

int main()
{   
    joypad_init();
    dfs_init(DFS_DEFAULT_LOCATION);
    rdpq_init();

    //debug_init_isviewer();

    audio_init(44100, 4);
    mixer_init(SFX_CHANNEL_COUNT + MUSIC_CHANNEL_COUNT);

    vlak_sprite_init();

    vlak_sfx_init();

    vlak_text_init();

    vlak_game_init();

    vlak_set_display();

    while (1)
    {
        if (g.should_load_level)
        {
            vlak_load_level();
        }

        mixer_try_play();

        vlak_process_input();

        mixer_try_play();

        if (g.title_screen_playing && (g.anim_counter % 5 == 0))
        {
            vlak_title_screen_tick();
        }

        if (!g.menu_open && g.train_moving && (g.anim_counter % 5 == 0))
        {
            vlak_collision_check();
        }

        mixer_try_play();

        vlak_render();

        mixer_try_play();

        g.anim_counter++;
    }
}