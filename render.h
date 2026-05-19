#ifndef RENDER_H
#define RENDER_H

#include "types.h"

void render_text(SDL_Renderer *r, TTF_Font *font,
                 const char *text, int x, int y,
                 Uint8 R, Uint8 G, Uint8 B);
void render_text_centered(SDL_Renderer *r, TTF_Font *font,
                          const char *text, int y,
                          Uint8 R, Uint8 G, Uint8 B);
void draw_hud(SDL_Renderer *r, TTF_Font *font,
              const Map *m, int score);
void draw_overlay(SDL_Renderer *r, TTF_Font *font_big,
                  TTF_Font *font_small,
                  const char *line1, const char *line2);
void draw_menu(Game *g);
void draw_scores_screen(SDL_Renderer *r, TTF_Font *font_big,
                        TTF_Font *font_small,
                        const ScoreBoard *sb, int current_score);
void game_render(Game *g);

#endif
