#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>
#include <stdint.h>

#include "game_settings.h"
#include "enemy.h"

#define BULLET_POOL_SIZE 5

typedef struct Bullet {
    float x, y;
    float vx, vy;     // retning/hastighed
    bool active;
} Bullet;

void bullets_init(Bullet bullets[], int count);

// skyder 1 bullet (normal)
void bullets_shoot_single(Bullet bullets[], int count, float x, float y);

// skyder 5 bullets i 5 vinkler (powerup)
void bullets_shoot_spread5(Bullet bullets[], int count, float x, float y);

void bullets_update(Bullet bullets[], int count, float dt);

// dræber enemies ved hit.
// returnerer 1 hvis bonus blev samlet (bonus-enemy blev ramt), ellers 0.
int bullets_hit_enemies(Bullet bullets[], int count, enemy enemy_pool[]);

void bullets_push_buffer(uint8_t buf[SCREEN_ROWS][SCREEN_COLS], Bullet bullets[], int count);
void bullets_power_activate(int ticks);
void bullets_powerup_tick(void);
void bullets_handle_shoot(Bullet bullets[], int count, int shoot_just_pressed, float x, float y);

#endif
