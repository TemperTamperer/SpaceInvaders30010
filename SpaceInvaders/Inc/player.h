#ifndef PLAYER_H_
#define PLAYER_H_

#include "stdint.h"
#include "game_settings.h"

typedef struct{
	uint16_t x, y; //position
	uint8_t sx, sy; //width (sx), height(sy) of player hitbox
} player;

void player_update_pos(uint8_t input, player *p1);
void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player *p1);


#endif /* PLAYER_H_ */
