#ifndef ENEMY_H_
#define ENEMY_H_
#include "stdint.h"
#include "game_settings.h"
#include "player.h"

typedef struct{
	uint16_t x, y; //position
	uint8_t sx, sy; //width (sx), height(sy) of player hitbox
	uint8_t hp; //health points for the player. When zero game end
	uint8_t alive; //flag telling if enemy is dead or alie
} enemy;

void enemies_push_buffer(uint8_t buffer[][SCREEN_COLS], enemy enemy_pool[MAX_ENEMIES]);
void enemies_update_pos(enemy enemy_pool[MAX_ENEMIES]);
uint8_t enemies_player_collision(enemy enemy_pool[MAX_ENEMIES], player *p);
void enemies_spawn(enemy enemy_pool[MAX_ENEMIES]);


#endif /* ENEMY_H_ */
