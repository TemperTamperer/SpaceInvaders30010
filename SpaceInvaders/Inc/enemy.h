#ifndef ENEMY_H_
#define ENEMY_H_

#include <stdint.h>
#include "game_settings.h"
#include "player.h"

/* Forward declaration to avoid including bullet.h here */
typedef struct Bullet Bullet;

/* Enemy struct
   Stores position, sprite size and state */
typedef struct
{
    uint16_t x, y;     // Position on screen
    uint8_t  sx, sy;   // Sprite width and height
    uint8_t  hp;       // Enemy health
    uint8_t  alive;    // 1 = active, 0 = inactive
    uint8_t  has_bonus;
} enemy;

/* Keeps track of enemy shooting order and timing */
typedef struct
{
    uint16_t shoot_counter; // Counts ticks between shots
    int next_enemy;         // Index of next enemy that should shoot
} EnemyShootState;

/* Initialize all enemies (set inactive) */
void enemies_init(enemy enemy_pool[]);

/* Reset enemies and shooting state (used on level change / restart) */
void enemies_reset(enemy enemy_pool[], EnemyShootState* st);

/* Enemy tick function
   Updates movement and spawning using counters */
void enemies_tick(enemy pool[],
                  uint16_t *move_counter,
                  uint16_t *spawn_counter,
                  uint8_t spawn_limit,
                  player *p);

/* Spawns a new row of enemies at*
