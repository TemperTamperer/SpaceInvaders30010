#include "player.h"
#include "joystick.h"
<<<<<<< Updated upstream
void player_update_pos(uint8_t input, player *p1)
{
    // Move left
    if (input & JOY_LEFT)
    {
        if (p1->x > 1)
            p1->x--;
    }
=======
#include "bullet.h"
>>>>>>> Stashed changes

    // Move right
    if (input & JOY_RIGHT)
    {
        if (p1->x < SCREEN_COLS - p1->sx - 1)
            p1->x++;
    }
}



void player_push_buffer(uint8_t buffer[][SCREEN_COLS], player *p1){
/* Player sprite:
     ==
 	/ \
   /═█═\
*/

	buffer[p1->y - 2][p1->x + 2] = '=';
	buffer[p1->y - 2][p1->x + 3] = '=';

	buffer[p1->y - 1][p1->x + 1] = '/';
	buffer[p1->y - 1][p1->x + 4] = '\\';

<<<<<<< Updated upstream
	buffer[p1->y][p1->x + 0] = '/';
	buffer[p1->y][p1->x + 1] = '=';
	buffer[p1->y][p1->x + 2] = '█';
	buffer[p1->y][p1->x + 3] = '=';
	buffer[p1->y][p1->x + 4] = '\\';
=======
	//Goes through character array and push them to buffer
	if(p.alive == 1){
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 5; j++){
				int ty = p.y - i;
				int tx = p.x + j;
				if(ty >= 0 && ty < SCREEN_ROWS && tx >= 0 && tx < SCREEN_COLS) {
					if (player_sprite[2 - i][j] != ' ') {
						buffer[ty][tx] = player_sprite[2 - i][j];
					}
				}
			}
		}
	}
	else{
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 5; j++){
				int ty = p.y - i;
				int tx = p.x + j;
				if(ty >= 0 && ty < SCREEN_ROWS && tx >= 0 && tx < SCREEN_COLS) {
					if (player_sprite[2 - i][j] != ' ') {
						buffer[ty][tx] = player_dead_sprite[2 - i][j];
					}
				}
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

		// Shoot


	}
}

void player_condition(player *p){
	if(p->hp < 0) p->alive = 0;
>>>>>>> Stashed changes
}
