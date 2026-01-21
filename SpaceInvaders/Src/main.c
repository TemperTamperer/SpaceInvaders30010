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
#include "game_state.h"
#include "lcd.h"

#include "buzzer.h"
#include "buzzer_songs.h"

#include "asteroids.h"
#include "enemy.h"
#include "bullet.h"

#include <stdint.h>
#include <string.h>

#include "level.h"
#include "powerup.h"

#define TICKS_PER_SECOND 20
#define TICK_MS          (1000 / TICKS_PER_SECOND)

#define POWERUP_SECONDS  5
#define POWERUP_TICKS    (POWERUP_SECONDS * TICKS_PER_SECOND)
#define ENEMY_BULLET_POOL_SIZE 16

game_state state = STATE_MENU;

#define ENEMY_BULLET_POOL_SIZE 16

int main(void)
{
    uart_init(115200);
    timer15_init();
    lcd_init();
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

    /* ===== BUZZER INIT + BG MUSIC ===== */
    //buzzer_init();
    buzzer_set_bg(BG_MAIN_THEME, 1);   // loop baggrundsmusik
    buzzer_bg_start();

    uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS];
    uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS];
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
    memset(shadow_buffer,  ' ', SCREEN_ROWS * SCREEN_COLS);

    uint8_t lcd_buffer[512];
    memset(lcd_buffer,0x00,512);

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

    AsteroidSystem asteroids;
    asteroids_init(&asteroids);

    uint8_t prev_center_pressed = 0;
    uint8_t last_level = level_get(&level);

    game_state prev_state = -1;

    while (1)
    {
        if (!timer_flag)
            continue;

        timer_flag = 0;

        /* ===== BUZZER TIMING =====
           Jeres tick er 20 Hz => 50 ms pr tick */
        buzzer_update((uint16_t)TICK_MS);

        input = read_joystick();

        /* Hvis state ændres: tegn ny skærm */
        if (state != prev_state)
        {
            clrscr();

            if (state == STATE_BOSSKEY) {
                boss_draw();
            }
            else if (state == STATE_MENU) {
                menu_draw();
            }
            else if (state == STATE_HELP) {
                help_draw();
            }
            else if (state == STATE_PLAYING) {
                draw_border();
            }
            prev_state = state;
        }

        switch (state)
        {
        case STATE_MENU:
            if (menu_update(input) == 1) {
                clrscr();
                draw_border();
                state = STATE_PLAYING;
            }
            if (menu_update(input) == 2) {
                clrscr();
                draw_border();
                state = STATE_HELP;
            }
            break;

        case STATE_HELP:
            if (menu_update(input) == 1) {
                clrscr();
                draw_border();
                state = STATE_PLAYING;
            }
            break;

        case STATE_BOSSKEY:
            if (menu_update(input) == 1) {
                state = STATE_PLAYING;
            }
            break;

        case STATE_PLAYING:
            if (menu_update(input) == 3) {
                state = STATE_BOSSKEY;
                break;
            }

            enemy_spawn_counter++;
            enemy_move_counter++;

            bonus_spawn_tick(enemy_pool);

            clear_buffer(current_buffer);

            /* Bevægelse: kun retninger (center påvirker ikke movement) */
            uint8_t move_input = input & (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT);
            player_update_pos(move_input, &p1);

            /* Skyd (player) */
            uint8_t center_just_pressed =
                joystick_just_pressed(input, JOY_CENTER, &prev_center_pressed);

            int startX = (p1.x + (p1.sx / 2));
            int startY = (p1.y - 1);

            if (center_just_pressed) {
                buzzer_play_sfx(SFX_SHOOT);
            }

            bullets_handle_shoot(bullets, BULLET_POOL_SIZE,
                                 center_just_pressed, startX, startY);

            bullets_powerup_tick();

            asteroid_gravity(bullets, ast);
            bullets_update(bullets, BULLET_POOL_SIZE);
            bullets_update(enemyBullets, ENEMY_BULLET_POOL_SIZE);

            /* Enemy move/spawn */
            if (enemy_move_counter > 15) {
                enemy_move_counter = 0;
                enemies_update_pos(enemy_pool);
            }

            if (enemy_spawn_counter > 80) {
                enemy_spawn_counter = 0;
                enemies_spawn(enemy_pool);
                asteroid_update_pos(&ast);
            }

            /* Fjender skyder */
            enemies_shoot(enemy_pool, enemyBullets, ENEMY_BULLET_POOL_SIZE);

            /* Enemy bullets + player hit SFX (detekter HP fald) */
            uint8_t hp_before = p1.hp;

            bullets_update(enemyBullets, ENEMY_BULLET_POOL_SIZE);
            player_hit_by_enemy_bullets(enemyBullets, ENEMY_BULLET_POOL_SIZE, &p1);

            if (p1.hp < hp_before) {
                buzzer_play_sfx(SFX_PLAYER_HIT);
            }


            /* Game over */
            if (p1.hp == 0) {
                buzzer_play_sfx(SFX_GAMEOVER);
                draw_game_over(score, highscore);
                while (1) { } // stop spillet her (simpelt)
            }

            /* Hit enemies med player bullets */
            int bonus_collected = 0;
            int kills = bullets_hit_enemies(bullets, BULLET_POOL_SIZE,
                                           enemy_pool, &bonus_collected);

            if (kills > 0) {
                buzzer_play_sfx(SFX_ENEMY_DEATH);

                score += (uint32_t)(kills * 10);
                if (score > highscore)
                    highscore = score;
            }

            if (bonus_collected) {
                buzzer_play_sfx(SFX_BONUS);
                bullets_powerup_activate(POWERUP_TICKS);
            }

            /* TEST: bonus efter 20 kills */
            if (bullets_test_should_powerup(20))
                bullets_powerup_activate(POWERUP_TICKS);

            asteroid_enemies_collision(&ast, enemy_pool);

            /* Draw */
            player_push_buffer(current_buffer, p1);
            enemies_push_buffer(current_buffer, enemy_pool);

            bullets_push_buffer(current_buffer, enemyBullets, ENEMY_BULLET_POOL_SIZE);
            bullets_push_buffer(current_buffer, bullets, BULLET_POOL_SIZE);
            asteroid_push_buffer(current_buffer, ast);

            draw_buffer(current_buffer, shadow_buffer);

        	lcd_draw_heart(p1.hp, lcd_buffer);
        	lcd_draw_score(score, lcd_buffer);

            ui_draw_status(p1.hp, p1.hit_count, score, highscore);
            break;
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

        asteroids_tick(&asteroids, enemy_pool);

        clear_buffer(current_buffer);

        player_push_buffer(current_buffer, p1);
        enemies_push_buffer(current_buffer, enemy_pool);
        bullets_push_buffer(current_buffer, enemyBullets, ENEMY_BULLET_POOL_SIZE);
        bullets_push_buffer(current_buffer, playerBullets, BULLET_POOL_SIZE);
        asteroids_draw(&asteroids, current_buffer);

        if (level_popup_active(&level))
            draw_level_box(level_get(&level));
        else if (level_popup_just_ended(&level))
            draw_level_box_clear();

        draw_buffer(current_buffer, shadow_buffer);
        ui_draw_status(p1.hp, p1.hit_count, score, highscore);
    }
}
