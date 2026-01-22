#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "game_settings.h"
#include "enemy.h"
#include "bullet.h"
#include <stdint.h>

/* Asteroid data */
typedef struct {
    uint16_t x, y;    // position
    uint16_t sx, sy;  // size
    uint8_t  alive;   // active on screen
    uint8_t  clean;   // used for collision state
} asteroid;

/* Asteroid functions */
void asteroid_push_buffer(uint8_t buffer[][SCREEN_COLS], asteroid ast);
void asteroid_update_pos(asteroid *ast);
void asteroid_enemies_collision(asteroid *ast, enemy enemy_pool[MAX_ENEMIES]);
void asteroid_gravity(Bullet bullets[], asteroid ast);
int32_t get_approx_dist(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/* Asteroid system state */
typedef struct {
    asteroid ast;
    uint16_t move_counter;
    uint32_t spawn_counter;
    uint32_t next_spawn;
    uint32_t rng;
} AsteroidSystem;

/* Asteroid system control */
void asteroids_init(AsteroidSystem *s);
void asteroids_tick(AsteroidSystem *s, enemy enemy_pool[MAX_ENEMIES]);
void asteroids_draw(const AsteroidSystem *s, uint8_t buffer[][SCREEN_COLS]);

#endif
