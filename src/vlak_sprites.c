#include "vlak_sprites.h"

static vlak_sprite_t krystal_sprite;
static vlak_sprite_t koruna_sprite;
static vlak_sprite_t strom_sprite;
static vlak_sprite_t jablko_sprite;
static vlak_sprite_t krava_sprite;
static vlak_sprite_t tresne_sprite;
static vlak_sprite_t rybnik_sprite;
static vlak_sprite_t zirafa_sprite;
static vlak_sprite_t zmrzlina_sprite;
static vlak_sprite_t dort_sprite;
static vlak_sprite_t pocitac_sprite;
static vlak_sprite_t auto_sprite;
static vlak_sprite_t balon_sprite;
static vlak_sprite_t budik_sprite;
static vlak_sprite_t slon_sprite;
static vlak_sprite_t vino_sprite;
static vlak_sprite_t penize_sprite;
static vlak_sprite_t letadlo_sprite;

static vlak_sprite_t vrata_sprite;

static vlak_sprite_t zed_sprite;

static vlak_sprite_t vlak_boom_sprite;
static vlak_sprite_t vlak_dead_sprite;
static vlak_sprite_t vlak_sprite;

static vlak_sprite_t vagon_krystal_sprite;
static vlak_sprite_t vagon_koruna_sprite;
static vlak_sprite_t vagon_strom_sprite;
static vlak_sprite_t vagon_jablko_sprite;
static vlak_sprite_t vagon_krava_sprite;
static vlak_sprite_t vagon_tresne_sprite;
static vlak_sprite_t vagon_rybnik_sprite;
static vlak_sprite_t vagon_zirafa_sprite;
static vlak_sprite_t vagon_zmrzlina_sprite;
static vlak_sprite_t vagon_dort_sprite;
static vlak_sprite_t vagon_pocitac_sprite;
static vlak_sprite_t vagon_auto_sprite;
static vlak_sprite_t vagon_balon_sprite;
static vlak_sprite_t vagon_budik_sprite;
static vlak_sprite_t vagon_slon_sprite;
static vlak_sprite_t vagon_vino_sprite;
static vlak_sprite_t vagon_penize_sprite;
static vlak_sprite_t vagon_letadlo_sprite;

vlak_sprite_t* vlak_sprite_array[NUM_LEVEL_ELEMENTS] = {
    // Empty space in level, has no sprite.
    NULL,

    &krystal_sprite,
    &koruna_sprite,
    &strom_sprite,
    &jablko_sprite,
    &krava_sprite,
    &tresne_sprite,
    &rybnik_sprite,
    &zirafa_sprite,
    &zmrzlina_sprite,
    &dort_sprite,
    &pocitac_sprite,
    &auto_sprite,
    &balon_sprite,
    &budik_sprite,
    &slon_sprite,
    &vino_sprite,
    &penize_sprite,
    &letadlo_sprite,

    &vrata_sprite,
    &zed_sprite,

    &vlak_boom_sprite,
    &vlak_dead_sprite,
    &vlak_sprite,

    &vagon_krystal_sprite,
    &vagon_koruna_sprite,
    &vagon_strom_sprite,
    &vagon_jablko_sprite,
    &vagon_krava_sprite,
    &vagon_tresne_sprite,
    &vagon_rybnik_sprite,
    &vagon_zirafa_sprite,
    &vagon_zmrzlina_sprite,
    &vagon_dort_sprite,
    &vagon_pocitac_sprite,
    &vagon_auto_sprite,
    &vagon_balon_sprite,
    &vagon_budik_sprite,
    &vagon_slon_sprite,
    &vagon_vino_sprite,
    &vagon_penize_sprite,
    &vagon_letadlo_sprite
};

void vlak_sprite_init()
{
    krystal_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/krystal0.sprite"),
            sprite_load("rom:/krystal1.sprite"),
            sprite_load("rom:/krystal2.sprite")
        }
    };
    koruna_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/koruna0.sprite"),
            sprite_load("rom:/koruna1.sprite"),
            sprite_load("rom:/koruna2.sprite")
        }
    };
    strom_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/strom0.sprite"),
            sprite_load("rom:/strom1.sprite"),
            sprite_load("rom:/strom2.sprite")
        }
    };
    jablko_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/jablko0.sprite"),
            sprite_load("rom:/jablko1.sprite"),
            sprite_load("rom:/jablko2.sprite")
        }
    };
    krava_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/krava0.sprite"),
            sprite_load("rom:/krava1.sprite"),
            sprite_load("rom:/krava2.sprite")
        }
    };
    tresne_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/tresne0.sprite"),
            sprite_load("rom:/tresne1.sprite"),
            sprite_load("rom:/tresne2.sprite")
        }
    };
    rybnik_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/rybnik0.sprite"),
            sprite_load("rom:/rybnik1.sprite"),
            sprite_load("rom:/rybnik2.sprite")
        }
    };
    zirafa_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/zirafa0.sprite"),
            sprite_load("rom:/zirafa1.sprite"),
            sprite_load("rom:/zirafa2.sprite")
        }
    };
    zmrzlina_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/zmrzlina0.sprite"),
            sprite_load("rom:/zmrzlina1.sprite"),
            sprite_load("rom:/zmrzlina2.sprite")
        }
    };
    dort_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/dort0.sprite"),
            sprite_load("rom:/dort1.sprite"),
            sprite_load("rom:/dort2.sprite")
        }
    };
    pocitac_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/pocitac0.sprite"),
            sprite_load("rom:/pocitac1.sprite"),
            sprite_load("rom:/pocitac2.sprite")
        }
    };
    auto_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/auto0.sprite"),
            sprite_load("rom:/auto1.sprite"),
            sprite_load("rom:/auto2.sprite")
        }
    };
    balon_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/balon0.sprite"),
            sprite_load("rom:/balon1.sprite"),
            sprite_load("rom:/balon2.sprite")
        }
    };
    budik_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/budik0.sprite"),
            sprite_load("rom:/budik1.sprite"),
            sprite_load("rom:/budik2.sprite")
        }
    };
    slon_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/slon0.sprite"),
            sprite_load("rom:/slon1.sprite"),
            sprite_load("rom:/slon2.sprite")
        }
    };
    vino_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/vino0.sprite"),
            sprite_load("rom:/vino1.sprite"),
            sprite_load("rom:/vino2.sprite")
        }
    };
    penize_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/penize0.sprite"),
            sprite_load("rom:/penize1.sprite"),
            sprite_load("rom:/penize2.sprite")
        }
    };
    letadlo_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/letadlo0.sprite"),
            sprite_load("rom:/letadlo1.sprite"),
            sprite_load("rom:/letadlo2.sprite")
        }
    };

    vrata_sprite = (vlak_sprite_t) {
        .nframes = 6,
        .anim_frames = {
            sprite_load("rom:/vrata0.sprite"),
            sprite_load("rom:/vrata1.sprite"),
            sprite_load("rom:/vrata2.sprite"),
            sprite_load("rom:/vrata3.sprite"),
            sprite_load("rom:/vrata4.sprite"),
            sprite_load("rom:/vrata5.sprite"),
        }
    };

    zed_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/zed0.sprite")}};

    vlak_boom_sprite = (vlak_sprite_t) {
        .nframes = 6,
        .anim_frames = {
            sprite_load("rom:/vlak_boom0.sprite"),
            sprite_load("rom:/vlak_boom1.sprite"),
            sprite_load("rom:/vlak_boom2.sprite"),
            sprite_load("rom:/vlak_boom3.sprite"),
            sprite_load("rom:/vlak_boom4.sprite"),
            sprite_load("rom:/vlak_boom5.sprite"),
        }
    };
    vlak_dead_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/vlak_dead0.sprite"),
            sprite_load("rom:/vlak_dead1.sprite"),
            sprite_load("rom:/vlak_dead2.sprite")
        }
    };
    vlak_sprite = (vlak_sprite_t) {
        .nframes = 3,
        .anim_frames = {
            sprite_load("rom:/vlak0.sprite"),
            sprite_load("rom:/vlak1.sprite"),
            sprite_load("rom:/vlak2.sprite")
        }
    };

    vagon_krystal_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_krystal0.sprite")}};
    vagon_koruna_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_koruna0.sprite")}};
    vagon_strom_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_strom0.sprite")}};
    vagon_jablko_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_jablko0.sprite")}};
    vagon_krava_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_krava0.sprite")}};
    vagon_tresne_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_tresne0.sprite")}};
    vagon_rybnik_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_rybnik0.sprite")}};
    vagon_zirafa_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_zirafa0.sprite")}};
    vagon_zmrzlina_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_zmrzlina0.sprite")}};
    vagon_dort_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_dort0.sprite")}};
    vagon_pocitac_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_pocitac0.sprite")}};
    vagon_auto_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_auto0.sprite")}};
    vagon_balon_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_balon0.sprite")}};
    vagon_budik_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_budik0.sprite")}};
    vagon_slon_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_slon0.sprite")}};
    vagon_vino_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_vino0.sprite")}};
    vagon_penize_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_penize0.sprite")}};
    vagon_letadlo_sprite = (vlak_sprite_t) {.nframes = 1, .anim_frames = {sprite_load("rom:/vagon_letadlo0.sprite")}};
}
