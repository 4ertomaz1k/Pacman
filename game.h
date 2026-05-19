#ifndef GAME_H
#define GAME_H

#include "types.h"

void game_init(Game *g);
void game_update(Game *g);
void game_handle_key(Game *g, SDL_Keycode key);
void game_handle_click(Game *g, int mx, int my);

#endif
