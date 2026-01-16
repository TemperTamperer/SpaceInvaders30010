#include "enemy.h"
#include "game_settings.h"
#include <string.h>
#include <stdint.h>

static uint8_t bonus_exists(enemy enemy_pool[])
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemy_pool[i].alive && enemy_pool[i].has_bonus)
            return 1;
    }
    return 0;
}


void enemies_spawn(enemy enemy_pool[])
{
    static int spawn_x = 2;   // simpel “rul” hen over skærmen

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemy_pool[i].alive)
        {
            enemy_pool[i].alive = 1;
            enemy_pool[i].has_bonus = 0;

            // Spawn position (simpel og stabil)
            enemy_pool[i].x = spawn_x;
            enemy_pool[i].y = 4;  // top-område (bunden af spriten)

            // næste spawn flytter sig mod højre
            spawn_x += 10;
            if (spawn_x > (SCREEN_COLS - 6))
                spawn_x = 2;

            // Sørg for præcis én bonus-enemy ad gangen
            if (!bonus_exists(enemy_pool))
                enemy_pool[i].has_bonus = 1;

            break;
        }
    }
}

/*
Flytter enemies nedad.
- Kun alive enemies opdateres
- Hvis de når for langt ned, dør de (alive = 0)
*/
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
        // fallback hvis I ikke bruger PLAYER_COLLISION_LINE
        if (enemy_pool[e].y >= (SCREEN_ROWS - 1))
            enemy_pool[e].alive = 0;
#endif
    }
}

/*
Tegner enemies ind i buffer.
- Tegner kun hvis alive=1
- has_bonus er skjult (ingen speciel rendering)
*/
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
                if (ch != 32) // 32 = ' ' (tegn ikke spaces)
                    buffer[by][bx] = ch;
            }
        }
    }
}
