#include "powerup.h"
#include "player.h"

#define TICKS_PER_SECOND 20u
#define PU_SECONDS 5u
#define PU_TICKS (PU_SECONDS * TICKS_PER_SECOND)

#define SCORE_TRIPLE_POS 250u
#define SCORE_SPREAD_5   750u

static void activate(PowerupState* s, PowerupType t)
{
    s->active = t;
    s->ticks_left = PU_TICKS;
}

void powerup_init(PowerupState* s)
{
    s->active = PU_NONE;
    s->ticks_left = 0;
    s->trig_250 = 0;
    s->trig_750 = 0;
}

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

void powerup_tick(PowerupState* s)
{
    if (s->ticks_left > 0)
    {
        s->ticks_left--;
        if (s->ticks_left == 0)
            s->active = PU_NONE;
    }
}

void powerup_shoot(PowerupState* s,
                   Bullet bullets[], int count,
                   uint8_t shoot_just_pressed,
				   player p,
				   uint8_t buffer[][SCREEN_COLS])
{
    if (!shoot_just_pressed) return;

     int8_t x = p.x + (p.sx >> 1);
     int8_t y = p.y - 1;

    if (s->active == PU_TRIPLE_POS)
    {
        bullets_shoot_single(bullets, count, x, y);

        p.x = p.x - 5;
        x = p.x + (p.sx >> 1);
        y = p.y - 1;
        bullets_shoot_single(bullets, count, x, y);
        //player_push_buffer(buffer, p);

        p.x = p.x + 10;
        x = p.x + (p.sx >> 1);
        y = p.y - 1;
        bullets_shoot_single(bullets, count, x, y);
        //player_push_buffer(buffer, p);
        return;
    }

    if (s->active == PU_SPREAD_5)
    {
        bullets_shoot_spread5(bullets, count, x, y);
        return;
    }

    bullets_shoot_single(bullets, count, x, y);
}
