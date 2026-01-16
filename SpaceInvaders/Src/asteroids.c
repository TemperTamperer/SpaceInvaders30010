#include "asteroids.h"

void asteroid_push_buffer(uint8_t buffer[][SCREEN_COLS], asteroid ast){
	/* Asteroid sprite (7x9):
	     .---.
	    / *   \
	   (       )
	   |  ( )  |
	   (      *)
	    \  _  /
	     '---'
	*/
	uint8_t asteroid_sprite[7][9] = {
	    {' ', ' ', '.', '-', '-', '-', '.', ' ', ' '},
	    {' ','/', ' ', '*', ' ', ' ', ' ', '\\', ' '},
	    {'(', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ')'},
	    {'|', ' ', ' ', '(', ' ', ')', ' ', ' ', '|'},
	    {'(', ' ', ' ', ' ', ' ', ' ', ' ', '*', ')'},
	    {' ', '\\', ' ', ' ', '_', ' ', ' ', '/', ' '},
	    {' ', ' ', '\'', '-', '-', '-', '\'', ' ', ' '}
	};

	if(ast.alive == 1){
		for(int i = 0; i < 7; i++){
			for(int j = 0; j < 9; j++){
				int ty = ast.y - i;
				int tx = ast.x + j;
				if(ty >= 0 && ty < SCREEN_ROWS && tx >= 0 && tx < SCREEN_COLS) {
					if (asteroid_sprite[6 - i][j] != ' ') {
						buffer[ty][tx] = asteroid_sprite[6 - i][j];
					}
				}
			}
		}
	}
}

void asteroid_update_pos(asteroid *ast){
	ast->x += ASTEROID_SPEED;
}

void asteroid_enemies_collision(asteroid *ast, enemy enemy_pool[MAX_ENEMIES]){
	for(int e = 0; e < MAX_ENEMIES; e++){
		if(enemy_pool[e].alive == 1){
			if((enemy_pool[e].x) > ast->x && (enemy_pool[e].x) < ast->x + ast->sx){
				if((enemy_pool[e].y) < ast->y && (enemy_pool[e].y) > ast->y - ast->sy){
					enemy_pool[e].alive =0;
				}
			}
		}
	}
}

void asteroid_spawn();
