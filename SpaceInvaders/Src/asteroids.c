#include "asteroids.h"
#include "bullet.h"

	/* Asteroid sprite (7x9):
	     .---.
	    / *   \
	   (       )
	   |  ( )  |
	   (      *)
	    \  _  /
	     '---'
	*/
	static const uint8_t asteroid_sprite[7][9] = {
	    {' ', ' ', '.', '-', '-', '-', '.', ' ', ' '},
	    {' ','/', ' ', '*', ' ', ' ', ' ', '\\', ' '},
	    {'(', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ')'},
	    {'|', ' ', ' ', '(', ' ', ')', ' ', ' ', '|'},
	    {'(', ' ', ' ', ' ', ' ', ' ', ' ', '*', ')'},
	    {' ', '\\', ' ', ' ', '_', ' ', ' ', '/', ' '},
	    {' ', ' ', '\'', '-', '-', '-', '\'', ' ', ' '}
	};

void asteroid_init(asteroid *ast){
	ast->x = 2;
	ast->y = 20;
	ast->sx = 19;
	ast->sy = 7;
	ast->alive = 1;
	ast->clean = 1;
}

void asteroid_push_buffer(uint8_t buffer[][SCREEN_COLS], asteroid ast){
	if(ast.alive == 1){
		for(int i = 0; i < 7; i++){
			for(int j = 0; j < 9; j++){
				int ty = ast.y - i;
				int tx = ast.x + j;
				if(ty >= 0 && ty < SCREEN_ROWS && tx >= 0 && tx < SCREEN_COLS) {
					if (asteroid_sprite[6 - i][j] != ' ') {
						buffer[ty][tx] = asteroid_sprite[6 - i][j];
					}
				}
			}
		}
	}
}

void asteroid_update_pos(asteroid *ast){
	ast->x += ASTEROID_SPEED;
}

void asteroid_gravity(Bullet bullets[], asteroid ast) {
    int16_t ast_center_x = ast.x + (ast.sx >> 1);
    int16_t ast_center_y = ast.y + (ast.sy >> 1);

    for (uint8_t i = 0; i < BULLET_POOL_SIZE; i++) {
        Bullet *b = &bullets[i];
        if (!b->active) continue;

        // Get current bullet position
        int16_t bx = b->x >> BULLET_FP_SHIFT;
        int16_t by = b->y >> BULLET_FP_SHIFT;

        int16_t dist = get_approx_dist(bx, by, ast_center_x, ast_center_y);

        //Radius where gravity is applied
        if (dist < 20 && dist > 2) {

            int16_t dx = ast_center_x - bx;
            int16_t dy = ast_center_y - by;

			//Magic numbers determine strength. Strength of y axis is large since ASCII chars are tall
            b->vx += (20 * dx) / dist;
            b->vy += (30 * dy) / dist;
        }
    }
}

int32_t get_approx_dist(int32_t x1, int32_t y1, int32_t x2, int32_t y2){
    // Funciton uses Alpha Max plus Beta Min algorithm for dist approximation
	int32_t dx = x1 - x2;
    int32_t dy = y1 - y2;

    // Absolute values
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    int32_t max_v, min_v;
    if (dx > dy) {
        max_v = dx;
        min_v = dy;
    } else {
        max_v = dy;
        min_v = dx;
    }

    //returns Distance = 1.0 * max + 0.375 * min (1.0 = alpha, 0.375 = Beta)
    return max_v + (min_v >> 2) + (min_v >> 3);
}

void asteroid_enemies_collision(asteroid *ast, enemy enemy_pool[MAX_ENEMIES]){
	for(int e = 0; e < MAX_ENEMIES; e++){
		if(enemy_pool[e].alive == 1){
			//If stetements check if any part of the enemy is within the bounds of the asteroid
			if((enemy_pool[e].x) > ast->x && (enemy_pool[e].x) < ast->x + ast->sx){
				if((enemy_pool[e].y) < ast->y && (enemy_pool[e].y) > ast->y - ast->sy){
					enemy_pool[e].alive =0;
				}
			}
		}
	}
}

void asteroid_spawn();
