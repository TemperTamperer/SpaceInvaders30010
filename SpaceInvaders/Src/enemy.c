#include "enemy.h"
#include "bullet.h"
#include <string.h>

void enemies_init(enemy enemy_pool[])
{
    memset(enemy_pool, 0, sizeof(enemy) * MAX_ENEMIES);
}

void enemies_reset(enemy enemy_pool[], EnemyShootState* st)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
        enemy_pool[i].alive = 0;

    st->shoot_counter = 0;
    st->next_enemy = 0;
}
void enemies_tick(enemy pool[],
                  uint16_t *move_counter,
                  uint16_t *spawn_counter,
                  uint8_t spawn_limit)
{
    (*move_counter)++;
    (*spawn_counter)++;

    if (*move_counter > 15)
    {
        *move_counter = 0;
        enemies_update_pos(pool);
    }

    if (*spawn_counter > spawn_limit)
    {
        *spawn_counter = 0;
        enemies_spawn(pool);
    }
}

void enemies_spawn(enemy enemy_pool[])
{
    const int SPAWN_AMOUNT = 6;
    const int SPAWN_STEP   = 10;

    int spawn_x = 2;

    for (int n = 0; n < SPAWN_AMOUNT; n++)
    {
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (!enemy_pool[i].alive)
            {
                enemy_pool[i].alive = 1;
                enemy_pool[i].hp = 1;

                enemy_pool[i].sx = 5;
                enemy_pool[i].sy = 3;

                enemy_pool[i].has_bonus = 0;

                enemy_pool[i].x = spawn_x;
                enemy_pool[i].y = 4;
                break;
            }
        }

        spawn_x += SPAWN_STEP;
        if (spawn_x > (SCREEN_COLS - 6))
            break;
    }
}

void enemies_update_pos(enemy enemy_pool[])
{
    for (int e = 0; e < MAX_ENEMIES; e++)
    {
        if (!enemy_pool[e].alive)
            continue;

        enemy_pool[e].y += 1;

#ifdef PLAYER_COLLISION_LINE
        if (enemy_pool[e].y >= PLAYER_COLLISION_LINE)
            enemy_pool[e].alive = 0;
#else
        if (enemy_pool[e].y >= (SCREEN_ROWS - 1))
            enemy_pool[e].alive = 0;
#endif
    }
}

void enemies_push_buffer(uint8_t buffer[SCREEN_ROWS][SCREEN_COLS], enemy enemy_pool[])
{
    uint8_t alien_lasher[3][5] = {
        {'(','X','X','X',')'},
        {')',' ','X',' ','('},
        {'(','X','V','X',')'},
    };

    for (int e = 0; e < MAX_ENEMIES; e++)
    {
        if (!enemy_pool[e].alive)
            continue;

        for (int i = 0; i < 3; i++)
        {
            int by = (int)enemy_pool[e].y - i;
            if (by < 0 || by >= SCREEN_ROWS)
                continue;

            for (int j = 0; j < 5; j++)
            {
                int bx = (int)enemy_pool[e].x + j;
                if (bx < 0 || bx >= SCREEN_COLS)
                    continue;

                uint8_t ch = alien_lasher[i][j];
                if (ch != 32)
                    buffer[by][bx] = ch;
            }
        }
    }
}

void enemies_shoot(enemy enemy_pool[],
                   Bullet* enemyBullets,
                   int enemyBullets_n,
                   EnemyShootState* st,
                   uint8_t level)
{
    uint16_t interval = 20;
    uint8_t shots_per_event = 1;

    if (level == 2)
    {
        interval = 12;
        shots_per_event = 2;
    }
    else if (level == 3)
    {
        interval = 8;
        shots_per_event = 3;
    }

    st->shoot_counter++;
    if (st->shoot_counter < interval)
        return;

    st->shoot_counter = 0;

    for (uint8_t s = 0; s < shots_per_event; s++)
    {
        for (int k = 0; k < MAX_ENEMIES; k++)
        {
            int i = (st->next_enemy + k) % MAX_ENEMIES;

            if (enemy_pool[i].alive)
            {
                bullets_shoot_enemy(enemyBullets, enemyBullets_n,
                                    (int)enemy_pool[i].x + 2,
                                    (int)enemy_pool[i].y + 1);

                st->next_enemy = (i + 1) % MAX_ENEMIES;
                break;
            }
        }
    }
}
