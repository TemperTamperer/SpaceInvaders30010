#include "level.h"

#define LEVEL2_SCORE 1000u
#define LEVEL3_SCORE 2000u
#define LEVEL_POPUP_TICKS 40u

void level_init(LevelState* s)
{
    s->level = 1;
    s->prev_level = 1;
    s->popup_ticks = 0;
    s->popup_ended_pulse = 0;
}

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

uint16_t level_spawn_limit(const LevelState* s)
{
    if (s->level == 3) return 45;
    if (s->level == 2) return 60;
    return 80;
}

uint8_t level_popup_active(const LevelState* s)
{
    return (s->popup_ticks > 0) ? 1u : 0u;
}

void level_popup_tick(LevelState* s)
{
    if (s->popup_ticks > 0)
    {
        s->popup_ticks--;
        if (s->popup_ticks == 0)
            s->popup_ended_pulse = 1;
    }
}

uint8_t level_popup_just_ended(LevelState* s)
{
    if (s->popup_ended_pulse)
    {
        s->popup_ended_pulse = 0;
        return 1u;
    }
    return 0u;
}
