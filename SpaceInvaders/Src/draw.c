#include "draw.h"
#include "ansi.h"
#include <stdio.h>
#include <string.h>
#include "game_settings.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "level.h"
#include "asteroids.h"

#define UI_OFFSET 2
#define HIT_TARGET 5

#define LEVEL_BOX_W 20
#define LEVEL_BOX_H 3
#define LEVEL_BOX_ROW (UI_OFFSET + (SCREEN_ROWS / 2))
#define LEVEL_BOX_COL (UI_OFFSET + ((SCREEN_COLS - LEVEL_BOX_W) / 2))

// Init screen
void draw_game_init_screen(void)
{
    clrscr();
    draw_border();
    printf("\x1B[?25l");
}

// Check for game over
void draw_check_game_over(uint8_t hp, uint32_t score, uint32_t highscore)
{
    if (hp == 0)
    {
        draw_game_over(score, highscore);
        while (1) { }
    }
}

// Draw one frame
void draw_frame(uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS],
                uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS],
                const player *p,
                enemy enemy_pool[MAX_ENEMIES],
                Bullet enemyBullets[], int enemyBulletCount,
                Bullet playerBullets[], int playerBulletCount,
                const AsteroidSystem *asteroids,
                const LevelState *level,
                uint32_t score, uint32_t highscore)
{
    clear_buffer(current_buffer);

    player_push_buffer(current_buffer, *p);
    enemies_push_buffer(current_buffer, enemy_pool);
    bullets_push_buffer(current_buffer, enemyBullets, enemyBulletCount);
    bullets_push_buffer(current_buffer, playerBullets, playerBulletCount);
    asteroids_draw((AsteroidSystem *)asteroids, current_buffer);

    if (level_popup_active((LevelState *)level))
        draw_level_box(level_get((LevelState *)level));
    else if (level_popup_just_ended((LevelState *)level))
        draw_level_box_clear();

    draw_buffer(current_buffer, shadow_buffer);
    ui_draw_status(p->hp, p->hit_count, score, highscore);
}

// Level popup
void draw_level_box(uint8_t level)
{
    ...
}

// Clear level popup
void draw_level_box_clear(void)
{
    ...
}

// Draw UI status
void ui_draw_status(uint8_t hp, uint8_t hits, uint32_t score, uint32_t highscore)
{
    ...
}

// Game over screen
void draw_game_over(uint32_t score, uint32_t highscore)
{
    ...
}

// Draw changed characters
void draw_buffer(uint8_t current[][SCREEN_COLS], uint8_t shado]()
