#include "powerup.h"

#define TICKS_PER_SECOND 20u
#define PU_SECONDS 5u
#define PU_TICKS (PU_SECONDS * TICKS_PER_SECOND)

#define SCORE_TRIPLE_POS 250u
#define SCORE_SPREAD_5   750u

#define TRIPLE_SHOT_OFFSET 5

// Internal helper
static void activate(PowerupState* s, PowerupType t)
{
    s->active = t;
    s->ticks_left = PU_TICKS;
}

// Init
void powerup_init(PowerupState* s)
{
    s->active = PU_NONE;
    s->ticks_left = 0;
    s->trig_250 = 0;
    s->trig_750 = 0;
}

// Unlock powerups based on score
void powerup_update_from_score(PowerupState* s, uint32_t score)
{
    if (!s->trig_250 && score >= SCORE_TRIPLE_POS)
    {
        s->trig_250 = 1;
        activate(s, PU_TRIPLE_POS);
    }

    if (!s->trig_750 && score >= SCORE_SPREAD_5)
    {
        s->trig_750 = 1;
        activate(s, PU_SPREAD_5);
    }
}

// Powerup timer
void powerup_tick(PowerupState* s)
{
    if (s->ticks_left > 0)
    {
        s->ticks_left--;
        if (s->ticks_left == 0)
            s->active = PU_NONE;
    }
}

// Shooting behavior
void powerup_shoot(PowerupState* s,
                   Bullet bullets[], int count,
                   uint8_t shoot_just_pressed,
                   int x_center, int y)
{
    if (!shoot_just_pressed) return;

    if (s->active == PU_TRIPLE_POS)
    {
        bullets_shoot_single(bullets, count, x_center, y);
        bullets_shoot_single(bullets, count, x_center - TRIPLE_SHOT_OFFSET, y);
        bullets_shoot_single(bullets, count, x_center + TRIPLE_SHOT_OFFSET, y);
        return;
    }

    if (s->active == PU_SPREAD_5)
    {
        bullets_shoot_spread5(bullets, count, x_center, y);
        return;
    }

    bullets_shoot_single(bullets, count, x_center, y);
}
