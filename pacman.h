#ifndef PACMAN_H
#define PACMAN_H

#include "types.h"


void pacman_init(Pacman *p);
void pacman_update(Game *g);
void draw_pacman(SDL_Renderer *r, const Pacman *p);

#endif
