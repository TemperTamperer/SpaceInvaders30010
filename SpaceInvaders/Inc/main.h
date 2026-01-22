#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "game_settings.h"
#include "player.h"
#include "draw.h"
#include "ansi.h"
#include "timer.h"
#include "joystick.h"
#include "game_state.h"
#include "asteroids.h"
#include "lcd.h"
#include "buzzer.h"
#include "buzzer_songs.h"
#include "enemy.h"
#include "bullet.h"
#include "level.h"
#include "powerup.h"

#include <stdint.h>
#include <string.h>

#define TICKS_PER_SECOND 20
#define TICK_MS (1000 / TICKS_PER_SECOND)

#define POWERUP_SECONDS  5
#define POWERUP_TICKS    (POWERUP_SECONDS * TICKS_PER_SECOND)
#define ENEMY_BULLET_POOL_SIZE 16

void game_reset(void);
void game_update(uint8_t input);
void game_draw(void);
void handle_state_entry(void);
void handle_state_logic(uint8_t input);

uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS];
uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS];
uint8_t lcd_buffer[512];

