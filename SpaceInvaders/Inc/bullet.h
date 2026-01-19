#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>
#include <stdint.h>

#include "game_settings.h"
#include "enemy.h"

#define BULLET_POOL_SIZE 50

// Fixed-point skala: 1 celle = 256 enheder (hurtigt pga. shift)
#define BULLET_FP_SHIFT 8
#define BULLET_FP (1 << BULLET_FP_SHIFT)

typedef struct Bullet {
    int32_t x, y;     // fixed-point (Q24.8-ish)
    int32_t vx, vy;   // fixed-point per tick
    bool active;
} Bullet;
Bullet* bullets_get_pool(void);
int bullets_get_count(void);
void bullets_init(Bullet bullets[], int count);

// shoot med int (celle-koordinater)
void bullets_shoot_single(Bullet bullets[], int count, int x, int y);
void bullets_shoot_spread5(Bullet bullets[], int count, int x, int y);
void bullets_shoot_enemy(Bullet bullets[], int count, int x, int y);
// update pr tick (ingen dt)
void bullets_update(Bullet bullets[], int count);

// returnerer kills, sætter *bonus_collected = 1 hvis bonus-enemy blev ramt
int bullets_hit_enemies(Bullet bullets[], int count, enemy enemy_pool[], int *bonus_collected);

void bullets_push_buffer(uint8_t buf[SCREEN_ROWS][SCREEN_COLS], Bullet bullets[], int count);

void bullets_powerup_activate(int ticks);
void bullets_powerup_tick(void);

void bullets_handle_shoot(Bullet bullets[], int count, int shoot_just_pressed, int x, int y);

int bullets_test_should_powerup(int threshold);

#endif
