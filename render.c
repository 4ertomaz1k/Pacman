#include "render.h"
#include "map.h"
#include "pacman.h"
#include "ghost.h"

void render_text(SDL_Renderer *r, TTF_Font *font,
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

void render_text_centered(SDL_Renderer *r, TTF_Font *font,
                          const char *text, int y,
                          Uint8 R, Uint8 G, Uint8 B) {
    int w=0, h=0;
    TTF_SizeUTF8(font, text, &w, &h);
    render_text(r, font, text, SCREEN_W/2-w/2, y, R, G, B);
}

static void draw_button(SDL_Renderer *r, TTF_Font *font,
                        SDL_Rect rect, const char *text,
                        Uint8 br, Uint8 bg2, Uint8 bb) {
    SDL_Point mouse; SDL_GetMouseState(&mouse.x, &mouse.y);
    int hover = (mouse.x>=rect.x && mouse.x<=rect.x+rect.w &&
                 mouse.y>=rect.y && mouse.y<=rect.y+rect.h);

    Uint8 fr = hover ? (br<200?br+55:255) : br;
    Uint8 fg = hover ? (bg2<200?bg2+55:255) : bg2;
    Uint8 fb = hover ? (bb<200?bb+55:255) : bb;

    SDL_SetRenderDrawColor(r, fr, fg, fb, 255);
    SDL_RenderFillRect(r, &rect);
    SDL_SetRenderDrawColor(r, 255,255,255,160);
    SDL_RenderDrawRect(r, &rect);

    int tw=0,th=0; TTF_SizeUTF8(font, text, &tw, &th);
    render_text(r, font, text,
                rect.x+rect.w/2-tw/2,
                rect.y+rect.h/2-th/2,
                255,255,255);
}

static void draw_play_triangle(SDL_Renderer *r, int cx, int cy, int size) {
    SDL_SetRenderDrawColor(r, 255,255,255,255);
    for (int row=-size; row<=size; row++) {
        int half = size - abs(row);
        for (int col=-half/2; col<=half; col++)
            SDL_RenderDrawPoint(r, cx+col, cy+row);
    }
}

void draw_hud(SDL_Renderer *r, TTF_Font *font, const Map *m, int score) {
    int y0 = MAP_ROWS * CELL_SIZE;
    SDL_Rect hud = {0, y0, SCREEN_W, 56};
    SDL_SetRenderDrawColor(r, 0,0,0,255);
    SDL_RenderFillRect(r, &hud);
    SDL_SetRenderDrawColor(r, 50,50,50,255);
    SDL_RenderDrawLine(r, 0, y0, SCREEN_W, y0);

    char buf[64];
    snprintf(buf, sizeof(buf), "Очки: %d", score);
    render_text(r, font, buf, 8, y0+6, 255,220,0);
    snprintf(buf, sizeof(buf), "Точки: %d/%d", m->eaten_dots, m->total_dots);
    render_text(r, font, buf, 8, y0+28, 180,180,180);
}

void draw_overlay(SDL_Renderer *r, TTF_Font *font_big, TTF_Font *font_small,
                  const char *line1, const char *line2) {
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0,0,0,185);
    SDL_Rect bg={0, MAP_ROWS*CELL_SIZE/2-45, SCREEN_W, 90};
    SDL_RenderFillRect(r, &bg);
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
    render_text_centered(r, font_big,   line1, MAP_ROWS*CELL_SIZE/2-36, 255,255,0);
    render_text_centered(r, font_small, line2, MAP_ROWS*CELL_SIZE/2+12, 200,200,200);
}

void draw_menu(Game *g) {
    SDL_Renderer *r = g->renderer;
    SDL_SetRenderDrawColor(r, 0,0,0,255);
    SDL_RenderClear(r);

    render_text_centered(r, g->font_ovl, "ПАКМАН", SCREEN_H/5, 255,220,0);


    /* Пакман-декорация */
    int dcx=SCREEN_W/2, dcy=SCREEN_H/5-35, drad=22;
    SDL_SetRenderDrawColor(r,255,220,0,255);
    for(int dy=-drad;dy<=drad;dy++)
        for(int dx=-drad;dx<=drad;dx++){
            if(dx*dx+dy*dy>drad*drad) continue;
            float a=atan2f((float)dy,(float)dx);
            if(fabsf(a)<0.45f) continue;
            SDL_RenderDrawPoint(r,dcx+dx,dcy+dy);
        }

    /* Кнопки — координаты фиксированы здесь и совпадают с Game.btn_* */
    draw_button(r, g->font_hud, g->btn_play,   "", 30,130,30);
    draw_play_triangle(r,
        g->btn_play.x + g->btn_play.w/2,
        g->btn_play.y + g->btn_play.h/2, 16);
    render_text_centered(r, g->font_sub, "ИГРАТЬ",
        g->btn_play.y + g->btn_play.h + 8, 160,160,160);

    draw_button(r, g->font_hud, g->btn_scores, "РЕКОРДЫ", 40,40,140);

    render_text_centered(r, g->font_sub,
        "WASD / Стрелки — движение",
        SCREEN_H-75, 80,80,80);
    render_text_centered(r, g->font_sub,
        "R — новая игра   L — рекорды   ESC — выход",
        SCREEN_H-50, 80,80,80);

    SDL_RenderPresent(r);
}

void draw_scores_screen(SDL_Renderer *r, TTF_Font *font_big,
                        TTF_Font *font_small,
                        const ScoreBoard *sb, int current_score) {
    SDL_SetRenderDrawColor(r,0,0,0,255);
    SDL_RenderClear(r);
    render_text_centered(r, font_big, "ТАБЛИЦА РЕКОРДОВ", 30, 255,220,0);

    int y=100;
    for(int i=0;i<SCORES_COUNT;i++){
        char buf[64];
        snprintf(buf,sizeof(buf),"#%d    %d", i+1, sb->scores[i]);
        int highlight = (sb->scores[i]>0 && sb->scores[i]==current_score);
        render_text_centered(r, font_small, buf, y,
            highlight?255:200, highlight?80:200, 200);
        y+=32;
    }
    render_text_centered(r, font_small, "R — новая игра   ESC — меню",    y+30, 150,150,150);
    /* убрано — уже есть выше */
    (void)(y+55, 150,150,150);
}

void game_render(Game *g) {
    SDL_SetRenderDrawColor(g->renderer,0,0,0,255);
    SDL_RenderClear(g->renderer);

    if (g->state == STATE_MENU) {
        draw_menu(g);
        return;
    }

    if (g->state == STATE_SCORES) {
        draw_scores_screen(g->renderer, g->font_ovl, g->font_sub,
                           &g->scoreboard, g->gs.score);
        SDL_RenderPresent(g->renderer);
        return;
    }

    draw_map(g->renderer, &g->map, g->gs.energizer_timer, g->tick);
    draw_pacman(g->renderer, &g->pacman);
    draw_ghosts(g->renderer, g->ghosts, g->tick);
    draw_hud(g->renderer, g->font_hud, &g->map, g->gs.score);

    if (g->state == STATE_WIN)
        draw_overlay(g->renderer, g->font_ovl, g->font_sub,
                     "ПОБЕДА!", "R — заново   L — рекорды   ESC — меню");
    else if (g->state == STATE_LOSE)
        draw_overlay(g->renderer, g->font_ovl, g->font_sub,
                     "ИГРА ОКОНЧЕНА", "R — заново   L — рекорды   ESC — меню");

    SDL_RenderPresent(g->renderer);
}
