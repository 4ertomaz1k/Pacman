#include "pacman.h"
#include "map.h"
#include "sound.h"

void map_try_spawn_fruit(Map *m);

void pacman_init(Pacman *p) {
    p->x = p->px = p->visual_x = 14.0f;
    p->y = p->py = p->visual_y = 23.0f;
    p->dir             = DIR_LEFT;
    p->next_dir        = DIR_LEFT;
    p->move_timer      = 0;
    p->anim_frame      = 0;
    p->anim_timer      = 0;
    p->speed           = PACMAN_SPEED_BASE;
    p->just_teleported = 0;
}

void pacman_update(Game *g) {
    Pacman *p = &g->pacman;
    Map    *m = &g->map;

    p->move_timer++;

    /* Интерполяция для плавного движения */
    float t = (float)p->move_timer / (float)p->speed;
    if (t > 1.0f) t = 1.0f;
    p->visual_x = p->px + (p->x - p->px) * t;
    p->visual_y = p->py + (p->y - p->py) * t;

    if (p->move_timer < p->speed) return;
    p->move_timer = 0;

    /* Анимация рта */
    p->anim_timer++;
    if (p->anim_timer >= 3) {
        p->anim_timer = 0;
        p->anim_frame = 1 - p->anim_frame;
    }

    int cx = (int)roundf(p->x);
    int cy = (int)roundf(p->y);

    /* Пробуем сменить направление */
    {
        int dx, dy;
        dir_to_delta(p->next_dir, &dx, &dy);
        if (can_enter(m, cx+dx, cy+dy))
            p->dir = p->next_dir;
    }

    int dx, dy;
    dir_to_delta(p->dir, &dx, &dy);
    int nx = cx + dx;
    int ny = cy + dy;

    /* Всегда обновляем px/py перед шагом —
       это фиксирует баг "дрожания" у стены */
    p->px = p->x;
    p->py = p->y;

    /* Портал */
    if (!p->just_teleported && cy == PORTAL_ROW && nx < 0) {
        p->x  = (float)(MAP_COLS - 1);
        p->y  = (float)PORTAL_ROW;
        /* px/py уже = старая позиция, visual тоже ставим на новую
           чтобы не было пролёта через весь экран */
        p->px = p->x; p->py = p->y;
        p->visual_x = p->x; p->visual_y = p->y;
        p->move_timer = p->speed; /* пропускаем ожидание — сразу следующий шаг */
        p->just_teleported = 1;
    }
    else if (!p->just_teleported && cy == PORTAL_ROW && nx >= MAP_COLS) {
        p->x  = 0.0f;
        p->y  = (float)PORTAL_ROW;
        p->px = p->x; p->py = p->y;
        p->visual_x = p->x; p->visual_y = p->y;
        p->move_timer = p->speed;
        p->just_teleported = 1;
    }
    else {
        p->just_teleported = 0;
        if (can_enter(m, nx, ny)) {
            p->x = (float)nx;
            p->y = (float)ny;
        }
        /* Если стена — x/y не меняются, px/py уже = x/y,
           visual будет стоять на месте. Всё корректно. */
    }

    /* Съедаем ячейку */
    int px2 = (int)roundf(p->x);
    int py2 = (int)roundf(p->y);
    if (px2 < 0 || px2 >= MAP_COLS || py2 < 0 || py2 >= MAP_ROWS) return;
    Uint8 cell = m->cells[py2][px2];

    if (cell == CELL_DOT) {
        m->cells[py2][px2] = CELL_EMPTY;
        m->eaten_dots++;
        g->gs.score += SCORE_DOT;
        int lvl = m->eaten_dots / SPEED_UP_EVERY;
        p->speed = PACMAN_SPEED_BASE - lvl;
        if (p->speed < SPEED_MIN) p->speed = SPEED_MIN;
    }
    else if (cell == CELL_ENERGIZER) {
        m->cells[py2][px2] = CELL_EMPTY;
        m->eaten_dots++;
        g->gs.score += SCORE_ENERGIZER;
        g->gs.energizer_timer = ENERGIZER_TICKS;
        for (int i = 0; i < GHOST_COUNT; i++)
            if (g->ghosts[i].state != GHOST_DEAD)
                g->ghosts[i].state = GHOST_SCARED;
    }
    else if (cell==CELL_FRUIT_A || cell==CELL_FRUIT_B || cell==CELL_FRUIT_C) {
        int score = (cell==CELL_FRUIT_A) ? SCORE_FRUIT_A :
                    (cell==CELL_FRUIT_B) ? SCORE_FRUIT_B : SCORE_FRUIT_C;
        for (int i = 0; i < FRUIT_MAX; i++)
            if (m->fruits[i].col==px2 && m->fruits[i].row==py2)
                m->fruits[i].timer = 0;
        m->cells[py2][px2] = CELL_EMPTY;
        g->gs.score += score;
        sound_play_fruit();
    }

    if (m->eaten_dots >= m->next_fruit_at) {
        map_try_spawn_fruit(m);
        m->next_fruit_at += FRUIT_REPEAT;
    }
}

void draw_pacman(SDL_Renderer *r, const Pacman *p) {
    int cx = (int)(p->visual_x * CELL_SIZE + CELL_SIZE/2);
    int cy = (int)(p->visual_y * CELL_SIZE + CELL_SIZE/2);
    int radius = CELL_SIZE/2 - 1;

    float mouth_deg = p->anim_frame ? 5.0f : 35.0f;
    float mouth_rad = mouth_deg * (float)M_PI / 180.0f;
    float base_angle = 0.0f;
    if (p->dir == DIR_LEFT)  base_angle = (float)M_PI;
    if (p->dir == DIR_UP)    base_angle = 3.0f*(float)M_PI/2.0f;
    if (p->dir == DIR_DOWN)  base_angle = (float)M_PI/2.0f;

    SDL_SetRenderDrawColor(r, 255, 220, 0, 255);
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx*dx + dy*dy > radius*radius) continue;
            float angle = atan2f((float)dy, (float)dx);
            float diff  = angle - base_angle;
            while (diff >  (float)M_PI) diff -= 2.0f*(float)M_PI;
            while (diff < -(float)M_PI) diff += 2.0f*(float)M_PI;
            if (fabsf(diff) < mouth_rad) continue;
            SDL_RenderDrawPoint(r, cx+dx, cy+dy);
        }
    }
    int ex = cx + (int)(radius*0.35f*cosf(base_angle-(float)M_PI/2.0f));
    int ey = cy + (int)(radius*0.35f*sinf(base_angle-(float)M_PI/2.0f));
    SDL_SetRenderDrawColor(r, 10, 10, 10, 255);
    SDL_Rect eye = {ex, ey, 2, 2};
    SDL_RenderFillRect(r, &eye);
}
