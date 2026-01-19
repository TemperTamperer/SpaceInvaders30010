#include "bullet.h"
#include "game_settings.h"   // SCREEN_ROWS / COLS

void bullets_init(bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
        bullets[i].active = 0;
}

void bullets_fire(bullet bullets[], int start_x, int start_y)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].x = start_x;
            bullets[i].y = start_y;
            bullets[i].dy = -1;   // up
            bullets[i].active = 1;
            break;
        }
    }
}

void bullets_update(bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].y += bullets[i].dy;

            if (bullets[i].y <= 0)
                bullets[i].active = 0;
        }
    }
}

void bullets_draw(bullet bullets[], char buffer[][SCREEN_COLS])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            buffer[bullets[i].y][bullets[i].x] = '|';
        }
    }
}
