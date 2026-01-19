#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
#include "game_settings.h"
#include "player.h"
#include "draw.h"
#include "ansi.h"
#include "timer.h"
#include "joystick.h"
<<<<<<< Updated upstream
=======
#include "asteroids.h"
#include "bullet.h"
#include "collision.h"
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes


int main(void)
{
	bullet bullets[MAX_BULLETS];

	uart_init(115200);
	timer15_init();
	GPIO_init();
	bullets_init(bullets);
	printf("\x1B[?25h");






	clrscr(); //Ensures putty terminal is clear before anything
	draw_border();
	printf("\x1B[?25l");
	uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS]; //Buffer is used to store visuals.
	uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS]; //Buffer used to sore visuals of last frame
	memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS); //Set all elements to " " to clear buffer
	memset(shadow_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);




	player p1 = {.x = 50, .y =SCREEN_ROWS-1, .sx =5, .sy = 3}; //defines the player at (50, 59) which is the bottom middle of the screen, with a size of 5x3
	uint8_t input = 0;
	while (1){
		 if (timer_flag)
		    {
		        timer_flag = 0;

		        input = read_joystick();

		        clear_buffer(current_buffer);
		        player_update_pos(input, &p1);
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		        player_push_buffer(current_buffer, &p1);
=======
=======
>>>>>>> Stashed changes
		        bullets_update(bullets);
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

		    	if (input & JOY_UP)
		    		{
		    		    bullets_fire(bullets, p1.x + p1.sx / 2, p1.y - 1);
		    		}

		    	bullets_vs_enemies(bullets, enemies, enemy_pool);

		        player_push_buffer(current_buffer, p1);
		        enemies_push_buffer(current_buffer, enemy_pool);
		        asteroid_push_buffer(current_buffer, ast);
		        bullets_draw(bullets, current_buffer);
				//
>>>>>>> Stashed changes
		        draw_buffer(current_buffer, shadow_buffer);
		    }
}

}
