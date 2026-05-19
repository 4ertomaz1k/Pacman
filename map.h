#ifndef MAP_H
#define MAP_H

#include "types.h"

void map_init(Map *m);
int  can_enter(const Map *m, int col, int row);
int  can_enter_ghost(const Map *m, int col, int row);
void dir_to_delta(int dir, int *dx, int *dy);
void map_update(Map *m);
void map_try_spawn_fruit(Map *m);
void draw_map(SDL_Renderer *r, const Map *m, int energizer_timer, int tick);

#endif