#ifndef ENEMY_H_
#define ENEMY_H_
#include "stdint.h"
#include "game_settings.h"

typedef struct{
	uint16_t x, y; //position
	uint8_t sx, sy; //width (sx), height(sy) of player hitbox
	uint8_t hp; //health points for the player. When zero game end
} enemy;

void enemy_push_buffer(uint8_t buffer[][SCREEN_COLS], enemy ene);

#endif /* ENEMY_H_ */
