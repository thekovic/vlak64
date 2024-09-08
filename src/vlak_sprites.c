#include "vlak_sprites.h"

static vlak_sprite_t krystal_sprite;
static vlak_sprite_t koruna_sprite;

static vlak_sprite_t vrata_sprite;

static vlak_sprite_t zed_sprite;

static vlak_sprite_t vlak_sprite;
static vlak_sprite_t vlak_dead_sprite;

static vlak_sprite_t vagon_krystal_sprite;
static vlak_sprite_t vagon_koruna_sprite;

vlak_sprite_t* vlak_sprite_array[NUM_LEVEL_ELEMENTS] = {
    // Empty space in level, has no sprite.
    NULL,

    &krystal_sprite,
    &koruna_sprite,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,

    &vrata_sprite,
    &zed_sprite,

    &vlak_dead_sprite,
    &vlak_sprite,

    &vagon_krystal_sprite,
    &vagon_koruna_sprite
};

void vlak_sprite_init()
{
    krystal_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .animation_frames = {
            sprite_load("rom:/krystal0.sprite"),
            sprite_load("rom:/krystal1.sprite"),
            sprite_load("rom:/krystal2.sprite")
        }
    };
    koruna_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .animation_frames = {
            sprite_load("rom:/koruna0.sprite"),
            sprite_load("rom:/koruna1.sprite"),
            sprite_load("rom:/koruna2.sprite")
        }
    };

    vrata_sprite = (vlak_sprite_t) {
        .nframes = 6,
        .animation_frames = {
            sprite_load("rom:/vrata0.sprite"),
            sprite_load("rom:/vrata1.sprite"),
            sprite_load("rom:/vrata2.sprite"),
            sprite_load("rom:/vrata3.sprite"),
            sprite_load("rom:/vrata4.sprite"),
            sprite_load("rom:/vrata5.sprite"),
        }
    };

    zed_sprite = (vlak_sprite_t) {
        .nframes = 1,
        .animation_frames = {
            sprite_load("rom:/zed0.sprite")
        }
    };

    vlak_dead_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .animation_frames = {
            sprite_load("rom:/vlak_dead0.sprite"),
            sprite_load("rom:/vlak_dead1.sprite"),
            sprite_load("rom:/vlak_dead2.sprite")
        }
    };
    vlak_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .animation_frames = {
            sprite_load("rom:/vlak0.sprite"),
            sprite_load("rom:/vlak1.sprite"),
            sprite_load("rom:/vlak2.sprite")
        }
    };

    vagon_krystal_sprite = (vlak_sprite_t) {
        .nframes = 1,
        .animation_frames = {
            sprite_load("rom:/vagon_krystal0.sprite")
        }
    };
    vagon_koruna_sprite = (vlak_sprite_t) {
        .nframes = 1,
        .animation_frames = {
            sprite_load("rom:/vagon_koruna0.sprite")
        }
    };
}
