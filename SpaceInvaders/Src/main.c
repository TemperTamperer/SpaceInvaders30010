#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "game_settings.h"
#include "player.h"
#include "draw.h"
#include "ansi.h"
#include "timer.h"
#include "joystick.h"
#include "asteroids.h"

#include "enemy.h"

int main(void)
{
	uart_init(115200);
	timer15_init();
	GPIO_init();
	printf("\x1B[?25h");

	uint16_t enemy_spawn_counter = 0;
	uint16_t enemy_move_counter = 0;

	clrscr(); //Ensures putty terminal is clear before anything
	draw_border();
	printf("\x1B[?25l");
	uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS]; //Buffer is used to store visuals.
	uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS]; //Buffer used to sore visuals of last frame
	memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS); //Set all elements to " " to clear buffer
	memset(shadow_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);




	player p1 = {.x = 30, .y =SCREEN_ROWS-1, .sx =5, .sy = 3, .hp = 3, .alive = 1}; //The player is defined at a position, with width; 5 and height; 3, with 3 hp, and alive
	enemy enemy_pool[MAX_ENEMIES]; //Array containing all enemies in the game
	asteroid ast = {.x = 2, .y = 20, .sx =9, .sy = 7, .alive = 1, .clean = 1}; //Defines
	memset(enemy_pool, 0, sizeof(enemy_pool));

	uint8_t input = 0;
	while (1){
		 if (timer_flag){
		        timer_flag = 0;
		        enemy_spawn_counter++;
		        enemy_move_counter++;


		        input = read_joystick();

		        clear_buffer(current_buffer);


		        player_update_pos(input, &p1);
		        if(enemy_move_counter >= 15){
		        	enemy_move_counter = 0;
		        	enemies_update_pos(enemy_pool);
		        	asteroid_update_pos(&ast);
		        }
		        asteroid_enemies_collision(&ast, enemy_pool);
		        enemies_player_collision(enemy_pool, &p1);
		        player_condition(&p1);

		        if(enemy_spawn_counter >= 30){
		        	enemy_spawn_counter = 0;
		        	enemies_spawn(enemy_pool);

		        }

		        player_push_buffer(current_buffer, p1);
		        enemies_push_buffer(current_buffer, enemy_pool);
		        asteroid_push_buffer(current_buffer, ast);
				//
		        draw_buffer(current_buffer, shadow_buffer);
		    }
}

}
