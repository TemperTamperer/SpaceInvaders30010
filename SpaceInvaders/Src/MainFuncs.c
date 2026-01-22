
#include "main.h"

player p1;
enemy enemy_pool[MAX_ENEMIES];
Bullet playerBullets[BULLET_POOL_SIZE];
Bullet enemyBullets[ENEMY_BULLET_POOL_SIZE];
EnemyShootState shootState;

LevelState level;
PowerupState powerup;
asteroid ast;

uint32_t score = 0;
uint32_t highscore = 0;

uint16_t enemy_spawn_counter = 0;
uint16_t enemy_move_counter  = 0;

uint8_t prev_center_pressed = 0;
uint8_t last_level = 0;



game_state state = STATE_MENU;
game_state prev_state = -1;

void game_reset(void)
{
    score = 0;

    player_init(&p1);
    enemies_init(enemy_pool);

    bullets_init(playerBullets, BULLET_POOL_SIZE);
    bullets_init(enemyBullets, ENEMY_BULLET_POOL_SIZE);

    level_init(&level);
    powerup_init(&powerup);

    enemy_spawn_counter = 0;
    enemy_move_counter  = 0;
    last_level = level_get(&level);

    ast.x = 2;
    ast.y = 20;
    ast.sx = 19;
    ast.sy = 7;
    ast.alive = 1;
    ast.clean = 1;
}

void handle_state_entry(void)
{
    if (state == prev_state) return;

    clrscr();

    switch (state) {
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
            break;
        case STATE_BOSSKEY:
            boss_draw();
            break;
        default:
            break;
    }

    prev_state = state;
}

void handle_state_logic(uint8_t input)
{
    int action = menu_update(input);

    switch (state) {

    case STATE_MENU:
        if (action == 1) {
            game_reset();
            prev_state = -1;
            state = STATE_PLAYING;
        } else if (action == 2) {
            prev_state = -1;
            state = STATE_HELP;
        }
        break;

    case STATE_HELP:
        if (action == 1) {
            prev_state = -1;
            state = STATE_PLAYING;
        }
        break;

    case STATE_GAME_OVER:
        if (action == 1) {
            game_reset();
            prev_state = -1;
            state = STATE_PLAYING;
        }
        break;

    case STATE_BOSSKEY:
        if (action == 1) {
            prev_state = -1;
            state = STATE_PLAYING;
        }
        break;

    case STATE_PLAYING:
        if (action == 3) {
            prev_state = -1;
            state = STATE_BOSSKEY;
            break;
        }

        game_update(input);
        game_draw();
        break;
    }
}

void game_update(uint8_t input)
{
    timer_wait_for_tick();

    clear_buffer(current_buffer);

    enemy_spawn_counter++;
    enemy_move_counter++;

    if (enemy_move_counter > 15) {
        enemy_move_counter = 0;
        enemies_update_pos(enemy_pool);
        asteroid_enemies_collision(&ast, enemy_pool);
    }

    if (enemy_spawn_counter > 80) {
        enemy_spawn_counter = 0;
        enemies_spawn(enemy_pool);
        asteroid_update_pos(&ast);
    }

    uint8_t move_input = input & (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT);
    player_update_pos(move_input, &p1);

    uint8_t center_just_pressed =
        joystick_just_pressed(input, JOY_CENTER, &prev_center_pressed);

    int sx, sy;
    player_get_shoot_pos(&p1, &sx, &sy);

    powerup_shoot(&powerup, playerBullets, BULLET_POOL_SIZE,
                  center_just_pressed, sx, sy);

    asteroid_gravity(playerBullets, ast);
    bullets_update(playerBullets, BULLET_POOL_SIZE);

    int kills = bullets_hit_enemies(playerBullets,
                                    BULLET_POOL_SIZE,
                                    enemy_pool);

    enemies_shoot(enemy_pool, enemyBullets,
                  ENEMY_BULLET_POOL_SIZE,
                  &shootState,
                  level_get(&level));

    bullets_update(enemyBullets, ENEMY_BULLET_POOL_SIZE);

    if (player_hit_by_enemy_bullets(enemyBullets,
                                    ENEMY_BULLET_POOL_SIZE,
                                    &p1)) {
        buzzer_play_sfx(SFX_PLAYER_HIT);
    }

    if (center_just_pressed)
        buzzer_play_sfx(SFX_SHOOT);

    if (kills > 0) {
        buzzer_play_sfx(SFX_ENEMY_DEATH);
        score += kills * 10;
        if (score > highscore) highscore = score;
    }

    if (p1.hp == 0) {
        buzzer_play_sfx(SFX_GAMEOVER);
        prev_state = -1;
        state = STATE_GAME_OVER;
    }

    powerup_update_from_score(&powerup, score);
    powerup_tick(&powerup);

    level_update_from_score(&level, score);
}

void game_draw(void)
{
    player_push_buffer(current_buffer, p1);
    enemies_push_buffer(current_buffer, enemy_pool);

    bullets_push_buffer(current_buffer, enemyBullets, ENEMY_BULLET_POOL_SIZE);
    bullets_push_buffer(current_buffer, playerBullets, BULLET_POOL_SIZE);
    asteroid_push_buffer(current_buffer, ast);

    draw_buffer(current_buffer, shadow_buffer);

    lcd_draw_heart(p1.hp, lcd_buffer);
    lcd_draw_score(score, lcd_buffer);

    ui_draw_status(p1.hp, p1.hit_count, score, highscore);
}
