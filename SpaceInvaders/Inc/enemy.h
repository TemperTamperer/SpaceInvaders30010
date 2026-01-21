#ifndef ENEMY_H_
#define ENEMY_H_


#include <stdint.h>
#include "game_settings.h"
#include "player.h"

typedef struct Bullet Bullet;

typedef struct
{
    uint16_t x, y;
    uint8_t  sx, sy;
    uint8_t  hp;
    uint8_t  alive;
    uint8_t  has_bonus;
} enemy;

typedef struct
{
    uint16_t shoot_counter;
    int next_enemy;
} EnemyShootState;

void enemies_init(enemy enemy_pool[]);
void enemies_reset(enemy enemy_pool[], EnemyShootState* st);

//void enemies_spawn(enemy enemy_pool[]);
//void enemies_update_pos(enemy enemy_pool[]);
void enemies_push_buffer(uint8_t buffer[SCREEN_ROWS][SCREEN_COLS], enemy enemy_pool[]);

void enemies_update_pos(enemy enemy_pool[MAX_ENEMIES]);
uint8_t enemies_player_collision(enemy enemy_pool[MAX_ENEMIES], player *p);
void enemies_spawn(enemy enemy_pool[MAX_ENEMIES]);

void enemies_shoot(enemy enemy_pool[],
                   Bullet* enemyBullets,
                   int enemyBullets_n,
                   EnemyShootState* st,
                   uint8_t level);


#endif /* ENEMY_H_ */
