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

	buffer[p1->y][p1->x + 0] = '/';
	buffer[p1->y][p1->x + 1] = '=';
	buffer[p1->y][p1->x + 2] = '█';
	buffer[p1->y][p1->x + 3] = '=';
	buffer[p1->y][p1->x + 4] = '\\';
}
