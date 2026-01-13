#include "draw.h"
#include "game_settings.h"

void draw_buffer(uint8_t buffer[][SCREEN_COLS]){
	    printf("\033[H"); // Move cursor to 0,0 (Home)

	    for (int r = 0; r < SCREEN_ROWS; r++) {
	        // Ensure the row is null-terminated so printf knows where to stop
	        buffer[r][SCREEN_COLS - 1] = '\0';

	        // 2. Print the entire row at once
	        puts((char*)buffer[r]);
	    }
}
