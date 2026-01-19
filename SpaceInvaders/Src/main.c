#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "game_settings.h"
#include "player.h"
#include "draw.h"
#include "ansi.h"
#include "timer.h"
#include "joystick.h"

#include "asteroids.h"

#include "enemy.h"
#include "bullet.h"
#include <stdint.h>
#include <string.h>

#define TICKS_PER_SECOND 20
#define POWERUP_SECONDS  5
#define POWERUP_TICKS    (POWERUP_SECONDS * TICKS_PER_SECOND)
#define ENEMY_BULLET_POOL_SIZE 16
int main(void)
{
    uart_init(115200);
    timer15_init();
    GPIO_init();
    printf("\x1B[?25h");

    uint16_t enemy_spawn_counter = 0;
    uint16_t enemy_move_counter  = 0;
    uint32_t score = 0;
    uint32_t highscore = 0;

    clrscr();
    draw_border();
    printf("\x1B[?25l");

    uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS];
    uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS];
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
    memset(shadow_buffer,  ' ', SCREEN_ROWS * SCREEN_COLS);

    player p1 = {.x = 50, .y = SCREEN_ROWS - 1, .sx = 5, .sy = 3};
           p1.hp = 3;
           p1.hit_count = 0;
    enemy enemy_pool[MAX_ENEMIES];
    memset(enemy_pool, 0, sizeof(enemy_pool));
    asteroid ast = {.x = 2, .y = 20, .sx =9, .sy = 7, .alive = 1, .clean = 1}


    /* Player bullets (din eksisterende pool) */
    Bullet* bullets = bullets_get_pool();
    int bullet_count = bullets_get_count();
    (void)bullet_count; // hvis du ikke bruger den direkte

    bullets_init(bullets, BULLET_POOL_SIZE);

    /* Enemy bullets (egen pool) */
    Bullet enemyBullets[ENEMY_BULLET_POOL_SIZE];
    bullets_init(enemyBullets, ENEMY_BULLET_POOL_SIZE);
    uint8_t prev_center_pressed = 0;
    uint8_t input = 0;

    while (1)
    {
        if (timer_flag)
        {
            timer_flag = 0;
            enemy_spawn_counter++;
            enemy_move_counter++;

            bonus_spawn_tick(enemy_pool);

            input = read_joystick();

            clear_buffer(current_buffer);

            /* Bevægelse: kun retninger (center påvirker ikke movement) */
            uint8_t move_input = input & (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT);
            player_update_pos(move_input, &p1);

            /* Skyd (player) */
            uint8_t center_just_pressed = joystick_just_pressed(input, JOY_CENTER, &prev_center_pressed);
            int startX = (p1.x + (p1.sx / 2));
            int startY = (p1.y - 1);

            bullets_handle_shoot(bullets, BULLET_POOL_SIZE, center_just_pressed, startX, startY);
            bullets_powerup_tick();

            bullets_update(bullets, BULLET_POOL_SIZE);
            bullets_update(enemyBullets, ENEMY_BULLET_POOL_SIZE);

            /* Enemy move/spawn */
            if (enemy_move_counter > 15)
            {
                enemy_move_counter = 0;
                enemies_update_pos(enemy_pool);
            }

            if (enemy_spawn_counter > 80)
            {
                enemy_spawn_counter = 0;
                enemies_spawn(enemy_pool);
            }

            /* NYT: Fjender skyder (logikken ligger i enemy.c) */
            enemies_shoot(enemy_pool, enemyBullets, ENEMY_BULLET_POOL_SIZE);

            /* Opdater enemy bullets */
            bullets_update(enemyBullets, ENEMY_BULLET_POOL_SIZE);
            player_hit_by_enemy_bullets(enemyBullets,  ENEMY_BULLET_POOL_SIZE, &p1);
            /* Hit enemies med player bullets (din eksisterende) */

            asteroid_update_pos(&ast);

            if (p1.hp == 0)
            {
                draw_game_over(score,highscore);
                while (1) { } // stop spillet her (simpelt)
            }
            int bonus_collected = 0;
            int kills = bullets_hit_enemies(bullets, BULLET_POOL_SIZE, enemy_pool, &bonus_collected);

            if (kills > 0)
            {
                score += (uint32_t)(kills * 10);
                if (score > highscore)
                    highscore = score;
            }

            if (bonus_collected)
                bullets_powerup_activate(POWERUP_TICKS);

            /* TEST: bonus efter 20 kills */
            if (bullets_test_should_powerup(20))
                bullets_powerup_activate(POWERUP_TICKS);

            asteroid_enemies_collision(&ast, enemy_pool);    
            /* Draw */
            player_push_buffer(current_buffer, p1);
            enemies_push_buffer(current_buffer, enemy_pool);

            /* Tegn først enemy bullets, så player bullets ovenpå (valgfrit) */
            bullets_push_buffer(current_buffer, enemyBullets, ENEMY_BULLET_POOL_SIZE);
            bullets_push_buffer(current_buffer, bullets, BULLET_POOL_SIZE);
            asteroid_push_buffer(current_buffer, ast);

            draw_buffer(current_buffer, shadow_buffer);
            ui_draw_status(p1.hp, p1.hit_count, score, highscore);
        }
    }
}