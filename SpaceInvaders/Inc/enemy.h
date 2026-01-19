#ifndef ENEMY_H_
#define ENEMY_H_

#include <stdint.h>
#include "game_settings.h"
#include "player.h"

typedef struct Bullet Bullet;
typedef struct
{
    uint16_t x, y;      // position
    uint8_t  sx, sy;    // width / height
    uint8_t  hp;        // health points
    uint8_t  alive;     // 1 = alive, 0 = dead
    uint8_t  has_bonus; // 1 = enemy giver powerup
} enemy;

void enemies_push_buffer(uint8_t buffer[SCREEN_ROWS][SCREEN_COLS],
                         enemy enemy_pool[MAX_ENEMIES]);

void enemies_update_pos(enemy enemy_pool[MAX_ENEMIES]);
void enemies_spawn(enemy enemy_pool[MAX_ENEMIES]);
void enemies_shoot(enemy enemy_pool[],
                   Bullet *enemyBullets,
                   int bullet_count);

#endif /* ENEMY_H_ */
