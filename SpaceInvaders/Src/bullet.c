#include "bullet.h"

static Bullet* find_inactive(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (!bullets[i].active)
            return &bullets[i];
    }
    return 0;
}

static const char bullet_anim[] = {
    '|',
    '\x18',  // ↑ (CP850)
    '*',
    '.'
};

#define BULLET_ANIM_FRAMES (sizeof(bullet_anim))


void bullets_apply_kills_to_score(int kills, uint32_t *score, uint32_t *highscore)
{
    if (kills <= 0) return;

    *score += (uint32_t)(kills * 10u);
    if (*score > *highscore) *highscore = *score;
}

void bullets_init(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        bullets[i].x = 0;
        bullets[i].y = 0;
        bullets[i].vx = 0;
        bullets[i].vy = 0;
        bullets[i].active = false;
    }
}

void bullets_shoot_single(Bullet bullets[], int x, int y, int count)
{
	for (int i = 0; i < count; i++)
	    {
	        if (!bullets[i].active)
	        {
	            bullets[i].x = x;
	            bullets[i].y = y;
	            bullets[i].vx = 0;
	            bullets[i].vy = -1;
	            bullets[i].frame = 0;   // start animation
	            bullets[i].active = true;
	            break;
	        }
	    }
}

void bullets_shoot_enemy(Bullet bullets[], int count, int x, int y)
{
    Bullet *b = find_inactive(bullets, count);
    if (!b) return;

    b->x = (int32_t)x * BULLET_FP;
    b->y = (int32_t)y * BULLET_FP;
    b->vx = 0;
    b->vy = +1 * BULLET_FP;
    b->active = true;
}

void bullets_shoot_spread5(Bullet bullets[], int count, int x, int y)
{
    const int32_t vx[5] = {
        -(BULLET_FP / 2),
        -(BULLET_FP / 4),
        0,
        (BULLET_FP / 4),
        (BULLET_FP / 2)
    };
    const int32_t vy[5] = {
        -(BULLET_FP * 3) / 4,
        -(BULLET_FP * 9) / 10,
        -BULLET_FP,
        -(BULLET_FP * 9) / 10,
        -(BULLET_FP * 3) / 4
    };

    for (int i = 0; i < 5; i++)
    {
        Bullet *b = find_inactive(bullets, count);
        if (!b) return;

        b->x = (int32_t)x * BULLET_FP;
        b->y = (int32_t)y * BULLET_FP;
        b->vx = vx[i];
        b->vy = vy[i];
        b->active = true;
    }
}

void bullets_update(Bullet bullets[], int count)
{
	 for (int i = 0; i < count; i++)
	    {
	        if (!bullets[i].active)
	            continue;

	        bullets[i].y += bullets[i].vy;
	        bullets[i].x += bullets[i].vx;

	        bullets[i].frame++;
	        if (bullets[i].frame >= BULLET_ANIM_FRAMES)
	            bullets[i].frame = 0;

	        if (bullets[i].y <= 0)
	            bullets[i].active = true;
	    }
}

int bullets_hit_enemies(Bullet bullets[], int count, enemy enemy_pool[])
{
    int kills = 0;

    for (int bi = 0; bi < count; bi++)
    {
        Bullet *b = &bullets[bi];
        if (!b->active) continue;

        int bx = (int)(b->x >> BULLET_FP_SHIFT);
        int by = (int)(b->y >> BULLET_FP_SHIFT);

        for (int ei = 0; ei < MAX_ENEMIES; ei++)
        {
            if (!enemy_pool[ei].alive) continue;

            int ex = enemy_pool[ei].x;
            int ey = enemy_pool[ei].y;

            int left   = ex;
            int right  = ex + 4;
            int top    = ey - 2;
            int bottom = ey;

            if (bx >= left && bx <= right &&
                by >= top  && by <= bottom)
            {
                enemy_pool[ei].alive = 0;
                b->active = false;
                kills++;
                break;
            }
        }
    }

    return kills;
}



void bullets_push_buffer(uint8_t buffer[SCREEN_ROWS][SCREEN_COLS], Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        Bullet *b = &bullets[i];
        if (!b->active) continue;

        if (bullets[i].y > 0 && bullets[i].y < SCREEN_ROWS &&
                    bullets[i].x > 0 && bullets[i].x < SCREEN_COLS)
                {
                    buffer[bullets[i].y][bullets[i].x] =
                        bullet_anim[bullets[i].frame];
        }
    }
}
