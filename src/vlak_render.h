#ifndef VLAK_RENDER_H
#define VLAK_RENDER_H

#include "vlak_levels.h"

void vlak_text_init();

void vlak_render_border();

void vlak_render_level(vlak_level_t* level);

void vlak_render_title_screen();

void vlak_render_transition();

void vlak_render();

#endif