#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>
#include <stdint.h>

#include "game_settings.h"
#include "enemy.h"

/* Bullet pool size */
#define BULLET_POOL_SIZE 50

/* Fixed point settings for bullet movement */
#define BULLET_FP_SHIFT 8
#define BULLET_FP (1 << BULLET_FP_SHIFT)

/* Bullet data */
typedef struct Bullet {
    int32_t x, y;     // position (fixed point)
    int32_t vx, vy;   // velocity (fixed point)
    bool active;      // in use
    uint8_t frame;    // animation frame
} Bullet;

/* Init and score */
void bullets_init(Bullet bullets[], int count);
void bullets_apply_kills_to_score(int kills,
                                  uint32_t *score,
                                  uint32_t *highscore);

/* Shooting */
void bullets_shoot_single(Bullet bullets[], int count, int x, int y);
void bullets_shoot_spread5(Bullet bullets[], int count, int x, int y);
void bullets_shoot_enemy(Bullet bullets[], int count, int x, int y);

/* Update and collision */
void bullets_update(Bullet bullets[], int count);
int bullets_hit_enemies(Bullet bullets[], int count, enemy enemy_pool[]);

/* Drawing */
void bullets_push_buffer(uint8_t buf[SCREEN_ROWS][SCREEN_COLS],
                         Bullet bullets[], int count);

#endif
