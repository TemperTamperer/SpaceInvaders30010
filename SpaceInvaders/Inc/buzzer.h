#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stddef.h>

/*
  En note:
  freq_hz = 0  -> pause (ingen lyd)
  dur_ms  = varighed i millisekunder
*/
typedef struct {
    uint16_t freq_hz;
    uint16_t dur_ms;
} buzzer_note_t;

/* En "sang" = array af noter */
typedef struct {
    const buzzer_note_t *notes;
    size_t len;
} buzzer_song_t;

/* Lydeffekter i spillet */
typedef enum {
    SFX_SHOOT = 0,
    SFX_ENEMY_DEATH,
    SFX_PLAYER_HIT,
    SFX_BONUS,
    SFX_VICTORY,
    SFX_GAMEOVER
} buzzer_sfx_t;

/* Init */
void buzzer_init(void);

/* Baggrundsmusik */
void buzzer_set_bg(buzzer_song_t song, uint8_t loop);
void buzzer_bg_start(void);

/* Lydeffekt */
void buzzer_play_sfx(buzzer_sfx_t sfx);

/*
  Kaldes fra game loop
  I har 20 Hz tick -> 50 ms
*/
void buzzer_update(uint16_t elapsed_ms);

#endif
