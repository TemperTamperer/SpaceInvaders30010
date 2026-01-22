#include "enemy.h"
#include "bullet.h"
#include <string.h>
#include "player.h"

#define MOVE_TICK_LIMIT 15

#define SPAWN_AMOUNT 6
#define SPAWN_STEP   10
#define SPAWN_START_X 2

#define ENEMY_W 5
#define ENEMY_H 3

#define ENEMY_SPAWN_Y 4
#define ENEMY_SHOOT_X_OFFSET 2
#define ENEMY_SHOOT_Y_OFFSET 1

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

/* Tick: move + spawn */
void enemies_tick(enemy pool[],
                  uint16_t *move_counter,
                  uint16_t *spawn_counter,
                  uint8_t spawn_limit,
                  player *p)
{
    (*move_counter)++;
    (*spawn_counter)++;

    if (*move_counter > MOVE_TICK_LIMIT)
    {
        *move_counter = 0;
        enemies_update_pos(pool, p);
    }

    if (*spawn_counter > spawn_limit)
    {
        *spawn_counter = 0;
        enemies_spawn(pool);
    }
}

/* Spawn a row of enemies */
void enemies_spawn(enemy enemy_pool[])
{
    int spawn_x = SPAWN_START_X;

    for (int n = 0; n < SPAWN_AMOUNT; n++)
    {
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (!enemy_pool[i].alive)
            {
                enemy_pool[i].alive = 1;
                enemy_pool[i].hp = 1;

                enemy_pool[i].sx = ENEMY_W;
                enemy_pool[i].sy = ENEMY_H;

                enemy_pool[i].has_bonus = 0;

                enemy_pool[i].x = spawn_x;
                enemy_pool[i].y = ENEMY_SPAWN_Y;
                break;
            }
        }

        spawn_x += SPAWN_STEP;
        if (spawn_x > (SCREEN_COLS - (ENEMY_W + 1)))
            break;
    }
}

/* Move enemies down */
void enemies_update_pos(enemy enemy_pool[], player *p)
{
    for (int e = 0; e < MAX_ENEMIES; e++)
    {
        if (!enemy_pool[e].alive)
            continue;

        enemy_pool[e].y += 1;

#ifdef PLAYER_COLLISION_LINE
        if (enemy_pool[e].y >= PLAYER_COLLISION_LINE) {
            enemy_pool[e].alive = 0;
            p->hit_count++;
        }
#else
        if (enemy_pool[e].y >= (SCREEN_ROWS - 1))
            enemy_pool[e].alive = 0;
#endif
    }
}

/* Draw enemies into buffer */
void enemies_push_buffer(uint8_t buffer[SCREEN_ROWS][SCREEN_COLS], enemy enemy_pool[])
{
    static const uint8_t alien_lasher[3][5] = {
        {'(','X','X','X',')'},
        {')',' ','X',' ','('},
        {'(','X','V','X',')'},
    };

    for (int e = 0; e < MAX_ENEMIES; e++)
    {
        if (!enemy_pool[e].alive)
            continue;

        for (int i = 0; i < ENEMY_H; i++)
        {
            int by = (int)enemy_pool[e].y - i;
            if (by < 0 || by >= SCREEN_ROWS)
                continue;

            for (int j = 0; j < ENEMY_W; j++)
            {
                int bx = (int)enemy_pool[e].x + j;
                if (bx < 0 || bx >= SCREEN_COLS)
                    continue;

                uint8_t ch = alien_lasher[i][j];
                if (ch != ' ')
                    buffer[by][bx] = ch;
            }
        }
    }
}

/* Enemy shooting */
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
                                    (int)enemy_pool[i].x + ENEMY_SHOOT_X_OFFSET,
                                    (int)enemy_pool[i].y + ENEMY_SHOOT_Y_OFFSET);

                st->next_enemy = (i + 1) % MAX_ENEMIES;
                break;
            }
        }
    }
}
