#ifndef DRAW_H_
#define DRAW_H_

#include <stdint.h>
#include "game_settings.h"

#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "level.h"
#include "asteroids.h"

/* Draw full game frame */
void draw_frame(uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS],
                uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS],
                const player *p,
                enemy enemy_pool[MAX_ENEMIES],
                Bullet enemyBullets[], int enemyBulletCount,
                Bullet playerBullets[], int playerBulletCount,
                const AsteroidSystem *asteroids,
                const LevelState *level,
                uint32_t score, uint32_t highscore);

/* Screen control */
void draw_game_init_screen(void);
void draw_check_game_over(uint8_t hp, uint32_t score, uint32_t highscore);
void draw_game_over(uint32_t score, uint32_t highscore);

/* Buffer handling */
void draw_buffer(uint8_t buffer[][SCREEN_COLS],
                 uint8_t shadow_buffer[][SCREEN_COLS]);
void clear_buffer(uint8_t current_buffer[][SCREEN_COLS]);
void draw_border(void);

/* Level popup */
void draw_level_box(uint8_t level);
void draw_level_box_clear(void);

/* UI */
void ui_draw_status(uint8_t hp, uint8_t hits,
                    uint32_t score, uint32_t highscore);

#endif
