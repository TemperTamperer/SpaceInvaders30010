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
#define TICK_MS          (1000 / TICKS_PER_SECOND)

#define POWERUP_SECONDS  5
#define POWERUP_TICKS    (POWERUP_SECONDS * TICKS_PER_SECOND)
#define ENEMY_BULLET_POOL_SIZE 16

game_state state = STATE_MENU;


void game_reset(
    player *p1,
    enemy enemy_pool[],
    Bullet playerBullets[],
    Bullet enemyBullets[],
    LevelState *level,
    PowerupState *powerup,
    asteroid *ast,
    uint32_t *score,
    uint16_t *enemy_spawn_counter,
    uint16_t *enemy_move_counter
) {
    *score = 0;

    player_init(p1);
    enemies_init(enemy_pool);

    bullets_init(playerBullets, BULLET_POOL_SIZE);
    bullets_init(enemyBullets, ENEMY_BULLET_POOL_SIZE);

    level_init(level);
    powerup_init(powerup);

    *enemy_spawn_counter = 0;
    *enemy_move_counter  = 0;

    ast->x = 2;
    ast->y = 20;
    ast->sx = 19;
    ast->sy = 7;
    ast->alive = 1;
    ast->clean = 1;
}


int main(void)
{
    uart_init(115200);
    timer15_init();
    lcd_init();
    //GPIO_init(); uncomment if on shiled joystick is to be used
    GPIO_30010_init();
    printf("\x1B[?h"); // go to (1,1) on putty
    printf("\x1B[?25h"); // remove cursor

    /* ===== BUZZER INIT + BG MUSIC ===== */
    //buzzer_init();
    buzzer_set_bg(BG_MAIN_THEME, 1);   // loop baggrundsmusik
    buzzer_bg_start();

    //Buffer and shadow mask setup
    uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS];
    uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS];
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
    memset(shadow_buffer,  ' ', SCREEN_ROWS * SCREEN_COLS);

    //LCD buffer setup
    uint8_t lcd_buffer[512];
    memset(lcd_buffer,0x00,512);

    uint32_t score = 0;
    uint32_t highscore = 0;

    uint16_t enemy_spawn_counter = 0;
    uint16_t enemy_move_counter  = 0;

    player p1;
    player_init(&p1);

    enemy enemy_pool[MAX_ENEMIES];
    enemies_init(enemy_pool);

    asteroid ast = {.x = 2, .y = 20, .sx = 9, .sy = 7, .alive = 1, .clean = 1};

    //Player and Enemy bullets setup
    Bullet playerBullets[BULLET_POOL_SIZE];
    Bullet enemyBullets[ENEMY_BULLET_POOL_SIZE];
    bullets_init(playerBullets, BULLET_POOL_SIZE);
    bullets_init(enemyBullets, ENEMY_BULLET_POOL_SIZE);

    EnemyShootState shootState = {0};

    LevelState level;
    level_init(&level);

    PowerupState powerup;
    powerup_init(&powerup);

    uint8_t prev_center_pressed = 0;
    uint8_t last_level = level_get(&level);

    game_state prev_state = -1;

    while (1)
    {

        /* ===== BUZZER TIMING =====
           Jeres tick er 20 Hz => 50 ms pr tick */
        buzzer_update((uint16_t)TICK_MS);

        //uint8_t input = read_joystick(); //uncomment if using shield joystick
        uint8_t input = read_30010_joystick();

        /* Hvis state ændres: tegn ny skærm */
        if (state != prev_state) {
            clrscr();

            switch (state) {
                case STATE_BOSSKEY:
                    boss_draw();
                    break;
                case STATE_MENU:
                    menu_draw();
                    break;
                case STATE_HELP:
                    help_draw();
                    break;
                case STATE_PLAYING:
                    draw_border();
                    break;
                case STATE_GAME_OVER:
                	draw_game_over(score, highscore);
                	printf("\x1B[?h");
                    break;
                default:
                    // Optional: handle unexpected states
                    break;
            }
            prev_state = state;
        }

        int menu_action = menu_update(input);

        switch (state)
        {
        case STATE_MENU:
            if (menu_action == 1) {
                clrscr();
                state = STATE_PLAYING;
            }
            else if (menu_action == 2) {
                clrscr();
                state = STATE_HELP;
            }
            break;

        case STATE_HELP:
            if (menu_action == 1) {
                clrscr();
                state = STATE_PLAYING;
            }
            break;

        case STATE_BOSSKEY:
            if (menu_action == 1) {
                state = STATE_PLAYING;
            }
            break;
        case STATE_GAME_OVER:
            if (menu_action == 1) {
                game_reset(&p1, enemy_pool,
                           playerBullets, enemyBullets,
                           &level, &powerup, &ast,
                           &score,
                           &enemy_spawn_counter,
                           &enemy_move_counter);

                prev_state = -1;   // force redraw
                state = STATE_PLAYING;
            }
            break;

        case STATE_PLAYING:
            if (menu_action == 3) {
            	printf("\x1B[?25l");
                state = STATE_BOSSKEY;
                break;
            }



            //MAIN GAME LOOP:
            timer_wait_for_tick();

            clear_buffer(current_buffer);

            //enemy counters
            enemy_spawn_counter++;
            enemy_move_counter++;

            // Enemy/asteroid move and spawn
            if (enemy_move_counter > 15) {
            	enemy_move_counter = 0;
            	enemies_update_pos(enemy_pool, &p1);

                asteroid_enemies_collision(&ast, enemy_pool);
                set_led(0); //Turns LED off
            }

            if (enemy_spawn_counter > 80) {
            	enemy_spawn_counter = 0;
            	enemies_spawn(enemy_pool);

            	asteroid_update_pos(&ast);
            }

            // Movement
            uint8_t move_input = input & (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT);
            player_update_pos(move_input, &p1);

            // Player bullet handling
            uint8_t center_just_pressed =
                joystick_just_pressed(input, JOY_CENTER, &prev_center_pressed);

            int startX, startY;
            player_get_shoot_pos(&p1, &startX, &startY);

            powerup_shoot(&powerup,
                      playerBullets,
                      BULLET_POOL_SIZE,
                      center_just_pressed,
                      startX,
                      startY);

            asteroid_gravity(playerBullets, ast);
            bullets_update(playerBullets, BULLET_POOL_SIZE);
            int bonus_collected = 0;
            int kills = bullets_hit_enemies(playerBullets, BULLET_POOL_SIZE,
                                           enemy_pool);

            //Enemy bullet handling
            enemies_shoot(enemy_pool,
                      enemyBullets,
                      ENEMY_BULLET_POOL_SIZE,
                      &shootState,
                      level_get(&level));
            bullets_update(enemyBullets, ENEMY_BULLET_POOL_SIZE);


            if(player_hit_by_enemy_bullets(enemyBullets, ENEMY_BULLET_POOL_SIZE, &p1)){
            	set_led(0b00000100); //Sets LED to red color
            	buzzer_play_sfx(SFX_PLAYER_HIT);;
            }


            if (center_just_pressed) {
                            buzzer_play_sfx(SFX_SHOOT);
                        }
            //hit scan
            if (kills > 0) {
                buzzer_play_sfx(SFX_ENEMY_DEATH);

                score += (uint32_t)(kills * 10);
                if (score > highscore)
                    highscore = score;
            }


            if (p1.hp == 0) {
            	buzzer_play_sfx(SFX_GAMEOVER);
            	state = STATE_GAME_OVER;
            }

            //Power up handling
            powerup_update_from_score(&powerup, score);
            powerup_tick(&powerup);

            //Level handling
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

            if (level_popup_active(&level))
            level_popup_tick(&level);

            if (level_popup_active((LevelState *)&level))
                draw_level_box(level_get((LevelState *)&level));
            else if (level_popup_just_ended((LevelState *)&level))
                draw_level_box_clear();


            //draw and buffer functions
            player_push_buffer(current_buffer, p1);
            enemies_push_buffer(current_buffer, enemy_pool);

            bullets_push_buffer(current_buffer, enemyBullets, ENEMY_BULLET_POOL_SIZE);
            bullets_push_buffer(current_buffer, playerBullets, BULLET_POOL_SIZE);
            asteroid_push_buffer(current_buffer, ast);

            draw_buffer(current_buffer, shadow_buffer);

        	lcd_draw_heart(p1.hp, lcd_buffer);
        	lcd_draw_score(score, lcd_buffer);

            ui_draw_status(p1.hp, p1.hit_count, score, highscore);
            break;
        }

    }
}
