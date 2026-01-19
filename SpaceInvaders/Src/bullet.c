#include "bullet.h"

static int g_powerup_ticks_left = 0;
static int g_test_hit_counter = 0;
static Bullet bullets[BULLET_POOL_SIZE];

Bullet* bullets_get_pool(void)
{
    return bullets;
}

int bullets_get_count(void)
{
    return BULLET_POOL_SIZE;
}


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
        bullets[i].x = 0;
        bullets[i].y = 0;
        bullets[i].vx = 0;
        bullets[i].vy = 0;
        bullets[i].active = false;
    }
}
void bullets_shoot_single(Bullet bullets[], int count, int x, int y)
{
    Bullet *b = find_inactive(bullets, count);
    if (!b) return;

    b->x = (int32_t)x * BULLET_FP;
    b->y = (int32_t)y * BULLET_FP;
    b->vx = 0;
    b->vy = -1 * BULLET_FP;   // -1 celle pr tick
    b->active = true;
}
void bullets_shoot_enemy(Bullet bullets[], int count, int x, int y)
{
    Bullet *b = find_inactive(bullets, count);
    if (!b) return;

    b->x = (int32_t)x * BULLET_FP;
    b->y = (int32_t)y * BULLET_FP;
    b->vx = 0;
    b->vy = +1 * BULLET_FP;   // enemy: nedad
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
        Bullet *b = &bullets[i];
        if (!b->active) continue;

        b->x += b->vx;
        b->y += b->vy;

        // konverter til celler for bounds
        int bx = (int)(b->x >> BULLET_FP_SHIFT);
        int by = (int)(b->y >> BULLET_FP_SHIFT);

        if (by < 0 || by >= SCREEN_ROWS || bx < 0 || bx >= SCREEN_COLS)
            b->active = false;
    }
}

int bullets_hit_enemies(Bullet bullets[], int count, enemy enemy_pool[], int *bonus_collected)
{
    int kills = 0;
    if (bonus_collected) *bonus_collected = 0;

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
                if (enemy_pool[ei].has_bonus)
                {
                    if (bonus_collected) *bonus_collected = 1;
                    enemy_pool[ei].has_bonus = 0;
                }

                enemy_pool[ei].alive = 0;
                b->active = false;

                kills++;
                g_test_hit_counter++;
                break;
            }
        }
    }

    return kills;
}

void bullets_push_buffer(uint8_t buf[SCREEN_ROWS][SCREEN_COLS], Bullet bullets[], int count)
{
    for (int i = 0; i < count; i++)
    {
        Bullet *b = &bullets[i];
        if (!b->active) continue;

        int bx = (int)(b->x >> BULLET_FP_SHIFT);
        int by = (int)(b->y >> BULLET_FP_SHIFT);

        if (bx >= 0 && bx < SCREEN_COLS &&
            by >= 0 && by < SCREEN_ROWS)
        {
            buf[by][bx] = '|';
        }
    }
}

void bullets_powerup_activate(int ticks)
{
    g_powerup_ticks_left = ticks;
}

void bullets_powerup_tick(void)
{
    if (g_powerup_ticks_left > 0)
        g_powerup_ticks_left--;
}

void bullets_handle_shoot(Bullet bullets[], int count, int shoot_just_pressed, int x, int y)
{
    if (!shoot_just_pressed) return;

    if (g_powerup_ticks_left > 0)
        bullets_shoot_spread5(bullets, count, x, y);
    else
        bullets_shoot_single(bullets, count, x, y);
}

int bullets_test_should_powerup(int threshold)
{
    if (g_test_hit_counter >= threshold)
    {
        g_test_hit_counter = 0;
        return 1;
    }
    return 0;
}
