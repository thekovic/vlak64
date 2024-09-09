#include <libdragon.h>

#include "vlak_game.h"
#include "vlak_sprites.h"
#include "vlak_render.h"

int main()
{   
    joypad_init();
    dfs_init(DFS_DEFAULT_LOCATION);
    rdpq_init();

    display_init((resolution_t) {.width = 320, .height = 240}, DEPTH_32_BPP, 3, GAMMA_NONE, FILTERS_DISABLED);

    display_set_fps_limit(12);

    vlak_sprite_init();

    vlak_text_init();

    vlak_game_init();

    while (1)
    {
        if (g.should_load_level)
        {
            vlak_load_level();
        }

        vlak_process_input();

        if (g.train_moving && (g.anim_counter % 5 == 0))
        {
            vlak_collision_check();
        }

        vlak_render();

        g.anim_counter++;
    }
}