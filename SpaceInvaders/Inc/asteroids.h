#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

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

void asteroid_push_buffer(uint8_t buffer[][SCREEN_COLS], asteroid ast);
void asteroid_update_pos(asteroid *ast);
void asteroid_enemies_collision(asteroid *ast, enemy enemy_pool[MAX_ENEMIES]);
void asteroid_gravity(Bullet bullets[], asteroid ast);
int32_t get_approx_dist(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

#endif /* ASTEROIDS_H_ */
