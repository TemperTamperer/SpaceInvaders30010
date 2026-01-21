#include "player.h"
#include "joystick.h"
#include "bullet.h"
void player_get_shoot_pos(const player *p, int *x, int *y)
{
    *x = p->x + (p->sx / 2);
    *y = p->y - 1;
}
void player_init(player *p)
{
    p->x = 50;
    p->y = SCREEN_ROWS - 1;
    p->sx = 5;
    p->sy = 3;
    p->hp = 3;
    p->hit_count = 0;
}
void player_update_pos(uint8_t input, player *p1)
{
    if (input & JOY_LEFT)
    {
        if (p1->x > 1)
            p1->x--;
    }

    if (input & JOY_RIGHT)
    {
        if (p1->x < SCREEN_COLS - p1->sx - 1)
            p1->x++;
    }
}

void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player p)
{
    /* Player sprite:
         ^
        / \
       /═█═\
    */

    uint8_t player_sprite[3][5] = {
        {' ', ' ', '^', ' ', ' '},
        {' ', '/', ' ', '\\', ' '},
        {'/', 205, 219, 205, '\\'}
    };

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            buffer[p.y - i][p.x + j] = player_sprite[p.sy - i - 1][j];
        }
    }
} // ✅ VIGTIG: lukker player_push_buffer korrekt her
void player_hit_by_enemy_bullets(Bullet *enemyBullets,
                                 int count,
                                 player *p)
{
    int left   = p->x - 3;
    int right  = p->x + p->sx + 3;
    int bottom = p->y;
    int top    = p->y - p->sy - 2;

    for (int i = 0; i < count; i++)
    {
        if (!enemyBullets[i].active)
            continue;

        int bx = (int)(enemyBullets[i].x >> BULLET_FP_SHIFT);
        int by = (int)(enemyBullets[i].y >> BULLET_FP_SHIFT);

        if (bx >= left && bx <= right &&
            by >= top  && by <= bottom)
        {
            enemyBullets[i].active = 0;

            p->hit_count++;

            if (p->hit_count >= 5)
            {
                p->hit_count = 0;
                if (p->hp > 0)
                    p->hp--;
            }

            return;
        }
    }
}
	/*
	//Following is the charecters pushed to the buffer at their respective x,y coordinates
	buffer[p1.y - 2][p1.x + 2] = '^';

	buffer[p1.y - 1][p1.x + 1] = '/';
	buffer[p1.y - 1][p1.x + 4] = '\\';

	buffer[p1.y][p1.x + 0] = '/';
	buffer[p1.y][p1.x + 1] = '=';
	buffer[p1.y][p1.x + 2] = '█';
	buffer[p1.y][p1.x + 3] = '=';
	buffer[p1.y][p1.x + 4] = '\\';
	*/
