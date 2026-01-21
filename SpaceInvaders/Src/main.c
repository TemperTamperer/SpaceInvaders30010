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
#include "asteroids.h"
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

    player p1;
    player_init(&p1);

    enemy enemy_pool[MAX_ENEMIES];
    enemies_init(enemy_pool);

    Bullet playerBullets[BULLET_POOL_SIZE];
    Bullet enemyBullets[ENEMY_BULLET_POOL_SIZE];
    bullets_init(playerBullets, BULLET_POOL_SIZE);
    bullets_init(enemyBullets, ENEMY_BULLET_POOL_SIZE);

    EnemyShootState shootState = {0};

    AsteroidSystem asteroids;
    asteroids_init(&asteroids);

    uint8_t prev_center_pressed = 0;
    uint8_t last_level = level_get(&level);

    while (1)
    {
        timer_wait_for_tick();

        uint8_t move_input;
        uint8_t center_just_pressed;

        joystick_update(&move_input,
                        &center_just_pressed,
                        &prev_center_pressed);

        player_update_pos(move_input, &p1);

        int startX, startY;
        player_get_shoot_pos(&p1, &startX, &startY);

        enemies_tick(enemy_pool,
                     &enemy_move_counter,
                     &enemy_spawn_counter,
                     level_spawn_limit(&level));

        powerup_shoot(&powerup,
                      playerBullets,
                      BULLET_POOL_SIZE,
                      center_just_pressed,
                      startX,
                      startY);

        bullets_update(playerBullets, BULLET_POOL_SIZE);
        bullets_update(enemyBullets, ENEMY_BULLET_POOL_SIZE);

        enemies_shoot(enemy_pool,
                      enemyBullets,
                      ENEMY_BULLET_POOL_SIZE,
                      &shootState,
                      level_get(&level));

        player_hit_by_enemy_bullets(enemyBullets,
                                    ENEMY_BULLET_POOL_SIZE,
                                    &p1);

        draw_check_game_over(p1.hp, score, highscore);

        int kills = bullets_hit_enemies(playerBullets, BULLET_POOL_SIZE, enemy_pool);
        bullets_apply_kills_to_score(kills, &score, &highscore);


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

        asteroids_tick(&asteroids, enemy_pool);

        draw_frame(current_buffer, shadow_buffer, &p1, enemy_pool,
                   enemyBullets, ENEMY_BULLET_POOL_SIZE,
                   playerBullets, BULLET_POOL_SIZE,
                   &asteroids, &level, score, highscore);



    }
}
