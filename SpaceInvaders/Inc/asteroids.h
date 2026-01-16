#ifndef ASTEROIDS_H_
#define ASTEROIDS_H_

#include "game_settings.h"
#include "stdint.h"

typedef struct{
	uint16_t x, y; //position
	uint8_t alive;
	uint8_t clean;
} asteroid;


typedef struct{
	uint16_t x, y; //position
	uint8_t sx, sy; //width (sx), height(sy) of player hitbox
	uint8_t hp; //health points for the player. When zero game end
	uint8_t alive; //flag telling if enemy is dead or alie
} enemy;

#endif /* ASTEROIDS_H_ */
