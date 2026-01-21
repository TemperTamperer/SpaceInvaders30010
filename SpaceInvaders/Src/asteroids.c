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

void asteroid_gravity(Bullet bullets[], asteroid ast){
	for(uint8_t i = 0; i < BULLET_POOL_SIZE; i++){
		Bullet *b = &bullets[i];
		if (!b->active) continue;

		int16_t bx = b->x >> BULLET_FP_SHIFT;
		int16_t by = b->y >> BULLET_FP_SHIFT;

		int16_t dist = get_approx_dist(by, by, ast.x, ast.y);
		b->vx += 10 * ((ast.x + (ast.sx >> 1)) - bx) / (dist);
		b->vy += 10 * ((ast.y + (ast.sy >> 1)) - by) / (dist);
	}
}

int32_t get_approx_dist(int32_t x1, int32_t y1, int32_t x2, int32_t y2){
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

    // Makes use of Alpha Max plus Beta Min for dist approximation: Distance = 1.0 * max + 0.375 * min
    return max_v + (min_v >> 2) + (min_v >> 3);
}

void asteroid_enemies_collision(asteroid *ast, enemy enemy_pool[MAX_ENEMIES]){
	for(int e = 0; e < MAX_ENEMIES; e++){
		if(enemy_pool[e].alive == 1){
			if((enemy_pool[e].x) > ast->x && (enemy_pool[e].x) < ast->x + ast->sx){
				if((enemy_pool[e].y) < ast->y && (enemy_pool[e].y) > ast->y - ast->sy){
					enemy_pool[e].alive =0;
				}
			}
		}
	}
}

void asteroid_spawn();
