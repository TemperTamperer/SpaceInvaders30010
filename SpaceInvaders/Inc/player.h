#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdint.h>
#include "game_settings.h"
#include "joystick.h"

struct Bullet;
struct PowerupState; // Forward declaration
typedef struct PowerupState PowerupState;

struct Bullet;
typedef struct player{
    int x, y;
    int sx, sy;
    uint8_t hp;
    uint8_t alive;
    uint8_t hit_count;
} player;

void player_init(player *p);
void player_get_shoot_pos(const player *p, int *x, int *y);
void player_update_pos(uint8_t input, player *p1);
void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player p1, PowerupState *s);
void triple_player_push_buffer(uint8_t buffer[][SCREEN_COLS], player p1);
void player_condition(player *p);

uint8_t player_hit_by_enemy_bullets(struct Bullet *enemyBullets,
                                 int count,
                                 player *p);

#endif
