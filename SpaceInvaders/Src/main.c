#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "game_settings.h"
#include "player.h"
#include "draw.h"
#include "ansi.h"
#include "timer.h"
#include "joystick.h"
#include "enemy.h"
#include "bullet.h"
#include "level.h"
#include "powerup.h"
#include <stdint.h>
#include <string.h>

#define ENEMY_BULLET_POOL_SIZE 16

int main(void)
{
    uart_init(115200);
    timer15_init();
    GPIO_init();
    printf("\x1B[?25h");

    uint32_t score = 0;
    uint32_t highscore = 0;

    uint16_t enemy_spawn_counter = 0;
    uint16_t enemy_move_counter  = 0;

    draw_game_init_screen();

    LevelState level;
    level_init(&level);

    PowerupState powerup;
    powerup_init(&powerup);

    uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS];
    uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS];
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
    memset(shadow_buffer,  ' ', SCREEN_ROWS * SCREEN_COLS);

    player p1 = {.x = 50, .y = SCREEN_ROWS - 1, .sx = 5, .sy = 3};
    p1.hp = 3;
    p1.hit_count = 0;

    enemy enemy_pool[MAX_ENEMIES];
    enemies_init(enemy_pool);

    Bullet playerBullets[BULLET_POOL_SIZE];
    Bullet enemyBullets[ENEMY_BULLET_POOL_SIZE];
    bullets_init(playerBullets, BULLET_POOL_SIZE);
    bullets_init(enemyBullets, ENEMY_BULLET_POOL_SIZE);

    EnemyShootState shootState = {0};

    uint8_t prev_center_pressed = 0;
    uint8_t last_level = level_get(&level);

    while (1)
    {
        if (!timer_flag) continue;
        timer_flag = 0;

        enemy_spawn_counter++;
        enemy_move_counter++;

        uint8_t input = read_joystick();

        uint8_t move_input = input & (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT);
        player_update_pos(move_input, &p1);

        uint8_t center_just_pressed =
            joystick_just_pressed(input, JOY_CENTER, &prev_center_pressed);

        int startX = (p1.x + (p1.sx / 2));
        int startY = (p1.y - 1);

        powerup_shoot(&powerup, playerBullets, BULLET_POOL_SIZE, center_just_pressed, startX, startY);

        bullets_update(playerBullets, BULLET_POOL_SIZE);
        bullets_update(enemyBullets, ENEMY_BULLET_POOL_SIZE);

        if (enemy_move_counter > 15)
        {
            enemy_move_counter = 0;
            enemies_update_pos(enemy_pool);
        }

        if (enemy_spawn_counter > level_spawn_limit(&level))
        {
            enemy_spawn_counter = 0;
            enemies_spawn(enemy_pool);
        }

        enemies_shoot(enemy_pool, enemyBullets, ENEMY_BULLET_POOL_SIZE, &shootState, level_get(&level));

        player_hit_by_enemy_bullets(enemyBullets, ENEMY_BULLET_POOL_SIZE, &p1);

        if (p1.hp == 0)
        {
            draw_game_over(score, highscore);
            while (1) { }
        }

        int kills = bullets_hit_enemies(playerBullets, BULLET_POOL_SIZE, enemy_pool);

        if (kills > 0)
        {
            score += (uint32_t)(kills * 10u);
            if (score > highscore) highscore = score;
        }

        level_update_from_score(&level, score);

        uint8_t now_level = level_get(&level);
        if (now_level != last_level)
        {
            last_level = now_level;

            enemies_reset(enemy_pool, &shootState);
            bullets_init(enemyBullets, ENEMY_BULLET_POOL_SIZE);

            enemy_spawn_counter = 0;
            enemy_move_counter = 0;
        }

        powerup_update_from_score(&powerup, score);
        powerup_tick(&powerup);

        if (level_popup_active(&level))
            level_popup_tick(&level);

        clear_buffer(current_buffer);

        player_push_buffer(current_buffer, p1);
        enemies_push_buffer(current_buffer, enemy_pool);
        bullets_push_buffer(current_buffer, enemyBullets, ENEMY_BULLET_POOL_SIZE);
        bullets_push_buffer(current_buffer, playerBullets, BULLET_POOL_SIZE);

        if (level_popup_active(&level))
            draw_level_box(level_get(&level));
        else if (level_popup_just_ended(&level))
            draw_level_box_clear();

        draw_buffer(current_buffer, shadow_buffer);
        ui_draw_status(p1.hp, p1.hit_count, score, highscore);
    }
}
