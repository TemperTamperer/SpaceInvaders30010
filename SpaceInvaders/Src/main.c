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

/* Tick setup */
#define TICKS_PER_SECOND        20u
#define TICK_MS                 (1000u / TICKS_PER_SECOND)

/* Gameplay timing (ticks) */
#define ENEMY_MOVE_TICKS        15u
#define ENEMY_SPAWN_TICKS       80u

/* Bullet pools */
#define ENEMY_BULLET_POOL_SIZE  16u

/* Game over sound length */
#define GAMEOVER_SOUND_MS       3000u
#define GAMEOVER_SOUND_TICKS    (GAMEOVER_SOUND_MS / TICK_MS)

/* LED mask (your code uses this for red) */
#define LED_RED_MASK            0b00000100

/* Asteroid start values (keep consistent) */
#define AST_START_X             2u
#define AST_START_Y             20u
#define AST_SX                  9u
#define AST_SY                  7u

static game_state state = STATE_MENU;

/* Reset game to a known start state */
static void game_reset(
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
    bullets_init(enemyBullets, (int)ENEMY_BULLET_POOL_SIZE);

    level_init(level);
    powerup_init(powerup);

    *enemy_spawn_counter = 0;
    *enemy_move_counter  = 0;

    ast->x = AST_START_X;
    ast->y = AST_START_Y;
    ast->sx = AST_SX;
    ast->sy = AST_SY;
    ast->alive = 1;
    ast->clean = 1;
}

static void bg_music_set_for_level(uint8_t lvl)
{
    /* Assumes you made these different in buzzer_songs.c */
    if (lvl == 1)      buzzer_set_bg(BG_LEVEL_1, 1);
    else if (lvl == 2) buzzer_set_bg(BG_LEVEL_2, 1);
    else               buzzer_set_bg(BG_LEVEL_3, 1);
}

int main(void)
{
    uart_init(115200);
    timer15_init();
    lcd_init();

    /* Use one joystick init path only (keep your choice) */
    GPIO_30010_init();   // joystick on shield
    //GPIO_init();       // joystick on board (uncomment if needed)

    /* Terminal: show cursor once, then hide during game */
    printf("\x1B[?25h");   // show cursor
    printf("\x1B[?25l");   // hide cursor

    /* Buzzer init (must be called, otherwise no sound config is done) */
    buzzer_init();
    /* No menu music: we will start bg music when entering STATE_PLAYING */

    /* Buffers for terminal graphics */
    uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS];
    uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS];
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
    memset(shadow_buffer,  ' ', SCREEN_ROWS * SCREEN_COLS);

    /* LCD framebuffer */
    uint8_t lcd_buffer[512];
    memset(lcd_buffer, 0x00, sizeof(lcd_buffer));

    uint32_t score = 0;
    uint32_t highscore = 0;

    uint16_t enemy_spawn_counter = 0;
    uint16_t enemy_move_counter  = 0;

    player p1;
    player_init(&p1);

    enemy enemy_pool[MAX_ENEMIES];
    enemies_init(enemy_pool);

    asteroid ast = {
        .x = AST_START_X, .y = AST_START_Y,
        .sx = AST_SX, .sy = AST_SY,
        .alive = 1, .clean = 1
    };

    Bullet playerBullets[BULLET_POOL_SIZE];
    Bullet enemyBullets[ENEMY_BULLET_POOL_SIZE];
    bullets_init(playerBullets, BULLET_POOL_SIZE);
    bullets_init(enemyBullets, (int)ENEMY_BULLET_POOL_SIZE);

    EnemyShootState shootState = {0};

    LevelState level;
    level_init(&level);

    PowerupState powerup;
    powerup_init(&powerup);

    uint8_t prev_center_pressed = 0;
    uint8_t last_level = level_get(&level);

    game_state prev_state = (game_state)-1;

    /* We start bg music once when we enter PLAYING */
    uint8_t bg_started = 0;

    /* Game over sound handling */
    uint16_t gameover_ticks_left = 0;
    uint8_t gameover_sound_started = 0;

    while (1)
    {
        /* Read joystick (shield) */
        uint8_t input = read_30010_joystick();
        //uint8_t input = read_joystick(); // board joystick (if used)

        /* Redraw UI when state changes */
        if (state != prev_state) {
            clrscr();

            if (state == STATE_BOSSKEY) {
                boss_draw();
            } else if (state == STATE_MENU) {
                menu_draw();
            } else if (state == STATE_HELP) {
                help_draw();
            } else if (state == STATE_PLAYING) {
                draw_border();
            } else if (state == STATE_GAME_OVER) {
                draw_game_over(score, highscore);
                printf("\x1B[?25h"); // show cursor on game over screen
            }

            /* If we leave PLAYING, stop bg music (no menu music) */
            if (prev_state == STATE_PLAYING && state != STATE_PLAYING) {
                buzzer_bg_stop();
                buzzer_set_bg((buzzer_song_t){0}, 0);
                bg_started = 0;
            }

            /* Reset game-over sound state on entering GAME_OVER */
            if (state == STATE_GAME_OVER) {
                gameover_sound_started = 0;
                gameover_ticks_left = 0;
            }

            prev_state = state;
        }

        int menu_action = menu_update(input);

        switch (state)
        {
        case STATE_MENU:
            if (menu_action == 1) {
                state = STATE_PLAYING;
            } else if (menu_action == 2) {
                state = STATE_HELP;
            }
            break;

        case STATE_HELP:
            if (menu_action == 1) {
                state = STATE_PLAYING;
            }
            break;

        case STATE_BOSSKEY:
            if (menu_action == 1) {
                state = STATE_PLAYING;
            }
            break;

        case STATE_GAME_OVER:
            /* Start game-over SFX once, then let it run ~3 seconds */
            if (!gameover_sound_started) {
                buzzer_bg_stop();
                buzzer_play_sfx(SFX_GAMEOVER);
                gameover_ticks_left = (uint16_t)GAMEOVER_SOUND_TICKS;
                gameover_sound_started = 1;
            }

            /* Keep buzzer sequencer running even on game over screen */
            timer_wait_for_tick();
            buzzer_update((uint16_t)TICK_MS);

            if (gameover_ticks_left > 0) {
                gameover_ticks_left--;
                if (gameover_ticks_left == 0) {
                    /* Stop any remaining sound so it doesn't hang */
                    buzzer_enable(0);
                    buzzer_enable(1);
                }
            }

            if (menu_action == 1) {
                game_reset(&p1, enemy_pool,
                           playerBullets, enemyBullets,
                           &level, &powerup, &ast,
                           &score,
                           &enemy_spawn_counter,
                           &enemy_move_counter);

                prev_state = (game_state)-1; // force redraw
                state = STATE_PLAYING;
            }
            break;

        case STATE_PLAYING:
        {
            if (menu_action == 3) {
                state = STATE_BOSSKEY;
                break;
            }

            /* Game tick */
            timer_wait_for_tick();

            /* Important: update buzzer on tick so music doesn't "freeze" */
            buzzer_update((uint16_t)TICK_MS);

            /* Start bg music the first time we enter PLAYING */
            if (!bg_started) {
                bg_music_set_for_level(level_get(&level));
                buzzer_bg_start();
                bg_started = 1;
                last_level = level_get(&level);
            }

            clear_buffer(current_buffer);

            enemy_spawn_counter++;
            enemy_move_counter++;

            /* Enemy movement tick */
            if (enemy_move_counter > ENEMY_MOVE_TICKS) {
                enemy_move_counter = 0;
                enemies_update_pos(enemy_pool, &p1);

                asteroid_enemies_collision(&ast, enemy_pool);
                set_led(0);
            }

            /* Enemy spawn tick */
            if (enemy_spawn_counter > ENEMY_SPAWN_TICKS) {
                enemy_spawn_counter = 0;
                enemies_spawn(enemy_pool);

                asteroid_update_pos(&ast);
            }

            /* Player movement */
            uint8_t move_input = input & (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT);
            player_update_pos(move_input, &p1);

            /* Shooting */
            uint8_t center_just_pressed =
                joystick_just_pressed(input, JOY_CENTER, &prev_center_pressed);

            int startX, startY;
            player_get_shoot_pos(&p1, &startX, &startY);

            if (center_just_pressed) {
                buzzer_play_sfx(SFX_SHOOT);
            }

            powerup_shoot(&powerup,
                          playerBullets,
                          BULLET_POOL_SIZE,
                          center_just_pressed,
                          startX,
                          startY);

            /* Player bullets */
            asteroid_gravity(playerBullets, ast);
            bullets_update(playerBullets, BULLET_POOL_SIZE);

            int kills = bullets_hit_enemies(playerBullets, BULLET_POOL_SIZE, enemy_pool);
            if (kills > 0) {
                buzzer_play_sfx(SFX_ENEMY_DEATH);

                score += (uint32_t)(kills * 10u);
                if (score > highscore) highscore = score;
            }

            /* Enemy bullets */
            enemies_shoot(enemy_pool,
                          enemyBullets,
                          (int)ENEMY_BULLET_POOL_SIZE,
                          &shootState,
                          level_get(&level));
            bullets_update(enemyBullets, (int)ENEMY_BULLET_POOL_SIZE);

            if (player_hit_by_enemy_bullets(enemyBullets, (int)ENEMY_BULLET_POOL_SIZE, &p1)) {
                set_led(LED_RED_MASK);
                buzzer_play_sfx(SFX_PLAYER_HIT);
            }

            /* Death -> go to game over state (sound handled there) */
            if (p1.hp == 0) {
                state = STATE_GAME_OVER;
                break;
            }

            /* Powerup */
            powerup_update_from_score(&powerup, score);
            powerup_tick(&powerup);

            /* Level */
            level_update_from_score(&level, score);

            uint8_t now_level = level_get(&level);
            if (now_level != last_level) {
                last_level = now_level;

                /* Switch music when level changes */
                bg_music_set_for_level(now_level);
                buzzer_bg_start();

                /* Reset enemies/bullets pacing for new level */
                enemies_reset(enemy_pool, &shootState);
                bullets_init(enemyBullets, (int)ENEMY_BULLET_POOL_SIZE);

                enemy_spawn_counter = 0;
                enemy_move_counter  = 0;
            }

            if (level_popup_active(&level)) level_popup_tick(&level);

            if (level_popup_active(&level))
                draw_level_box(level_get(&level));
            else if (level_popup_just_ended(&level))
                draw_level_box_clear();

            /* Draw */
            player_push_buffer(current_buffer, p1);
            enemies_push_buffer(current_buffer, enemy_pool);

            bullets_push_buffer(current_buffer, enemyBullets, (int)ENEMY_BULLET_POOL_SIZE);
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
}
