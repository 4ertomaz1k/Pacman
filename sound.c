#include "sound.h"

#ifdef SOUND_ENABLED

static Mix_Chunk *snd_dot       = NULL;
static Mix_Chunk *snd_ghost     = NULL;
static Mix_Chunk *snd_death     = NULL;
static Mix_Chunk *snd_energizer = NULL;
static Mix_Chunk *snd_fruit     = NULL;

// Выделенный канал для точек — не накладывается
#define CH_DOT  0
#define CH_FREE -1  //любой свободный

void sound_init(void) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0) return;
    Mix_AllocateChannels(8);
    snd_dot       = Mix_LoadWAV(SFX_EAT_DOT);
    snd_ghost     = Mix_LoadWAV(SFX_EAT_GHOST);
    snd_death     = Mix_LoadWAV(SFX_DEATH);
    snd_energizer = Mix_LoadWAV(SFX_ENERGIZER);
    snd_fruit     = Mix_LoadWAV(SFX_FRUIT);
}

void sound_quit(void) {
    Mix_HaltChannel(-1);
    Mix_FreeChunk(snd_dot);
    Mix_FreeChunk(snd_ghost);
    Mix_FreeChunk(snd_death);
    Mix_FreeChunk(snd_energizer);
    Mix_FreeChunk(snd_fruit);
    Mix_CloseAudio();
}

//Точки — канал 0, новый звук только если предыдущий уже закончился
void sound_play_dot(void) {
    if (!snd_dot) return;
    if (!Mix_Playing(CH_DOT))
        Mix_PlayChannel(CH_DOT, snd_dot, 0);
}

void sound_play_ghost(void) {
    if (snd_ghost) Mix_PlayChannel(CH_FREE, snd_ghost, 0);
}

void sound_play_death(void) {
    if (snd_death) {
        Mix_HaltChannel(-1);  
        Mix_PlayChannel(CH_FREE, snd_death, 0);
    }
}

void sound_play_energizer(void) {
    if (snd_energizer) Mix_PlayChannel(CH_FREE, snd_energizer, 0);
}

void sound_play_fruit(void) {
    if (snd_fruit) Mix_PlayChannel(CH_FREE, snd_fruit, 0);
}

#endif
