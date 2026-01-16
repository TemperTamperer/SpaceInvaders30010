#include "enemy.h"

	/*
	(XXX)
	) X (
	(XVX)
	 */
	static const uint8_t alien_lasher[3][5] = {
		{'(','X','X','X',')'},
		{')',' ','X',' ','('},
		{'(','X','V','X',')'},
	};


void enemies_push_buffer(uint8_t buffer[][SCREEN_COLS], enemy enemy_pool[MAX_ENEMIES]){
	for(uint8_t e = 0; e < MAX_ENEMIES; e++){
		if(enemy_pool[e].alive == 1){
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 5; j++){
					int ty = enemy_pool[e].y - i;
					int tx = enemy_pool[e].x + j;
					if(ty >= 0 && ty < SCREEN_ROWS && tx >= 0 && tx < SCREEN_COLS) {
						if (alien_lasher[2 - i][j] != ' ') {
							buffer[ty][tx] = alien_lasher[2 - i][j];
						}
					}
				}
			}
		}
	}
}

void enemies_update_pos(enemy enemy_pool[MAX_ENEMIES]){
	for(uint8_t e = 0; e < MAX_ENEMIES; e++){
		if(enemy_pool[e].alive == 1){
				enemy_pool[e].y += 3;
		}
	}
}

uint8_t enemies_player_collision(enemy enemy_pool[MAX_ENEMIES], player *p){
	for(uint8_t e = 0; e < MAX_ENEMIES; e++){
			if(enemy_pool[e].alive == 1){
				if(enemy_pool[e].y > PLAYER_COLLISION_LINE){
					enemy_pool[e].alive = 0;
					p->hp--;
				}
			}
		}
	}

void enemies_spawn(enemy enemy_pool[MAX_ENEMIES]){
	for(uint8_t e = 0; e < SPAWN_AMOUNT; e++){
		for(uint8_t i = 0; i < MAX_ENEMIES; i++){
			if(enemy_pool[i].alive == 0){
				enemy_pool[i].x = e * (SPAWN_DISTANCE) + 3; //Division bad, magic number 3 is left offset
				enemy_pool[i].y = 4;
				enemy_pool[i].alive = 1;
				break;
			}
		}
	}
}
