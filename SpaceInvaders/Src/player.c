#include "player.h"
void player_update_pos(uint8_t input, player *p1){
	if(p1->x < 1) return;
	p1->x -= 1;
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
