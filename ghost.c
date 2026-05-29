#include "ghost.h"
#include "map.h"
#include "sound.h"

static const float GX[GHOST_COUNT]  = {13.0f, 14.0f, 13.0f, 14.0f};
static const float GY[GHOST_COUNT]  = {13.5f, 13.5f, 15.0f, 15.0f};
static const int   GAI[GHOST_COUNT] = {AI_CHASE, AI_AMBUSH, AI_RANDOM, AI_PATROL};
static const Uint8 GR[GHOST_COUNT]  = {255, 255,   0, 255};
static const Uint8 GG[GHOST_COUNT]  = {  0, 184, 255, 184};
static const Uint8 GB[GHOST_COUNT]  = {  0, 255, 255,   0};

#define GHOST_EXIT_COL  13
#define GHOST_EXIT_ROW  11

void ghosts_init(Ghost ghosts[GHOST_COUNT]) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *gh  = &ghosts[i];
        gh->x = gh->px = gh->visual_x = GX[i];
        gh->y = gh->py = gh->visual_y = GY[i];
        gh->dir        = (i % 2 == 0) ? DIR_LEFT : DIR_RIGHT;
        gh->move_timer = i * 30;
        gh->state      = GHOST_NORMAL;
        gh->ai         = GAI[i];
        gh->speed      = GHOST_SPEED_BASE + i;
        gh->color_r    = GR[i];
        gh->color_g    = GG[i];
        gh->color_b    = GB[i];
    }
}

static int best_toward(Ghost *gh, float tx, float ty, int *valid, int n) {
    int best=valid[0]; float bestd=1e9f;
    int cx=(int)roundf(gh->x), cy=(int)roundf(gh->y);
    for(int i=0;i<n;i++){
        int dx,dy; dir_to_delta(valid[i],&dx,&dy);
        float fx=(float)(cx+dx)-tx, fy=(float)(cy+dy)-ty;
        float d=fx*fx+fy*fy;
        if(d<bestd){bestd=d;best=valid[i];}
    }
    return best;
}

static int best_away(Ghost *gh, float tx, float ty, int *valid, int n) {
    int best=valid[0]; float bestd=-1.0f;
    int cx=(int)roundf(gh->x), cy=(int)roundf(gh->y);
    for(int i=0;i<n;i++){
        int dx,dy; dir_to_delta(valid[i],&dx,&dy);
        float fx=(float)(cx+dx)-tx, fy=(float)(cy+dy)-ty;
        float d=fx*fx+fy*fy;
        if(d>bestd){bestd=d;best=valid[i];}
    }
    return best;
}

static int build_valid(Ghost *gh, const Map *m, int *valid) {
    int dirs[4]={DIR_LEFT,DIR_RIGHT,DIR_UP,DIR_DOWN};
    int opp[5]={DIR_NONE,DIR_RIGHT,DIR_LEFT,DIR_DOWN,DIR_UP};
    int n=0;
    for(int i=0;i<4;i++){
        if(dirs[i]==opp[gh->dir]) continue;
        int dx,dy; dir_to_delta(dirs[i],&dx,&dy);
        if(can_enter_ghost(m,(int)roundf(gh->x)+dx,(int)roundf(gh->y)+dy))
            valid[n++]=dirs[i];
    }
    if(n==0){
        int dx,dy; dir_to_delta(opp[gh->dir],&dx,&dy);
        if(can_enter_ghost(m,(int)roundf(gh->x)+dx,(int)roundf(gh->y)+dy))
            valid[n++]=opp[gh->dir];
    }
    return n;
}

static float patrol_tx[GHOST_COUNT]={1.0f,26.0f,1.0f,26.0f};
static float patrol_ty[GHOST_COUNT]={1.0f, 1.0f,30.0f,30.0f};

static void ghost_choose_dir(Ghost *gh, int idx, const Game *g) {
    const Map *m=&g->map;
    const Pacman *p=&g->pacman;
    int valid[4]; int n=build_valid(gh,m,valid);
    if(n==0) return;

    if(gh->state==GHOST_SCARED){
        gh->dir=best_away(gh,p->x,p->y,valid,n);
        return;
    }

    int cx=(int)roundf(gh->x), cy=(int)roundf(gh->y);
    if(cy>=12 && cy<=15 && cx>=10 && cx<=17){
        gh->dir=best_toward(gh,(float)GHOST_EXIT_COL,(float)GHOST_EXIT_ROW,valid,n);
        return;
    }

    switch(gh->ai){
        case AI_CHASE:
            gh->dir=best_toward(gh,p->x,p->y,valid,n);
            break;
        case AI_AMBUSH:{
            int pdx,pdy; dir_to_delta(p->dir,&pdx,&pdy);
            gh->dir=best_toward(gh,p->x+pdx*4,p->y+pdy*4,valid,n);
            break;
        }
        case AI_PATROL:
            gh->dir=best_toward(gh,patrol_tx[idx],patrol_ty[idx],valid,n);
            if(fabsf(gh->x-patrol_tx[idx])<2.0f && fabsf(gh->y-patrol_ty[idx])<2.0f){
                patrol_tx[idx]=(patrol_tx[idx]<14)?26.0f:1.0f;
                patrol_ty[idx]=(patrol_ty[idx]<16)?30.0f:1.0f;
            }
            break;
        default:
            gh->dir=valid[rand()%n];
            break;
    }
}

static int collision_radius(float ax, float ay, float bx, float by) {
    float dx=ax-bx, dy=ay-by;
    return (dx*dx+dy*dy) < (0.85f*0.85f);
}

static void ghost_step(Ghost *gh, int idx, Game *g) {
    const Map *m=&g->map;

    float t=(float)gh->move_timer/(float)gh->speed;
    if(t>1.0f) t=1.0f;
    gh->visual_x=gh->px+(gh->x-gh->px)*t;
    gh->visual_y=gh->py+(gh->y-gh->py)*t;

    // Проверка коллизии во время движения
    if(collision_radius(gh->visual_x,gh->visual_y,
                        g->pacman.visual_x,g->pacman.visual_y)){
        if(gh->state==GHOST_SCARED){
            gh->state=GHOST_DEAD;
            g->gs.score+=SCORE_GHOST;
            sound_play_ghost();
        } else if(gh->state==GHOST_NORMAL){
            g->state=STATE_LOSE;
        }
        return;
    }

    gh->move_timer++;
    if(gh->move_timer<gh->speed) return;
    gh->move_timer=0;

    ghost_choose_dir(gh,idx,g);

    gh->px=gh->x; gh->py=gh->y;
    int cx=(int)roundf(gh->x), cy=(int)roundf(gh->y);
    int dx,dy; dir_to_delta(gh->dir,&dx,&dy);
    int nx=cx+dx, ny=cy+dy;

    //Портал — сброс move_timer чтобы не было фриза
    if(cy==PORTAL_ROW && nx<0){
        gh->x=(float)(MAP_COLS-1); gh->y=(float)PORTAL_ROW;
        gh->px=gh->x; gh->py=gh->y;
        gh->visual_x=gh->x; gh->visual_y=gh->y;
        gh->move_timer=gh->speed;
    }
    else if(cy==PORTAL_ROW && nx>=MAP_COLS){
        gh->x=0.0f; gh->y=(float)PORTAL_ROW;
        gh->px=gh->x; gh->py=gh->y;
        gh->visual_x=gh->x; gh->visual_y=gh->y;
        gh->move_timer=gh->speed;
    }
    else if(can_enter_ghost(m,nx,ny)){
        gh->x=(float)nx; gh->y=(float)ny;
    }
}

void ghosts_update(Game *g) {
    if(g->gs.energizer_timer>0){
        g->gs.energizer_timer--;
        if(g->gs.energizer_timer==0)
            for(int i=0;i<GHOST_COUNT;i++)
                if(g->ghosts[i].state==GHOST_SCARED)
                    g->ghosts[i].state=GHOST_NORMAL;
    }

    int speed_lvl=g->map.eaten_dots/SPEED_UP_EVERY;
    for(int i=0;i<GHOST_COUNT;i++){
        Ghost *gh=&g->ghosts[i];
        if(gh->state==GHOST_DEAD) continue;
        int base=GHOST_SPEED_BASE+i-speed_lvl;
        if(base<SPEED_MIN) base=SPEED_MIN;
        gh->speed=(gh->state==GHOST_SCARED)?base+4:base;
        ghost_step(gh,i,g);
    }
}

void draw_ghost(SDL_Renderer *r, const Ghost *gh, int tick) {
    if(gh->state==GHOST_DEAD) return;
    int cx=(int)(gh->visual_x*CELL_SIZE+CELL_SIZE/2);
    int cy=(int)(gh->visual_y*CELL_SIZE+CELL_SIZE/2);
    int radius=CELL_SIZE/2-1;
    Uint8 R,G,B;
    if(gh->state==GHOST_SCARED){
        if((tick/8)%2){R=255;G=255;B=255;}
        else          {R=30; G=30; B=220;}
    } else {
        R=gh->color_r; G=gh->color_g; B=gh->color_b;
    }
    SDL_SetRenderDrawColor(r,R,G,B,255);
    for(int dy=-radius;dy<=0;dy++)
        for(int dx=-radius;dx<=radius;dx++)
            if(dx*dx+dy*dy<=radius*radius)
                SDL_RenderDrawPoint(r,cx+dx,cy+dy);
    SDL_Rect body={cx-radius,cy,radius*2+1,radius};
    SDL_RenderFillRect(r,&body);
    int seg_w=(radius*2+1)/3;
    for(int i=0;i<3;i++){
        int sx=cx-radius+i*seg_w, sy=cy+radius;
        for(int dy2=0;dy2<radius/2;dy2++){
            float t2=(float)dy2/(radius/2.0f);
            int half=(int)(t2*seg_w/2.0f);
            for(int dx2=half;dx2<=seg_w-half;dx2++)
                SDL_RenderDrawPoint(r,sx+dx2,sy-dy2);
        }
    }
    if(gh->state==GHOST_SCARED){
        SDL_SetRenderDrawColor(r,255,80,80,255);
        SDL_Rect m1={cx-4,cy-radius/2,3,2};
        SDL_Rect m2={cx+2,cy-radius/2,3,2};
        SDL_RenderFillRect(r,&m1); SDL_RenderFillRect(r,&m2);
    } else {
        SDL_SetRenderDrawColor(r,255,255,255,255);
        SDL_Rect el={cx-radius/2-1,cy-radius/2-2,5,5};
        SDL_Rect er={cx+radius/2-3,cy-radius/2-2,5,5};
        SDL_RenderFillRect(r,&el); SDL_RenderFillRect(r,&er);
        SDL_SetRenderDrawColor(r,0,0,180,255);
        SDL_Rect pl={cx-radius/2,  cy-radius/2-1,3,3};
        SDL_Rect pr={cx+radius/2-1,cy-radius/2-1,3,3};
        SDL_RenderFillRect(r,&pl); SDL_RenderFillRect(r,&pr);
    }
}

void draw_ghosts(SDL_Renderer *r, Ghost ghosts[GHOST_COUNT], int tick) {
    for(int i=0;i<GHOST_COUNT;i++)
        draw_ghost(r,&ghosts[i],tick);
}
