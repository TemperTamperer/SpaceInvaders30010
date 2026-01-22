#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "game_settings.h"
#include "stdint.h"
#include "enemy.h"
#include "bullet.h"

typedef struct{
	uint16_t x, y; //position
	uint16_t sx, sy; //dimensions
	uint8_t alive; //on screen
	uint8_t clean; //Is clean until enemy collision
} asteroid;

void asteroid_init(asteroid *ast);
void asteroid_push_buffer(uint8_t buffer[][SCREEN_COLS], asteroid ast);
void asteroid_update_pos(asteroid *ast);
void asteroid_enemies_collision(asteroid *ast, enemy enemy_pool[MAX_ENEMIES]);
void asteroid_gravity(Bullet bullets[], asteroid ast);
int32_t get_approx_dist(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

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
