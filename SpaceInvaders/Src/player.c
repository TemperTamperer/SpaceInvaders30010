#include "player.h"
#include "joystick.h"
void player_update_pos(uint8_t input, player *p1)
{
    // Move left
    if (input & JOY_LEFT)
    {
        if (p1->x > 1)
            p1->x--;
    }

    // Move right
    if (input & JOY_RIGHT)
    {
        if (p1->x < SCREEN_COLS - p1->sx - 1)
            p1->x++;
    }
}

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

	//Goes through character array and push them to buffer
	for(int i = 0; i < 3; i++){
				for(int j = 0; j < 5; j++){
					buffer[p.y - i][p.x + j] = player_sprite[p.sy - i - 1][j]; //Magic number -1 is to fix index offset
				}
			}
	/*
	//Following is the charecters pushed to the buffer at their respective x,y coordinates
	buffer[p1.y - 2][p1.x + 2] = '^';

	buffer[p1.y - 1][p1.x + 1] = '/';
	buffer[p1.y - 1][p1.x + 4] = '\\';

	buffer[p1.y][p1.x + 0] = '/';
	buffer[p1.y][p1.x + 1] = '=';
	buffer[p1.y][p1.x + 2] = '█';
	buffer[p1.y][p1.x + 3] = '=';
	buffer[p1.y][p1.x + 4] = '\\';
	*/
}
