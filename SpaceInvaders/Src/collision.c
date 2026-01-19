#include "collision.h"

void bullets_vs_enemies(bullet bullets[], enemy enemies[], int enemy_count)
{
    for (int b = 0; b < MAX_BULLETS; b++)
    {
        if (!bullets[b].active)
            continue;

        for (int e = 0; e < enemy_count; e++)
        {
            if (!enemies[e].alive)
                continue;

            // AABB collision check
            if (bullets[b].x >= enemies[e].x &&
                bullets[b].x <  enemies[e].x + enemies[e].sx &&
                bullets[b].y >= enemies[e].y &&
                bullets[b].y <  enemies[e].y + enemies[e].sy)
            {
                // Collision!
                bullets[b].active = 0;
                enemies[e].alive = 0;
                break;
            }
        }
    }
}
