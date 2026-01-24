#include "bullet.h"

// find first inactive bullet in pool
static Bullet* find_inactive(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (!bullets[i].active)
            return &bullets[i];
    }
    return 0;
}

// apply score from number of kills
void bullets_apply_kills_to_score(int kills, uint32_t *score, uint32_t *highscore)
{
    if (kills <= 0) return;

    *score += (uint32_t)(kills * 10u);
    if (*score > *highscore) *highscore = *score;
}

// initialize bullet pool
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

// player single-shot bullet
void bullets_shoot_single(Bullet bullets[], int count, int x, int y)
{
    Bullet *b = find_inactive(bullets, count);
    if (!b) return;

    b->x = (int32_t)x * BULLET_FP;
    b->y = (int32_t)y * BULLET_FP;
    b->vx = 0;
    b->vy = -1 * BULLET_FP;
    b->active = true;
    b->frame = 0;
}

// enemy downward bullet
void bullets_shoot_enemy(Bullet bullets[], int count, int x, int y)
{
    Bul
