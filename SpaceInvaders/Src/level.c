#include "level.h"

#define LEVEL2_SCORE 1000u
#define LEVEL3_SCORE 2000u
#define LEVEL_POPUP_TICKS 40u

// default level state
void level_init(LevelState* s)
{
    s->level = 1;
    s->prev_level = 1;
    s->popup_ticks = 0;
    s->popup_ended_pulse = 0;
}

// update level thresholds based on score
void level_update_from_score(LevelState* s, uint32_t score)
{
    uint8_t new_level;
    if (score >= LEVEL3_SCORE) new_level = 3;
    else if (score >= LEVEL2_SCORE) new_level = 2;
    else new_level = 1;

    s->level = new_level;

    if (s->level != s->prev_level)
    {
        s->popup_ticks = LEVEL_POPUP_TICKS;
        s->prev_level = s->level;
        s->popup_ended_pulse = 0;
    }
}

uint8_t level_get(const LevelState* s)
{
    return s->level;
}

// enemy spawn pacing per level
uint16_t level_spawn_limit(const LevelState* s)
{
    if (s->level == 3) return 45;
    if (s->level == 2) return 60;
    return 80;
}

// popup visible while ticks > 0
uint8_t level_popup_active(const LevelState* s)
{
    return (s->popup_ticks > 0) ? 1u : 0u;
}

// count down popup timer
void level_popup_tick(LevelState* s)
{
    if (s->popup_ticks > 0)
    {
        s->popup_ticks--;
        if (s->popup_ticks == 0)
            s->popup_ended_pulse = 1;
    }
}

// one-shot pulse for clearing the popup box
uint8_t level_popup_just_ended(LevelSta
