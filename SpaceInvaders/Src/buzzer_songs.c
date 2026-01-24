#include "buzzer_songs.h"

/* background theme notes */
static const buzzer_note_t BG_THEME_NOTES[] = {
    { 523, 150 }, { 0, 50 },
    { 659, 150 }, { 0, 50 },
    { 784, 200 }, { 0, 80 },
    { 659, 150 }, { 0, 50 },
    { 523, 250 }, { 0, 150 },
};

const buzzer_song_t BG_MAIN_THEME = {
    BG_THEME_NOTES,
    sizeof(BG_THEME_NOTES) / sizeof(BG_THEME_NOTES[0])
};
