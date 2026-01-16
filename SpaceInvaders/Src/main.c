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

#define DT_SECONDS 0.05f
#define POWERUP_TICKS ((int)(10.0f / DT_SECONDS))

static Bullet bullets[BULLET_POOL_SIZE];

int main(void)
{
    uart_init(115200);
    timer15_init();
    GPIO_init();
    printf("\x1B[?25h");

    uint16_t enemy_spawn_counter = 0;
    uint16_t enemy_move_counter  = 0;

    clrscr();
    draw_border();
    printf("\x1B[?25l");

    uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS];
    uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS];
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
    memset(shadow_buffer,  ' ', SCREEN_ROWS * SCREEN_COLS);

    player p1 = {.x = 50, .y = SCREEN_ROWS - 1, .sx = 5, .sy = 3};
    enemy enemy_pool[MAX_ENEMIES];
    memset(enemy_pool, 0, sizeof(enemy_pool));

    bullets_init(bullets, BULLET_POOL_SIZE);

    uint8_t prev_center_pressed = 0;
    uint8_t input = 0;

    while (1)
    {
        if (timer_flag)
        {
            timer_flag = 0;
            enemy_spawn_counter++;
            enemy_move_counter++;

            input = read_joystick();

            clear_buffer(current_buffer);

            player_update_pos(input, &p1);

            uint8_t center_pressed = (input & JOY_CENTER) ? 1 : 0;
            uint8_t center_just_pressed = (center_pressed && !prev_center_pressed) ? 1 : 0;
            prev_center_pressed = center_pressed;

            float startX = (float)(p1.x + (p1.sx / 2));
            float startY = (float)(p1.y - 1);

            bullets_handle_shoot(bullets, BULLET_POOL_SIZE, center_just_pressed, startX, startY);
            bullets_powerup_tick();

            bullets_update(bullets, BULLET_POOL_SIZE, DT_SECONDS);

            if (enemy_move_counter > 15)
            {
                enemy_move_counter = 0;
                enemies_update_pos(enemy_pool);
            }

            if (enemy_spawn_counter > 30)
            {
                enemy_spawn_counter = 0;
                enemies_spawn(enemy_pool);
            }

            if (bullets_hit_enemies(bullets, BULLET_POOL_SIZE, enemy_pool))
                bullets_powerup_activate(POWERUP_TICKS);

            player_push_buffer(current_buffer, p1);
            enemies_push_buffer(current_buffer, enemy_pool);
            bullets_push_buffer(current_buffer, bullets, BULLET_POOL_SIZE);

            draw_buffer(current_buffer, shadow_buffer);
        }
    }
}
