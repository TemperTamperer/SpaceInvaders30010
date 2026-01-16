#ifndef PLAYER_H_
#define PLAYER_H_

#include "stdint.h"
#include "game_settings.h"

typedef struct{
	uint16_t x, y; //position
	uint8_t sx, sy; //width (sx), height(sy) of player hitbox
	int8_t hp; //health points for the player. When zero game end
	uint8_t alive;
} player;

void player_update_pos(uint8_t input, player *p1);
void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player p1);
void player_condition(player *p);


#endif /* PLAYER_H_ */
