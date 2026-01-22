#include "powerup.h"
#include "bullet.h"

#define TICKS_PER_SECOND 20u

#define PU_SECONDS_TRIPLE   10u
#define PU_SECONDS_SPREAD5   5u

#define PU_TICKS_TRIPLE  (PU_SECONDS_TRIPLE  * TICKS_PER_SECOND)
#define PU_TICKS_SPREAD5 (PU_SECONDS_SPREAD5 * TICKS_PER_SECOND)


#define TRIPLE_STEP_SCORE  30u
#define SPREAD_STEP_SCORE  60u

#define COMP_OFFSET_X 8

static void activate(PowerupState* s, PowerupType t)
{
    s->active = t;

    if (t == PU_TRIPLE_POS) {
        s->ticks_left = PU_TICKS_TRIPLE;
        s->comp_ticks_left = PU_TICKS_TRIPLE;
    } else if (t == PU_SPREAD_5) {
        s->ticks_left = PU_TICKS_SPREAD5;
        s->comp_ticks_left = 0;
    } else {
        s->ticks_left = 0;
        s->comp_ticks_left = 0;
    }
}

void powerup_init(PowerupState* s)
{
    s->active = PU_NONE;
    s->ticks_left = 0;
    s->comp_ticks_left = 0;


    s->next_triple_score = TRIPLE_STEP_SCORE;   // 30
    s->next_spread_score = SPREAD_STEP_SCORE;   // 60
}

void powerup_update_from_score(PowerupState* s, uint32_t score)
{
    while (score >= s->next_triple_score) {
        activate(s, PU_TRIPLE_POS);
        s->next_triple_score += TRIPLE_STEP_SCORE;
    }

    while (score >= s->next_spread_score) {
        activate(s, PU_SPREAD_5);
        s->next_spread_score += SPREAD_STEP_SCORE;
    }
}

void powerup_tick(PowerupState* s)
{
    if (s->ticks_left > 0) {
        s->ticks_left--;

        if (s->active == PU_TRIPLE_POS && s->comp_ticks_left > 0) {
            s->comp_ticks_left--;
        }

        if (s->ticks_left == 0) {
            s->active = PU_NONE;
            s->comp_ticks_left = 0;
        }
    }
}

void powerup_shoot(PowerupState* s,
                   Bullet *bullets, int count,
                   uint8_t shoot_just_pressed,
                   int x_center, int y)
{
    if (!shoot_just_pressed) return;

    if (s->active == PU_SPREAD_5) {
        bullets_shoot_spread5(bullets, count, x_center, y);
        return;
    }

    if (s->active == PU_TRIPLE_POS && s->comp_ticks_left > 0) {
        bullets_shoot_single(bullets, count, x_center, y);                      // dig
        bullets_shoot_single(bullets, count, x_center - COMP_OFFSET_X, y);      // venstre “medskaber”
        bullets_shoot_single(bullets, count, x_center + COMP_OFFSET_X, y);      // højre “medskaber”
        return;
    }

    bullets_shoot_single(bullets, count, x_center, y);
}

uint8_t powerup_get_companions(const PowerupState* s,
                               int x_center, int y,
                               int *lx, int *ly,
                               int *rx, int *ry)
{
    if (s->active != PU_TRIPLE_POS) return 0;
    if (s->comp_ticks_left == 0) return 0;

    *lx = x_center - 8;
    *rx = x_center + 8;
    *ly = y;
    *ry = y;

    return 1;
}
