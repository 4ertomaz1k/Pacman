#ifndef GHOST_H
#define GHOST_H

#include "types.h"

void ghosts_init(Ghost ghosts[GHOST_COUNT]);
void ghosts_update(Game *g);
void draw_ghost(SDL_Renderer *r, const Ghost *gh, int tick);
void draw_ghosts(SDL_Renderer *r, Ghost ghosts[GHOST_COUNT], int tick);

#endif
