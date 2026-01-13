#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "game_settings.h"
#include "player.h"
#include "draw.h"
#include "ansi.h"

int main(void)
{
	uart_init(115200);

	clrscr(); //Ensures putty terminal is clear
	uint8_t foreground_buffer[SCREEN_ROWS][SCREEN_COLS]; //Buffer is used to store visuals. "game board" size is defined for now as 50 by 200.
	for (int r = 0; r < SCREEN_ROWS; r++) {
	    for (int c = 0; c < SCREEN_COLS; c++) {
	        foreground_buffer[r][c] = '#'; // Use a visible character
	    }
	    foreground_buffer[r][SCREEN_COLS - 1] = '\0';
	}

	draw_buffer(foreground_buffer);


	printf("Space invaders should go here");
	while(1){}
}
