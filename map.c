#include "map.h"

static const Uint8 BASE_MAP[MAP_ROWS][MAP_COLS] = {
//       0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 
       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       { 1, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 3, 3, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 3, 3, 3, 3, 3, 3, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 3, 3, 3, 3, 3, 3, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
       { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
       { 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1},
       { 1, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 1},
       { 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
       { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void map_init(Map *m) {
    m->total_dots    = 0;
    m->eaten_dots    = 0;
    m->next_fruit_at = FRUIT_APPEARS_AT;
    for (int i = 0; i < FRUIT_MAX; i++) m->fruits[i].timer = 0;
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++) {
            m->cells[r][c] = BASE_MAP[r][c];
            if (BASE_MAP[r][c] == CELL_DOT || BASE_MAP[r][c] == CELL_ENERGIZER)
                m->total_dots++;
        }
}

int can_enter(const Map *m, int col, int row) {
    if (row < 0 || row >= MAP_ROWS) return 0;
    if (row == PORTAL_ROW) {
        if (col < 0 || col >= MAP_COLS) return 1;
        return m->cells[row][col] != CELL_WALL;
    }
    if (col < 0 || col >= MAP_COLS) return 0;
    return m->cells[row][col] != CELL_WALL;
}

int can_enter_ghost(const Map *m, int col, int row) {
    return can_enter(m, col, row);
}

void dir_to_delta(int dir, int *dx, int *dy) {
    *dx = 0; *dy = 0;
    if (dir == DIR_LEFT)  *dx = -1;
    if (dir == DIR_RIGHT) *dx =  1;
    if (dir == DIR_UP)    *dy = -1;
    if (dir == DIR_DOWN)  *dy =  1;
}

// Найти случайную пустую клетку для фрукта
static int fruit_spawn(Map *m, int *out_col, int *out_row) {
    // Пробуем до 50 раз найти свободную клетку
    for (int attempt = 0; attempt < 50; attempt++) {
        int c = 1 + rand() % (MAP_COLS - 2);
        int r = 1 + rand() % (MAP_ROWS - 2);
        if (m->cells[r][c] == CELL_EMPTY) {
            *out_col = c; *out_row = r;
            return 1;
        }
    }
    return 0;
}

void map_update(Map *m) {
    //Обновляем таймеры фруктов
    for (int i = 0; i < FRUIT_MAX; i++) {
        if (m->fruits[i].timer > 0) {
            m->fruits[i].timer--;
            if (m->fruits[i].timer == 0)
                m->cells[m->fruits[i].row][m->fruits[i].col] = CELL_EMPTY;
        }
    }
}

void map_try_spawn_fruit(Map *m) {
    //Ищем свободный слот
    int slot = -1;
    for (int i = 0; i < FRUIT_MAX; i++)
        if (m->fruits[i].timer == 0) { slot = i; break; }
    if (slot < 0) return;

    int col, row;
    if (!fruit_spawn(m, &col, &row)) return;

    //Случайный тип фрукта
    int types[3] = {CELL_FRUIT_A, CELL_FRUIT_B, CELL_FRUIT_C};
    int type = types[rand() % 3];

    m->fruits[slot].col   = col;
    m->fruits[slot].row   = row;
    m->fruits[slot].timer = FRUIT_LIFETIME;
    m->fruits[slot].type  = type;
    m->cells[row][col]    = type;
}

//ОТРИСОВКА
static void draw_wall(SDL_Renderer *r, int x, int y, int w, int h) {
    SDL_Rect rect = {x+1, y+1, w-2, h-2};
    SDL_SetRenderDrawColor(r, 33, 33, 200, 255);
    SDL_RenderFillRect(r, &rect);
    SDL_SetRenderDrawColor(r, 70, 70, 255, 255);
    SDL_RenderDrawRect(r, &rect);
}

static void draw_dot(SDL_Renderer *r, int cx, int cy) {
    SDL_Rect dot = {cx-2, cy-2, 4, 4};
    SDL_SetRenderDrawColor(r, 255, 200, 100, 255);
    SDL_RenderFillRect(r, &dot);
}

static void draw_energizer(SDL_Renderer *r, int cx, int cy,
                            int energizer_timer, int tick) {
    if (energizer_timer > 0 && energizer_timer < ENERGIZER_BLINK && (tick/8)%2)
        return;
    int rad = CELL_SIZE/2 - 3;
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    for (int dy=-rad; dy<=rad; dy++)
        for (int dx=-rad; dx<=rad; dx++)
            if (dx*dx+dy*dy<=rad*rad)
                SDL_RenderDrawPoint(r, cx+dx, cy+dy);
}

//Яблоко
static void draw_apple(SDL_Renderer *r, int cx, int cy) {
    int rad = CELL_SIZE/2 - 4;
    SDL_SetRenderDrawColor(r, 220, 30, 30, 255);
    for (int dy=-rad; dy<=rad; dy++)
        for (int dx=-rad; dx<=rad; dx++)
            if (dx*dx+dy*dy<=rad*rad)
                SDL_RenderDrawPoint(r, cx+dx, cy+dy);
    SDL_SetRenderDrawColor(r, 40, 160, 40, 255);
    SDL_RenderDrawLine(r, cx, cy-rad, cx+3, cy-rad-4);
}

//Банан
static void draw_banana(SDL_Renderer *r, int cx, int cy) {
    SDL_SetRenderDrawColor(r, 255, 215, 0, 255);
    for (int layer = -1; layer <= 1; layer++) {
        for (int i = 0; i < 18; i++) {
            float a = (float)i / 17.0f * (float)M_PI;
            float rx = 7.5f;
            float ry = 4.0f;
            float tilt = ((float)i / 17.0f) * 2.5f;
            int x = cx - (int)(rx * cosf(a));
            int y = cy + (int)(ry * sinf(a)) + (int)tilt + layer;
            SDL_RenderDrawPoint(r, x, y);
            SDL_RenderDrawPoint(r, x+1, y);
        }
    }
    SDL_SetRenderDrawColor(r, 180, 140, 0, 255);
    SDL_Rect t1 = {cx-9, cy+1, 3, 2};
    SDL_Rect t2 = {cx+6, cy+3, 3, 2};
    SDL_RenderFillRect(r, &t1);
    SDL_RenderFillRect(r, &t2);
}

//Вишня
static void draw_cherry(SDL_Renderer *r, int cx, int cy) {
    int rad = CELL_SIZE/2 - 5;
    SDL_SetRenderDrawColor(r, 200, 0, 0, 255);
    for (int dy=-rad; dy<=rad; dy++)
        for (int dx=-rad; dx<=rad; dx++)
            if (dx*dx+dy*dy<=rad*rad) {
                SDL_RenderDrawPoint(r, cx+dx-3, cy+dy+1);
                SDL_RenderDrawPoint(r, cx+dx+3, cy+dy+1);
            }
    SDL_SetRenderDrawColor(r, 40, 140, 40, 255);
    SDL_RenderDrawLine(r, cx-3, cy-rad, cx, cy-rad-4);
    SDL_RenderDrawLine(r, cx+3, cy-rad, cx, cy-rad-4);
}

void draw_map(SDL_Renderer *r, const Map *m, int energizer_timer, int tick) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            int x  = col * CELL_SIZE;
            int y  = row * CELL_SIZE;
            int cx = x + CELL_SIZE/2;
            int cy = y + CELL_SIZE/2;
            switch (m->cells[row][col]) {
                case CELL_WALL:      draw_wall(r, x, y, CELL_SIZE, CELL_SIZE); break;
                case CELL_DOT:       draw_dot(r, cx, cy);                       break;
                case CELL_ENERGIZER: draw_energizer(r,cx,cy,energizer_timer,tick); break;
                case CELL_FRUIT_A:   draw_apple(r, cx, cy);   break;
                case CELL_FRUIT_B:   draw_banana(r, cx, cy);  break;
                case CELL_FRUIT_C:   draw_cherry(r, cx, cy);  break;
                default: break;
            }
        }
    }
}
