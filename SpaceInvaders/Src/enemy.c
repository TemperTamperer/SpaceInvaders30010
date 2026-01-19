#include "enemy.h"
#include "game_settings.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>   // rand()
#include "bullet.h"

static uint8_t bonus_exists(enemy enemy_pool[])
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_pool[i].alive && enemy_pool[i].has_bonus)
            return 1;
    }
    return 0;
}

/* bonus_ready siger "cooldown færdig, bonus må forsøges" */
static int bonus_spawn_ticks_left = 10800; // ~3 min ved 60 ticks/s
static uint8_t bonus_ready = 0;

void bonus_spawn_tick(enemy enemy_pool[])
{
    // hvis der allerede er en bonus enemy på skærmen, gør ingenting
    if (bonus_exists(enemy_pool))
        return;

    if (bonus_spawn_ticks_left > 0)
    {
        bonus_spawn_ticks_left--;
        return;
    }

    // cooldown færdig -> nu må bonus forsøges på kommende spawns
    bonus_ready = 1;
}

/* =========================
   SPAWN: spawn en HEL række ad gangen (som din gamle)
   ========================= */
void enemies_spawn(enemy enemy_pool[])
{
    // justér her hvis du vil have flere/færre pr række
    const int SPAWN_AMOUNT = 6;
    const int SPAWN_STEP   = 10;

    int spawn_x = 2;   // start altid samme sted (stabil række)

    for (int n = 0; n < SPAWN_AMOUNT; n++)
    {
        // find en ledig plads i poolen
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (!enemy_pool[i].alive)
            {
                enemy_pool[i].alive = 1;
                enemy_pool[i].has_bonus = 0;

                // Spawn position: samme y for hele rækken
                enemy_pool[i].x = spawn_x;
                enemy_pool[i].y = 4;

                // bonus er sjælden og kun efter cooldown
                if (bonus_ready && !bonus_exists(enemy_pool))
                {
                    if ((rand() % 20) == 0)
                    {
                        enemy_pool[i].has_bonus = 1;

                        // reset: tidligst ny bonus om ~3 min igen
                        bonus_ready = 0;
                        bonus_spawn_ticks_left = 10800;
                    }
                }

                break; // ud af "find slot" loop
            }
        }

        // næste enemy i rækken
        spawn_x += SPAWN_STEP;

        // stop hvis vi ikke har mere plads på linjen
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

        // Simpel “falder ned”
        enemy_pool[e].y += 1;

        // Hvis den går for langt ned, fjern den
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
            int by = enemy_pool[e].y - i;
            if (by < 0 || by >= SCREEN_ROWS)
                continue;

            for (int j = 0; j < 5; j++)
            {
                int bx = enemy_pool[e].x + j;
                if (bx < 0 || bx >= SCREEN_COLS)
                    continue;

                uint8_t ch = alien_lasher[i][j];
                if (ch != 32) // 32 = ' '
                    buffer[by][bx] = ch;
            }
        }
    }
}

/* =========================
   Fjender skyder
   ========================= */
void enemies_shoot(enemy enemy_pool[], Bullet enemyBullets[], int bullet_count)
{
    static uint16_t shoot_counter = 0;
    static int next_enemy = 0;

    shoot_counter++;
    if (shoot_counter < 20)   // lavere tal = skyder oftere
        return;

    shoot_counter = 0;

    for (int k = 0; k < MAX_ENEMIES; k++)
    {
        int i = (next_enemy + k) % MAX_ENEMIES;

        if (enemy_pool[i].alive)
        {
            bullets_shoot_enemy(enemyBullets, bullet_count,
                                (int)enemy_pool[i].x + 2,
                                (int)enemy_pool[i].y + 1);

            next_enemy = (i + 1) % MAX_ENEMIES;
            break;
        }
    }
}
