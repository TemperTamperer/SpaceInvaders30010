#include "enemy.h"

void enemy_push_buffer(uint8_t buffer[][SCREEN_COLS], enemy ene){

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

		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 13; j++){
				buffer[ene.y - i][ene.x + j] = alien_lasher[2 - i][j];
			}
		}
}

void enemy_update_pos(){

}
