#ifndef LEVEL_H_
#define LEVEL_H_

#include <stdint.h>

typedef struct {
    uint8_t level;
    uint8_t prev_level;
    uint16_t popup_ticks;
    uint8_t popup_ended_pulse;
} LevelState;

void level_init(LevelState* s);
void level_update_from_score(LevelState* s, uint32_t score);

uint8_t level_get(const LevelState* s);
uint16_t level_spawn_limit(const LevelState* s);

uint8_t level_popup_active(const LevelState* s);
void level_popup_tick(LevelState* s);
uint8_t level_popup_just_ended(LevelState* s);

#endif
