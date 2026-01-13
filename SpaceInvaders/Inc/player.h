#ifndef PLAYER_H_
#define PLAYER_H_

#include "stdint.h"
#include "game_settings.h"

typedef struct{
	uint16_t x, y; //position
	uint8_t sx, sy; //width (sx), height(sy) of player hitbox
} player;

void push_buffer_player(uint8_t *buffer[][SCREEN_COLS]);

#endif /* PLAYER_H_ */
