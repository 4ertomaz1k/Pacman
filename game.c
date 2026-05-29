#include "game.h"
#include "map.h"
#include "pacman.h"
#include "ghost.h"
#include "scores.h"
#include "sound.h"

void game_init(Game *g) {
    map_init(&g->map);
    pacman_init(&g->pacman);
    ghosts_init(g->ghosts);
    g->gs.score           = 0;
    g->gs.energizer_timer = 0;
    g->state = STATE_PLAYING;
    g->tick  = 0;
}

void game_update(Game *g) {
    if (g->state != STATE_PLAYING) return;

    int dots_before = g->map.eaten_dots;
    int en_before   = g->gs.energizer_timer;

    pacman_update(g);
    ghosts_update(g);
    map_update(&g->map);

    if (g->map.eaten_dots > dots_before)           sound_play_dot();
    if (g->gs.energizer_timer > 0 && en_before==0) sound_play_energizer();
    if (g->state == STATE_LOSE)                    sound_play_death();

    if (g->map.eaten_dots >= g->map.total_dots)
        g->state = STATE_WIN;

    g->tick++;
}


void game_handle_key(Game *g, SDL_Keycode key) {

    //ESC — универсальный выход в меню (обрабатывается в main.c)
    if (g->state == STATE_MENU) {
        if (key == SDLK_RETURN || key == SDLK_SPACE)
            game_init(g);
        if (key == SDLK_l)
            g->state = STATE_SCORES;
        if (key == SDLK_r)
            game_init(g);
        return;
    }

    if (g->state == STATE_PLAYING) {
        if (key == SDLK_r)
            game_init(g);
        if (key == SDLK_l)
            g->state = STATE_SCORES;
        switch (key) {
            case SDLK_LEFT:  case SDLK_a: g->pacman.next_dir = DIR_LEFT;  break;
            case SDLK_RIGHT: case SDLK_d: g->pacman.next_dir = DIR_RIGHT; break;
            case SDLK_UP:    case SDLK_w: g->pacman.next_dir = DIR_UP;    break;
            case SDLK_DOWN:  case SDLK_s: g->pacman.next_dir = DIR_DOWN;  break;
            default: break;
        }
        return;
    }

    if (g->state == STATE_WIN || g->state == STATE_LOSE) {
        if (key == SDLK_r) {
            game_init(g);
        }
        if (key == SDLK_l) {
            scores_save(&g->scoreboard, g->gs.score);
            g->state = STATE_SCORES;
        }
        return;
    }

    if (g->state == STATE_SCORES) {
        if (key == SDLK_r)
            game_init(g);
        return;
    }
}

void game_handle_click(Game *g, int mx, int my) {
    if (g->state != STATE_MENU) return;
    if (mx>=g->btn_play.x && mx<=g->btn_play.x+g->btn_play.w &&
        my>=g->btn_play.y && my<=g->btn_play.y+g->btn_play.h) {
        game_init(g); return;
    }
    if (mx>=g->btn_scores.x && mx<=g->btn_scores.x+g->btn_scores.w &&
        my>=g->btn_scores.y && my<=g->btn_scores.y+g->btn_scores.h) {
        g->state = STATE_SCORES; return;
    }
}
