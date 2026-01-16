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

#include <string.h>
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uart_init(115200);
    timer15_init();
    GPIO_init();

    printf("\x1B[?25h");   // show cursor
    clrscr();
    draw_border();
    printf("\x1B[?25l");   // hide cursor

    uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS];
    uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS];
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
    memset(shadow_buffer,  ' ', SCREEN_ROWS * SCREEN_COLS);

    player p1 = {.x = 50, .y = SCREEN_ROWS - 1, .sx = 5, .sy = 3};

    enemy enemies[MAX_ENEMIES];

    uint8_t input = 0;

    static uint32_t score = 0;
    static uint32_t highscore = 0;

    bullet_init();
    enemies_spawn(enemies);

    while (1) {
        if (timer_flag) {
            timer_flag = 0;

            input = read_joystick();

            clear_buffer(current_buffer);

            // -------------------------
            // 1) Update player
            // -------------------------
            player_update_pos(input, &p1);

            // -------------------------
            // 2) Fire (CENTER PB5)
            // venstre + spids + højre
            // -------------------------
            if (input == 0b00010000) {
                bullet_fire(p1.x,                 p1.y - 1);                // venstre
                bullet_fire(p1.x + (p1.sx / 2),   p1.y - 1);                // spids (midten)
                bullet_fire(p1.x + p1.sx - 1,     p1.y - 1);                // højre
            }

            // -------------------------
            // 3) Update systems
            // -------------------------
            bullet_update();
            enemies_update_pos(enemies);

            // -------------------------
            // 4) Collision: bullets kill enemies
            // enemy is 5x3 at (x,y)
            // -------------------------
            const Bullet* bs = bullet_get_array();

            for (int b = 0; b < MAX_BULLETS; b++) {
                if (!bs[b].active) continue;

                for (int e = 0; e < MAX_ENEMIES; e++) {
                    if (!enemies[e].alive) continue;

                    int ex = enemies[e].x;
                    int ey = enemies[e].y;

                    if (bs[b].x >= ex && bs[b].x < ex + 5 &&
                        bs[b].y >= ey && bs[b].y < ey + 3) {

                        enemies[e].alive = 0;
                        enemies[e].smoke_ticks = 15;  // røg (samme størrelse, men længere tid)
                        score += 10;
                        if (score > highscore) highscore = score;

                        bullet_kill(b);
                        break;
                    }
                }
            }

            // -------------------------
            // 5) Push visuals
            // -------------------------
            player_push_buffer(current_buffer, p1);
            enemies_push_buffer(current_buffer, enemies);

            // Draw bullets
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bs[i].active) continue;

                if (bs[i].y >= 0 && bs[i].y < SCREEN_ROWS &&
                    bs[i].x >= 0 && bs[i].x < SCREEN_COLS) {
                    current_buffer[bs[i].y][bs[i].x] = '|';
                }
            }

            // -------------------------
            // 6) HUD: SCORE + HIGH
            // -------------------------
            char hud[40];
            snprintf(hud, sizeof(hud), "SCORE:%lu  HIGH:%lu",
                     (unsigned long)score, (unsigned long)highscore);

            for (int i = 0; hud[i] != '\0' && (2 + i) < SCREEN_COLS; i++) {
                current_buffer[1][2 + i] = (uint8_t)hud[i];
            }

            // -------------------------
            // 7) Render
            // -------------------------
            draw_buffer(current_buffer, shadow_buffer);
        }
    }
}
