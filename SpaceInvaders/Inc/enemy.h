#ifndef ENEMY_H_
#define ENEMY_H_

#include <stdint.h>
#include "game_settings.h"

typedef struct{
	uint16_t x, y; //position
	uint8_t sx, sy; //width (sx), height(sy) of player hitbox
	uint8_t hp; //health points for the player. When zero game end
	uint8_t alive; //flag telling if enemy is dead or alie
    uint8_t smoke_ticks;
} enemy;

void enemies_spawn(enemy enemy_pool[MAX_ENEMIES]);
void enemies_update_pos(enemy enemy_pool[MAX_ENEMIES]);
void enemies_push_buffer(uint8_t buffer[][SCREEN_COLS], enemy enemy_pool[MAX_ENEMIES]);


#endif /* ENEMY_H_ */
