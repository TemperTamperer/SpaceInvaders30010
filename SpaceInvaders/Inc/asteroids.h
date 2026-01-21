#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include <stdint.h>
#include "game_settings.h"
#include "enemy.h"

typedef struct {
    int16_t x;
    int16_t y;
    int16_t sx;
    int16_t sy;
    uint8_t alive;
} asteroid;

typedef struct {
    asteroid ast;
    uint16_t move_counter;
    uint32_t spawn_counter;
    uint32_t next_spawn;
    uint32_t rng;
} AsteroidSystem;

void asteroids_init(AsteroidSystem *s);
void asteroids_tick(AsteroidSystem *s, enemy enemy_pool[MAX_ENEMIES]);
void asteroids_draw(const AsteroidSystem *s, uint8_t buffer[][SCREEN_COLS]);

#endif
