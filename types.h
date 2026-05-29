#ifndef TYPES_H
#define TYPES_H

#include <SDL.h>
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
#define SCREEN_H     (MAP_ROWS * CELL_SIZE + 56)

#define FPS       60
#define FRAME_MS  (1000 / FPS)

#define PACMAN_SPEED_BASE  8
#define GHOST_SPEED_BASE   11
#define SPEED_UP_EVERY     60
#define SPEED_MIN          4

#define PORTAL_ROW  14

#define CELL_EMPTY      0
#define CELL_WALL       1
#define CELL_DOT        2
#define CELL_EMPTY2     3
#define CELL_ENERGIZER  4
#define CELL_FRUIT_A    5   //яблоко 
#define CELL_FRUIT_B    6   //банан
#define CELL_FRUIT_C    7   //вишня

#define DIR_NONE  0
#define DIR_LEFT  1
#define DIR_RIGHT 2
#define DIR_UP    3
#define DIR_DOWN  4

#define STATE_MENU     0
#define STATE_PLAYING  1
#define STATE_WIN      2
#define STATE_LOSE     3
#define STATE_SCORES   4

#define GHOST_NORMAL  0
#define GHOST_SCARED  1
#define GHOST_DEAD    2

#define AI_RANDOM  0
#define AI_CHASE   1
#define AI_AMBUSH  2
#define AI_PATROL  3

#define ENERGIZER_TICKS  360
#define ENERGIZER_BLINK  120

//Фрукты
#define FRUIT_LIFETIME      500   //тиков живёт один фрукт 
#define FRUIT_APPEARS_AT    50    //первое появление 
#define FRUIT_REPEAT        40    //потом каждые N точек 
#define FRUIT_MAX           3     //максимум одновременно 


#define SCORE_DOT        10
#define SCORE_ENERGIZER  50
#define SCORE_GHOST      200
#define SCORE_FRUIT_A    100   //яблоко
#define SCORE_FRUIT_B    150   //банан
#define SCORE_FRUIT_C    200   //вишня  

#define FONT_PATH  "OpenSans.ttf"
#define FONT_HUD   16
#define FONT_OVL   30
#define FONT_SUB   15

#define SCORES_FILE   "scores.dat"
#define SCORES_COUNT  5

#define SFX_EAT_DOT    "snd_dot.wav"
#define SFX_EAT_GHOST  "snd_ghost.wav"
#define SFX_DEATH      "snd_death.wav"
#define SFX_ENERGIZER  "snd_energizer.wav"
#define SFX_FRUIT      "snd_fruit.wav"

#define GHOST_COUNT 4

typedef struct {
    int col, row;
    int timer;   
    int type;   
} Fruit;

typedef struct {
    Uint8 cells[MAP_ROWS][MAP_COLS];
    int   total_dots;
    int   eaten_dots;
    int   next_fruit_at;        
    Fruit fruits[FRUIT_MAX];
} Map;

typedef struct {
    float x, y;
    float px, py;
    float visual_x, visual_y;
    int   dir, next_dir;
    int   move_timer;
    int   anim_frame, anim_timer;
    int   speed;
    int   just_teleported;
} Pacman;

typedef struct {
    float x, y;
    float px, py;
    float visual_x, visual_y;
    int   dir;
    int   move_timer;
    int   state;
    int   ai;
    int   speed;
    Uint8 color_r, color_g, color_b;
} Ghost;

typedef struct {
    int score;
    int energizer_timer;
} GameState;

typedef struct {
    int scores[SCORES_COUNT];
} ScoreBoard;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font_hud;
    TTF_Font     *font_ovl;
    TTF_Font     *font_sub;
    Map           map;
    Pacman        pacman;
    Ghost         ghosts[GHOST_COUNT];
    GameState     gs;
    ScoreBoard    scoreboard;
    int           state;
    int           tick;
    SDL_Rect      btn_play;
    SDL_Rect      btn_scores;
} Game;

#endif
