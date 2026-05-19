#ifndef SOUND_H
#define SOUND_H

/* Звуковой модуль для Пакмана */
#define SOUND_ENABLED

#ifdef SOUND_ENABLED
  #include <SDL_mixer.h>

  /* Пути к файлам звука */
  #define SFX_EAT_DOT       "snd_dot.wav"
  #define SFX_EAT_GHOST     "snd_ghost.wav"
  #define SFX_DEATH         "snd_death.wav"
  #define SFX_ENERGIZER     "snd_energizer.wav"
  #define SFX_FRUIT         "snd_fruit.wav"

  void sound_init(void);
  void sound_quit(void);
  void sound_play_dot(void);
  void sound_play_ghost(void);
  void sound_play_death(void);
  void sound_play_energizer(void);
  void sound_play_fruit(void);
#else
  /* Заглушки для сборки без звука */
  static inline void sound_init(void)           {}
  static inline void sound_quit(void)           {}
  static inline void sound_play_dot(void)       {}
  static inline void sound_play_ghost(void)     {}
  static inline void sound_play_death(void)     {}
  static inline void sound_play_energizer(void) {}
  static inline void sound_play_fruit(void)     {}
#endif

#endif