#include "buzzer_songs.h"

#define BUZZER_REST 0  // 0 Hz means silence

// Level 1: slow background music
static const buzzer_note_t BG_L1_NOTES[] = {
    { 262, 200 }, { BUZZER_REST, 120 },  // C
    { 262, 200 }, { BUZZER_REST, 120 },
    { 330, 200 }, { BUZZER_REST, 160 },  // E
    { 294, 300 }, { BUZZER_REST, 300 },  // D
};

const buzzer_song_t BG_LEVEL_1 = {
    BG_L1_NOTES,
    sizeof(BG_L1_NOTES) / sizeof(BG_L1_NOTES[0])
};

// Level 2: faster tempo
static const buzzer_note_t BG_L2_NOTES[] = {
    { 262, 140 }, { BUZZER_REST,  90 },  // C
    { 262, 140 }, { BUZZER_REST,  90 },
    { 294, 140 }, { BUZZER_REST,  90 },  // D
    { 262, 160 }, { BUZZER_REST, 140 },  // C
    { 330, 140 }, { BUZZER_REST, 120 },  // E
    { 294, 220 }, { BUZZER_REST, 220 },  // D
};

const buzzer_song_t BG_LEVEL_2 = {
    BG_L2_NOTES,
    sizeof(BG_L2_NOTES) / sizeof(BG_L2_NOTES[0])
};

// Level 3: alarm sound
static const buzzer_note_t BG_L3_NOTES[] = {
    { 784, 100 }, { BUZZER_REST, 40 },  // G5
    { 659, 100 }, { BUZZER_REST, 40 },  // E5
    { 784, 100 }, { BUZZER_REST, 40 },
    { 988, 180 }, { BUZZER_REST, 80 },  // B5
};

const buzzer_song_t BG_LEVEL_3 = {
    BG_L3_NOTES,
    sizeof(BG_L3_NOTES) / sizeof(BG_L3_NOTES[0])
};
