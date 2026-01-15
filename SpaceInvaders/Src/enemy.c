#include "enemy.h"

void enemies_push_buffer(uint8_t buffer[][SCREEN_COLS], enemy enemy_pool[MAX_ENEMIES]){

	/*
		  --< (XXX) >--
		      ) X (
		  --< (XVX) >--
		*/
		uint8_t alien_lasher[3][13] = {
		    {'-','-','<',' ','(','X','X','X',')',' ','>','-','-'},
		    {' ',' ',' ',' ',')',' ','X',' ','(',' ',' ',' ',' '},
			{'-','-','<',' ','(','X','V','X',')',' ','>','-','-'},
		};

		for(uint8_t e = 0; e < MAX_ENEMIES; e++){
			if(enemy_pool[e].alive == 1){

				for(int i = 0; i < 3; i++){
					for(int j = 0; j < 13; j++){
						buffer[enemy_pool[e].y - i][enemy_pool[e].x + j] = alien_lasher[2 - i][j];
					}
				}

			}
		}


}

void enemies_update_pos(enemy enemy_pool[MAX_ENEMIES]){
	for(uint8_t e = 0; e < MAX_ENEMIES; e++){
		if(enemy_pool[e].alive == 1){
			if(enemy_pool[e].y < PLAYER_COLLISION_LINE - 1){
				enemy_pool[e].y += 1;
			}
			else{
				enemy_pool[e].alive = 0;
			}
		}

	}
}

void enemies_spawn(enemy enemy_pool[MAX_ENEMIES]){
	for(uint8_t e = 0; e < SPAWN_AMOUNT; e++){
		for(uint8_t i = 0; i < MAX_ENEMIES; i++){
			if(enemy_pool[i].alive == 0){
				enemy_pool[i].x = e * (SCREEN_COLS / SPAWN_AMOUNT);
				enemy_pool[i].y = 4;
				enemy_pool[i].alive = 1;
				break;
			}
		}
	}
}
