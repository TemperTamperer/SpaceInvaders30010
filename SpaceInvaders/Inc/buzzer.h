#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stddef.h>

// single note
// freq_hz = 0 -> pause
// dur_ms  = duration in ms
typedef struct {
    uint16_t freq_hz;
    uint16_t dur_ms;
} buzzer_note_t;

// song = array of notes
typedef struct {
    const buzzer_note_t *notes;
    size_t len;
} buzzer_song_t;

// sound effects
typedef enum {
    SFX_SHOOT = 0,
    SFX_ENEMY_DEATH,
    SFX_PLAYER_HIT,
    SFX_BONUS,
    SFX_VICTORY,
    SFX_GAMEOVER
} buzzer_sfx_t;

// init buzzer hardware
void buzzer_init(void);

// background music control
void buzzer_set_bg(buzzer_song_t song, uint8_t loop);
void buzzer_bg_start(void);

// play one-shot sound effect
void buzzer_play_sfx(buzzer_sfx_t sfx);

// update timing (called from game loop)
void buzzer_update(uint16_t elapsed_ms);

#endif
