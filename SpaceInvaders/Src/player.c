#include "player.h"
#include "joystick.h"
#include "bullet.h"

#define PLAYER_START_X 50
#define PLAYER_HP_MAX  3

#define HIT_LIMIT      5
#define HITBOX_PAD_X   3
#define HITBOX_PAD_TOP 2

// Player spawn/shoot helpers
void player_get_shoot_pos(const player *p, int *x, int *y)
{
    *x = p->x + (p->sx / 2);
    *y = p->y - 1;
}

void player_init(player *p)
{
    p->x = PLAYER_START_X;
    p->y = SCREEN_ROWS - 1;
    p->sx = 5;
    p->sy = 3;
    p->hp = PLAYER_HP_MAX;
    p->hit_count = 0;
}

// Player movement
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

// Draw player
void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player p)
{
    static const uint8_t player_sprite[3][5] = {
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
}

// Check if enemy bullets hit the player
uint8_t player_hit_by_enemy_bullets(Bullet *enemyBullets,
                                    int count,
                                    player *p)
{
    int left   = p->x - HITBOX_PAD_X;
    int right  = p->x + p->sx + HITBOX_PAD_X;
    int bottom = p->y;
    int top    = p->y - p->sy - HITBOX_PAD_TOP;

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

            if (p->hit_count >= HIT_LIMIT)
            {
                p->hit_count = 0;
                if (p->hp > 0)
                    p->hp--;
            }

            return 1;
        }
    }
    return 0;
}
