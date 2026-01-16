#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

#include "game_settings.h"
#include "stdint.h"
#include "enemy.h"

typedef struct{
	uint16_t x, y; //position
	uint16_t sx, sy; //dimensions
	uint8_t alive; //on screen
	uint8_t clean; //Is clean until enemy collision
} asteroid;

void asteroid_push_buffer(uint8_t buffer[][SCREEN_COLS], asteroid ast);
void asteroid_update_pos(asteroid *ast);
void asteroid_enemies_collision(asteroid *ast, enemy enemy_pool[MAX_ENEMIES]);


#endif /* ASTEROIDS_H_ */
