#include "powerup.h"
#include "player.h"

#define TICKS_PER_SECOND 20u
#define PU_SECONDS 5u
#define PU_TICKS (PU_SECONDS * TICKS_PER_SECOND)

#define SCORE_TRIPLE_POS 250u
#define SCORE_SPREAD_5   750u

// activate powerup and reset timer
static void activate(PowerupState* s, PowerupType t)
{
    s->active = t;
    s->ticks_left = PU_TICKS;
}

// init powerup state
void powerup_init(PowerupState* s)
{
    s->active = PU_NONE;
    s->ticks_left = 0;
    s->trig_250 = 0;
    s->trig_750 = 0;
}

// unlock powerups based on score
void powerup_update_from_score(PowerupState* s, uint32_t score)
{
    if (!s->trig_250 && score >= SCORE_TRIPLE_POS)
    {
        s->trig_250 = 1;
        activate(s, PU_TRIPLE_POS);
    }

    if (!s->
