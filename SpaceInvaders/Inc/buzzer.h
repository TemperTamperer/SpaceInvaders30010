#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include <stddef.h>

// One note: freq_hz = 0 means silence, dur_ms is duration in ms
typedef struct {
    uint16_t freq_hz;
    uint16_t dur_ms;
} buzzer_note_t;

// A song is just an array of notes
typedef struct {
    const buzzer_note_t *notes;
    size_t len;
} buzzer_song_t;

// Sound effects used in the game
typedef enum {
    SFX_SHOOT = 0,
    SFX_ENEMY_DEATH,
    SFX_PLAYER_HIT,
    SFX_BONUS,
    SFX_VICTORY,
    SFX_GAMEOVER
} buzzer_sfx_t;

void buzzer_init(void);

// Background music
void buzzer_set_bg(buzzer_song_t song, uint8_t loop);
void buzzer_bg_start(void);

// Play a sound effect (temporarily pauses background music)
void buzzer_play_sfx(buzzer_sfx_t sfx);

// Call from the game loop. elapsed_ms is time since last call.
void buzzer_update(uint16_t elapsed_ms);

#endif
