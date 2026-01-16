#include "bullet.h"

/* powerup state ligger nu her (ikke i main.c) */
static int g_powerup_ticks_left = 0;

static Bullet* find_inactive(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (!bullets[i].active)
            return &bullets[i];
    }
    return 0;
}

void bullets_init(Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        bullets[i].x = 0.0f;
        bullets[i].y = 0.0f;
        bullets[i].vx = 0.0f;
        bullets[i].vy = 0.0f;
        bullets[i].active = false;
    }
}

void bullets_shoot_single(Bullet bullets[], int count, float x, float y)
{
    Bullet *b = find_inactive(bullets, count);
    if (!b) return;

    b->x = x;
    b->y = y;
    b->vx = 0.0f;
    b->vy = -20.0f;
    b->active = true;
}

void bullets_shoot_spread5(Bullet bullets[], int count, float x, float y)
{
    const float vx[5] = { -10.0f, -5.0f, 0.0f, 5.0f, 10.0f };
    const float vy[5] = { -15.0f, -18.0f, -20.0f, -18.0f, -15.0f };

    for (int i = 0; i < 5; i++)
    {
        Bullet *b = find_inactive(bullets, count);
        if (!b) return;

        b->x = x;
        b->y = y;
        b->vx = vx[i];
        b->vy = vy[i];
        b->active = true;
    }
}

void bullets_update(Bullet bullets[], int count, float dt)
{
    for (int i = 0; i < count; i++)
    {
        Bullet *b = &bullets[i];
        if (!b->active) continue;

        b->x += b->vx * dt;
        b->y += b->vy * dt;

        if (b->y < 1.0f || b->x < 0.0f || b->x >= (float)SCREEN_COLS)
            b->active = false;
    }
}

/* Returnerer 1 hvis bonus-enemy blev ramt */
int bullets_hit_enemies(Bullet bullets[], int count, enemy enemy_pool[])
{
    int bonus_collected = 0;

    for (int bi = 0; bi < count; bi++)
    {
        Bullet *b = &bullets[bi];
        if (!b->active) continue;

        int bx = (int)b->x;
        int by = (int)b->y;

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
                if (enemy_pool[ei].has_bonus)
                {
                    bonus_collected = 1;
                    enemy_pool[ei].has_bonus = 0;
                }

                enemy_pool[ei].alive = 0;
                b->active = false;
                break;
            }
        }
    }

    return bonus_collected;
}

void bullets_push_buffer(uint8_t buf[SCREEN_ROWS][SCREEN_COLS], Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        Bullet *b = &bullets[i];
        if (!b->active) continue;

        int bx = (int)b->x;
        int by = (int)b->y;

        if (bx >= 0 && bx < SCREEN_COLS &&
            by >= 0 && by < SCREEN_ROWS)
        {
            buf[by][bx] = '|';
        }
    }
}

/* --- NYT: powerup styres her --- */
void bullets_powerup_activate(int ticks)
{
    g_powerup_ticks_left = ticks;
}

void bullets_powerup_tick(void)
{
    if (g_powerup_ticks_left > 0)
        g_powerup_ticks_left--;
}

void bullets_handle_shoot(Bullet bullets[], int count, int shoot_just_pressed, float x, float y)
{
    if (!shoot_just_pressed) return;

    if (g_powerup_ticks_left > 0)
        bullets_shoot_spread5(bullets, count, x, y);
    else
        bullets_shoot_single(bullets, count, x, y);
}
