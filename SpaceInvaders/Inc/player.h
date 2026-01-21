#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdint.h>
#include "game_settings.h"

struct Bullet;
typedef struct {
    int x, y;
    int sx, sy;
    uint8_t hp;
    uint8_t alive;
    uint8_t hit_count;
} player;

void player_init(player *p);
void player_get_shoot_pos(const player *p, int *x, int *y);
void player_update_pos(uint8_t input, player *p1);
void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player p1);
void player_condition(player *p);

void player_hit_by_enemy_bullets(struct Bullet *enemyBullets,
                                 int count,
                                 player *p);

#endif
