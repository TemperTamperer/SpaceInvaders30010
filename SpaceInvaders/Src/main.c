#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "game_settings.h"
#include "player.h"
#include "draw.h"
#include "ansi.h"
#include "enemy.h"

int main(void)
{
	uart_init(115200);

	clrscr(); //Ensures putty terminal is clear before anything
<<<<<<< Updated upstream
	uint8_t foreground_buffer[SCREEN_ROWS][SCREEN_COLS]; //Buffer is used to store visuals.
	for (int r = 0; r < SCREEN_ROWS; r++) {
	    for (int c = 0; c < SCREEN_COLS; c++) {
	        foreground_buffer[r][c] = ' '; // Use a visible character
	    }
	    foreground_buffer[r][SCREEN_COLS - 1] = '\0';
	}
	player p1 = {.x = 50, .y =59, .sx =5, .sy = 3}; //defines the player at (50, 59) which is the bottom middle of the screen, with a size of 5x3
=======
	draw_border();
	uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS]; //Buffer is used to store visuals.
	uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS]; //Buffer used to sore visuals of last frame
	memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS); //Set all elements to " " to clear buffer
	memset(shadow_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);




	player p1 = {.x = 50, .y = SCREEN_ROWS-1, .sx =5, .sy = 3}; //defines the player at (50, 59) which is the bottom middle of the screen, with a size of 5x3
	enemy ene = {.x = 50, .y = 4,};
>>>>>>> Stashed changes
	uint8_t tempvar = 1;
	while (1){
	player_update_pos(tempvar, &p1);
<<<<<<< Updated upstream
	player_push_buffer(foreground_buffer, &p1);
	draw_buffer(foreground_buffer);
=======
	player_push_buffer(current_buffer, p1);
	enemy_push_buffer(current_buffer, ene);
	draw_buffer(current_buffer, shadow_buffer);
>>>>>>> Stashed changes
}
	while(1){}
}
