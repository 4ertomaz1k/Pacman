//C:/msys64/ucrt64/bin/gcc.exe Pacman.c -o Pacman.exe -I C:/SDL2/include -I C:/SDL2_ttf/include -L C:/SDL2/lib/x64 -L C:/SDL2_ttf/lib -lSDL2main -lSDL2 -lSDL2_ttf -lm -mwindows

#include <SDL.h>
#undef main
#include <SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define CELL_SIZE    20
#define MAP_COLS     28
#define MAP_ROWS     32

#define SCREEN_W     (MAP_COLS * CELL_SIZE)
#define SCREEN_H     (MAP_ROWS * CELL_SIZE + 48)

#define FPS          60
#define FRAME_MS     (1000 / FPS)

#define PACMAN_SPEED   8
#define GHOST_SPEED    10

#define CELL_EMPTY   0
#define CELL_WALL    1
#define CELL_DOT     2
#define CELL_EMPTY2  3

#define DIR_NONE  0
#define DIR_LEFT  1
#define DIR_RIGHT 2
#define DIR_UP    3
#define DIR_DOWN  4

#define STATE_PLAYING  0
#define STATE_WIN      1
#define STATE_LOSE     2

#define FONT_PATH  "OpenSans.ttf"
#define FONT_HUD   18
#define FONT_OVL   32   
#define FONT_SUB   16 

static const Uint8 BASE_MAP[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,3,3,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,0,1,3,3,3,3,3,3,1,0,1,1,2,1,1,1,1,1,1},
    {0,0,0,0,0,0,2,0,0,0,1,3,3,3,3,3,3,1,0,0,0,2,0,0,0,0,0,0},
    {1,1,1,1,1,1,2,1,1,0,1,3,3,3,3,3,3,1,0,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};


typedef struct {
    float x, y;
    int   dir;
    int   next_dir;
    int   move_timer;
} Pacman;

typedef struct {
    float x, y;
    int   dir;
    int   move_timer;
    int   alive;
} Ghost;

typedef struct {
    Uint8 cells[MAP_ROWS][MAP_COLS];
    int   total_dots;
    int   eaten_dots;
} Map;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font_hud;
    TTF_Font     *font_ovl;
    TTF_Font     *font_sub;
    Map           map;
    Pacman        pacman;
    Ghost         ghost;
    int           state;
    int           tick;
} Game;


static int can_enter(const Map *m, int col, int row) {
    if (col < 0 || col >= MAP_COLS || row < 0 || row >= MAP_ROWS)
        return 0;
    return m->cells[row][col] != CELL_WALL;
}

static void dir_to_delta(int dir, int *dx, int *dy) {
    *dx = 0; *dy = 0;
    if (dir == DIR_LEFT)  *dx = -1;
    if (dir == DIR_RIGHT) *dx =  1;
    if (dir == DIR_UP)    *dy = -1;
    if (dir == DIR_DOWN)  *dy =  1;
}


static void map_init(Map *m) {
    m->total_dots = 0;
    m->eaten_dots = 0;
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++) {
            m->cells[r][c] = BASE_MAP[r][c];
            if (BASE_MAP[r][c] == CELL_DOT)
                m->total_dots++;
        }
}


static void pacman_init(Pacman *p) {
    p->x          = 14.0f;
    p->y          = 23.0f;
    p->dir        = DIR_LEFT;
    p->next_dir   = DIR_LEFT;
    p->move_timer = 0;
}

static void ghost_init(Ghost *g) {
    g->x          = 14.0f;
    g->y          = 13.0f;
    g->dir        = DIR_LEFT;
    g->move_timer = 0;
    g->alive      = 1;
}


static void move_pacman(Game *g) {
    Pacman *p = &g->pacman;
    Map    *m = &g->map;

    p->move_timer++;
    if (p->move_timer < PACMAN_SPEED) return;
    p->move_timer = 0;

    int cx = (int)roundf(p->x);
    int cy = (int)roundf(p->y);

    // Пробуем сменить направление 
    {
        int dx, dy;
        dir_to_delta(p->next_dir, &dx, &dy);
        if (can_enter(m, cx + dx, cy + dy))
            p->dir = p->next_dir;
    }

    // Шаг
    {
        int dx, dy;
        dir_to_delta(p->dir, &dx, &dy);
        int nx = cx + dx, ny = cy + dy;
        if (can_enter(m, nx, ny)) {
            p->x = (float)nx;
            p->y = (float)ny;
        }
    }

    // Съедаем точку
    int px = (int)roundf(p->x);
    int py = (int)roundf(p->y);
    if (m->cells[py][px] == CELL_DOT) {
        m->cells[py][px] = CELL_EMPTY;
        m->eaten_dots++;
    }
}

static void move_ghost(Game *g) {
    Ghost *gh = &g->ghost;
    Map   *m  = &g->map;

    if (!gh->alive) return;

    gh->move_timer++;
    if (gh->move_timer < GHOST_SPEED) return;
    gh->move_timer = 0;

    int cx = (int)roundf(gh->x);
    int cy = (int)roundf(gh->y);

    int dirs[4]     = {DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN};
    int opposite[5] = {DIR_NONE, DIR_RIGHT, DIR_LEFT, DIR_DOWN, DIR_UP};
    int valid[4];
    int n_valid = 0;

    for (int i = 0; i < 4; i++) {
        if (dirs[i] == opposite[gh->dir]) continue;
        int dx, dy;
        dir_to_delta(dirs[i], &dx, &dy);
        if (can_enter(m, cx + dx, cy + dy))
            valid[n_valid++] = dirs[i];
    }

    if (n_valid == 0) {
        gh->dir = opposite[gh->dir];
    } else {
        gh->dir = valid[rand() % n_valid];
    }

    {
        int dx, dy;
        dir_to_delta(gh->dir, &dx, &dy);
        int nx = cx + dx, ny = cy + dy;
        if (can_enter(m, nx, ny)) {
            gh->x = (float)nx;
            gh->y = (float)ny;
        }
    }
}

static void check_collisions(Game *g) {
    int px = (int)roundf(g->pacman.x), py = (int)roundf(g->pacman.y);
    int gx = (int)roundf(g->ghost.x),  gy = (int)roundf(g->ghost.y);

    if (g->ghost.alive && px == gx && py == gy)
        g->state = STATE_LOSE;

    if (g->map.eaten_dots >= g->map.total_dots)
        g->state = STATE_WIN;
}


// Отрисовка

static void draw_wall(SDL_Renderer *r, int x, int y, int w, int h) {
    SDL_Rect rect = {x+1, y+1, w-2, h-2};
    SDL_SetRenderDrawColor(r, 33, 33, 222, 255);
    SDL_RenderFillRect(r, &rect);
    SDL_SetRenderDrawColor(r, 80, 80, 255, 255);
    SDL_RenderDrawRect(r, &rect);
}

static void draw_dot(SDL_Renderer *r, int cx, int cy) {
    SDL_Rect dot = {cx-2, cy-2, 5, 5};
    SDL_SetRenderDrawColor(r, 255, 200, 100, 255);
    SDL_RenderFillRect(r, &dot);
}

// Круг с фиксированным вырезом 35° в сторону движения
static void draw_pacman(SDL_Renderer *r, const Pacman *p) {
    int cx = (int)(p->x * CELL_SIZE + CELL_SIZE / 2);
    int cy = (int)(p->y * CELL_SIZE + CELL_SIZE / 2);
    int radius = CELL_SIZE / 2 - 2;

    float mouth_rad = 35.0f * (float)M_PI / 180.0f;

    float base_angle = 0.0f;
    if (p->dir == DIR_LEFT)  base_angle = (float)M_PI;
    if (p->dir == DIR_UP)    base_angle = (float)(3.0 * M_PI / 2.0);
    if (p->dir == DIR_DOWN)  base_angle = (float)(M_PI / 2.0);

    SDL_SetRenderDrawColor(r, 255, 220, 0, 255);

    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx*dx + dy*dy > radius*radius) continue;

            float angle = atan2f((float)dy, (float)dx);
            float diff  = angle - base_angle;
            while (diff >  (float)M_PI) diff -= 2.0f * (float)M_PI;
            while (diff < -(float)M_PI) diff += 2.0f * (float)M_PI;

            if (fabsf(diff) < mouth_rad) continue;

            SDL_RenderDrawPoint(r, cx+dx, cy+dy);
        }
    }

    // Глаз
    int ex = cx + (int)(radius * 0.3f * cosf(base_angle - (float)M_PI / 2.0f));
    int ey = cy + (int)(radius * 0.3f * sinf(base_angle - (float)M_PI / 2.0f));
    SDL_SetRenderDrawColor(r, 20, 20, 20, 255);
    SDL_Rect eye = {ex, ey, 2, 2};
    SDL_RenderFillRect(r, &eye);
}

static void draw_ghost(SDL_Renderer *r, const Ghost *gh) {
    if (!gh->alive) return;

    int cx     = (int)(gh->x * CELL_SIZE + CELL_SIZE / 2);
    int cy     = (int)(gh->y * CELL_SIZE + CELL_SIZE / 2);
    int radius = CELL_SIZE / 2 - 2;

    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);

    // Верхний полукруг
    for (int dy = -radius; dy <= 0; dy++)
        for (int dx = -radius; dx <= radius; dx++)
            if (dx*dx + dy*dy <= radius*radius)
                SDL_RenderDrawPoint(r, cx+dx, cy+dy);

    // Прямоугольное тело
    SDL_Rect body = {cx - radius, cy, radius*2+1, radius};
    SDL_RenderFillRect(r, &body);

    // Зубчики снизу
    int seg_w = (radius * 2 + 1) / 3;
    for (int i = 0; i < 3; i++) {
        int sx = cx - radius + i * seg_w;
        int sy = cy + radius;
        for (int dy = 0; dy < radius/2; dy++) {
            float t    = (float)dy / (radius / 2.0f);
            int   half = (int)(t * seg_w / 2.0f);
            for (int dx = half; dx <= seg_w - half; dx++)
                SDL_RenderDrawPoint(r, sx+dx, sy-dy);
        }
    }

    // Белки глаз
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_Rect el = {cx - radius/2 - 1, cy - radius/2 - 2, 5, 5};
    SDL_Rect er = {cx + radius/2 - 3, cy - radius/2 - 2, 5, 5};
    SDL_RenderFillRect(r, &el);
    SDL_RenderFillRect(r, &er);

    // Зрачки
    SDL_SetRenderDrawColor(r, 0, 0, 200, 255);
    SDL_Rect pl = {cx - radius/2,     cy - radius/2 - 1, 3, 3};
    SDL_Rect pr = {cx + radius/2 - 1, cy - radius/2 - 1, 3, 3};
    SDL_RenderFillRect(r, &pl);
    SDL_RenderFillRect(r, &pr);
}

static void draw_map(SDL_Renderer *r, const Map *m) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            int x = col * CELL_SIZE;
            int y = row * CELL_SIZE;
            switch (m->cells[row][col]) {
                case CELL_WALL: draw_wall(r, x, y, CELL_SIZE, CELL_SIZE); break;
                case CELL_DOT:  draw_dot(r, x + CELL_SIZE/2, y + CELL_SIZE/2); break;
                default: break;
            }
        }
    }
}


static void render_text(SDL_Renderer *r, TTF_Font *font,
                         const char *text, int x, int y,
                         Uint8 R, Uint8 G, Uint8 B) {
    SDL_Color color = {R, G, B, 255};
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, text, color);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);
    if (tex) {
        SDL_Rect dst = {x, y, surf->w, surf->h};
        SDL_RenderCopy(r, tex, NULL, &dst);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(surf);
}


static void render_text_centered(SDL_Renderer *r, TTF_Font *font,
                                  const char *text, int y,
                                  Uint8 R, Uint8 G, Uint8 B) {
    int w = 0, h = 0;
    TTF_SizeUTF8(font, text, &w, &h);
    render_text(r, font, text, SCREEN_W/2 - w/2, y, R, G, B);
}

static void draw_hud(SDL_Renderer *r, TTF_Font *font, const Map *m) {
    int y0 = MAP_ROWS * CELL_SIZE;

    SDL_Rect hud = {0, y0, SCREEN_W, 48};
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderFillRect(r, &hud);

    char buf[64];
    snprintf(buf, sizeof(buf), "Dots: %d / %d", m->eaten_dots, m->total_dots);
    render_text(r, font, buf, 10, y0 + 14, 255, 220, 0);
}

static void draw_overlay(SDL_Renderer *r, TTF_Font *font_big, TTF_Font *font_small,
                          const char *line1, const char *line2) {
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 175);
    SDL_Rect bg = {0, MAP_ROWS*CELL_SIZE/2 - 40, SCREEN_W, 80};
    SDL_RenderFillRect(r, &bg);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

    render_text_centered(r, font_big,   line1, MAP_ROWS*CELL_SIZE/2 - 32, 255, 255,   0);
    render_text_centered(r, font_small, line2, MAP_ROWS*CELL_SIZE/2 + 10, 200, 200, 200);
}

// Game loop

static void game_init(Game *g) {
    map_init(&g->map);
    pacman_init(&g->pacman);
    ghost_init(&g->ghost);
    g->state = STATE_PLAYING;
    g->tick  = 0;
}

static void game_update(Game *g) {
    if (g->state != STATE_PLAYING) return;
    move_pacman(g);
    move_ghost(g);
    check_collisions(g);
    g->tick++;
}

static void game_render(Game *g) {
    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->renderer);

    draw_map(g->renderer, &g->map);
    draw_pacman(g->renderer, &g->pacman);
    draw_ghost(g->renderer, &g->ghost);
    draw_hud(g->renderer, g->font_hud, &g->map);

    if (g->state == STATE_WIN)
        draw_overlay(g->renderer, g->font_ovl, g->font_sub,
                     "YOU WIN!", "Press R to restart");
    else if (g->state == STATE_LOSE)
        draw_overlay(g->renderer, g->font_ovl, g->font_sub,
                     "GAME OVER", "Press R to restart");

    SDL_RenderPresent(g->renderer);
}

static void game_handle_key(Game *g, SDL_Keycode key) {
    if (key == SDLK_r) { game_init(g); return; }
    if (g->state != STATE_PLAYING) return;
    switch (key) {
        case SDLK_LEFT:  case SDLK_a: g->pacman.next_dir = DIR_LEFT;  break;
        case SDLK_RIGHT: case SDLK_d: g->pacman.next_dir = DIR_RIGHT; break;
        case SDLK_UP:    case SDLK_w: g->pacman.next_dir = DIR_UP;    break;
        case SDLK_DOWN:  case SDLK_s: g->pacman.next_dir = DIR_DOWN;  break;
        default: break;
    }
}


int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    srand((unsigned)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font *font_hud = TTF_OpenFont(FONT_PATH, FONT_HUD);
    TTF_Font *font_ovl = TTF_OpenFont(FONT_PATH, FONT_OVL);
    TTF_Font *font_sub = TTF_OpenFont(FONT_PATH, FONT_SUB);

    if (!font_hud || !font_ovl || !font_sub) {
        fprintf(stderr, "TTF_OpenFont error: %s\n  (убедись, что %s лежит рядом с .exe)\n",
                TTF_GetError(), FONT_PATH);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "PACMAN",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        TTF_Quit(); SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit(); SDL_Quit();
        return 1;
    }

    Game g;
    g.window   = window;
    g.renderer = renderer;
    g.font_hud = font_hud;
    g.font_ovl = font_ovl;
    g.font_sub = font_sub;
    game_init(&g);

    int running = 1;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
                game_handle_key(&g, e.key.keysym.sym);
            }
        }

        Uint32 now     = SDL_GetTicks();
        Uint32 elapsed = now - last_time;
        if (elapsed >= FRAME_MS) {
            last_time = now;
            game_update(&g);
            game_render(&g);
        } else {
            SDL_Delay(1);
        }
    }

    TTF_CloseFont(font_hud);
    TTF_CloseFont(font_ovl);
    TTF_CloseFont(font_sub);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}