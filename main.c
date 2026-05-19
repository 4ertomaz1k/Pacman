/*
 * ПАКМАН — курсовая работа
 *
 * Сборка (Windows MinGW):
 *   gcc main.c game.c map.c pacman.c ghost.c render.c scores.c sound.c ^
 *       -o Pacman.exe ^
 *       -I C:/SDL2/include -I C:/SDL2_ttf/include ^
 *       -L C:/SDL2/lib/x64 -L C:/SDL2_ttf/lib ^
 *       -lSDL2main -lSDL2 -lSDL2_ttf -lm -mwindows
 */

#include "game.h"
#include "render.h"
#include "scores.h"
#include "sound.h"

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    srand((unsigned)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        SDL_Quit(); return 1;
    }
    sound_init();

    TTF_Font *font_hud = TTF_OpenFont(FONT_PATH, FONT_HUD);
    TTF_Font *font_ovl = TTF_OpenFont(FONT_PATH, FONT_OVL);
    TTF_Font *font_sub = TTF_OpenFont(FONT_PATH, FONT_SUB);
    if (!font_hud || !font_ovl || !font_sub) {
        fprintf(stderr, "TTF_OpenFont: %s\n  Положи %s рядом с .exe\n",
                TTF_GetError(), FONT_PATH);
        TTF_Quit(); SDL_Quit(); return 1;
    }

    SDL_Window *window = SDL_CreateWindow("ПАКМАН",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        TTF_Quit(); SDL_Quit(); return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window); TTF_Quit(); SDL_Quit(); return 1;
    }

    Game g;
    g.window   = window;
    g.renderer = renderer;
    g.font_hud = font_hud;
    g.font_ovl = font_ovl;
    g.font_sub = font_sub;
    g.state    = STATE_MENU;
    g.tick     = 0;
    g.gs.score = 0;
    g.gs.energizer_timer = 0;

    /* Кнопки меню — инициализируем один раз */
    g.btn_play.x = SCREEN_W/2 - 90;
    g.btn_play.y = SCREEN_H/2 - 30;
    g.btn_play.w = 180;
    g.btn_play.h = 55;
    g.btn_scores.x = SCREEN_W/2 - 100;
    g.btn_scores.y = SCREEN_H/2 + 60;
    g.btn_scores.w = 200;
    g.btn_scores.h = 40;

    scores_load(&g.scoreboard);

    int running = 1;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
            else if (e.type == SDL_KEYDOWN) {
                SDL_Keycode key = e.key.keysym.sym;

                /* ESC — обрабатываем централизованно здесь */
                if (key == SDLK_ESCAPE) {
                    if (g.state == STATE_MENU) {
                        running = 0;       /* в меню → выход */
                    } else {
                        g.state = STATE_MENU; /* везде иначе → меню */
                    }
                } else {
                    game_handle_key(&g, key);
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN &&
                     e.button.button == SDL_BUTTON_LEFT) {
                game_handle_click(&g, e.button.x, e.button.y);
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

    sound_quit();
    TTF_CloseFont(font_hud);
    TTF_CloseFont(font_ovl);
    TTF_CloseFont(font_sub);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
