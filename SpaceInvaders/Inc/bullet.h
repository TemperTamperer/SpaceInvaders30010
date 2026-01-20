#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>
#include <stdint.h>

#include "game_settings.h"
#include "enemy.h"

#define BULLET_POOL_SIZE 50

#define BULLET_FP_SHIFT 8
#define BULLET_FP (1 << BULLET_FP_SHIFT)

typedef struct Bullet {
    int32_t x, y;
    int32_t vx, vy;
    bool active;
} Bullet;

void bullets_init(Bullet bullets[], int count);

void bullets_shoot_single(Bullet bullets[], int count, int x, int y);
void bullets_shoot_spread5(Bullet bullets[], int count, int x, int y);
void bullets_shoot_enemy(Bullet bullets[], int count, int x, int y);

void bullets_update(Bullet bullets[], int count);

int bullets_hit_enemies(Bullet bullets[], int count, enemy enemy_pool[]);

void bullets_push_buffer(uint8_t buf[SCREEN_ROWS][SCREEN_COLS], Bullet bullets[], int count);

#endif
