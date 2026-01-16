#include "player.h"
#include "joystick.h"

void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player p){
/* Player sprite:
     ^
 	/ \
   /═█═\
*/

	//Array containing all characters used in above player sprite
	uint8_t player_sprite[3][5] = {
	    {' ', ' ', '^', ' ', ' '},
	    {' ', '/', ' ', '\\', ' '},
	    {'/', 205, 219, 205, '\\'}
	};
	uint8_t player_dead_sprite[3][5] = {
		    {' ', ' ', ',', ' ', ' '},
		    {' ', '\\', ' ', '\\', ' '},
		    {'o', '-', ' ', 205, '/'}
		};

	//Goes through character array and push them to buffer
	if(p.alive == 1){
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 5; j++){
				buffer[p.y - i][p.x + j] = player_sprite[p.sy - i - 1][j]; //Magic number -1 is to fix index offset
			}
		}
	}
	else{
		for(int i = 0; i < 3; i++){
					for(int j = 0; j < 5; j++){
						buffer[p.y - i][p.x + j] = player_dead_sprite[p.sy - i - 1][j]; //Magic number -1 is to fix index offset
					}
				}
	}
}

void player_update_pos(uint8_t input, player *p){
	if(p->alive == 1){
    // Move left
		if (input & JOY_LEFT)
		{
			if (p->x > 1)
				p->x -= PLAYER_MOVESPEED;
		}

		// Move right
		if (input & JOY_RIGHT)
		{
			if (p->x < SCREEN_COLS - p->sx - 1)
				p->x += PLAYER_MOVESPEED;
		}
	}
}

void player_condition(player *p){
	if(p->hp < 0) p->alive = 0;
}
