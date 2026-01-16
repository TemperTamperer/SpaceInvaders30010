#include "enemy.h"

// -------- Enemy sprite 5x3 --------
static const char enemy_sprite[3][5] = {
    {' ', 'W', 'W', 'W', ' '},
    {'W', ' ', 'W', ' ', 'W'},
    {'W', 'W', 'W', 'W', 'W'}
};

// -------- Smoke sprites 5x3 (3 frames) --------
static const char smoke0[3][5] = {
    {' ', '.', '.', '.', ' '},
    {'.', ' ', '.', ' ', '.'},
    {' ', '.', '.', '.', ' '}
};
static const char smoke1[3][5] = {
    {'o', 'o', 'o', 'o', 'o'},
    {'o', ' ', 'o', ' ', 'o'},
    {'o', 'o', 'o', 'o', 'o'}
};
static const char smoke2[3][5] = {
    {'*', '*', '*', '*', '*'},
    {'*', ' ', '*', ' ', '*'},
    {'*', '*', '*', '*', '*'}
};

static const char (*smoke_frames[3])[5] = { smoke0, smoke1, smoke2 };

// Hvor mange enemies der må være alive samtidig
#define ACTIVE_ENEMIES 2

// -------- PRNG (ingen rand()) --------
static uint32_t lfsr = 0xACE1u;

static uint16_t prng16(void) {
    uint32_t bit = lfsr & 1u;
    lfsr >>= 1;
    if (bit) lfsr ^= 0xB400u;
    return (uint16_t)lfsr;
}

static void spawn_one(enemy* e) {
    e->alive = 1;
    e->smoke_ticks = 0;
    e->sx = 5;
    e->sy = 3;
    e->hp = 1;

    uint16_t r = prng16();
    uint16_t max_x = (SCREEN_COLS > 5) ? (SCREEN_COLS - 5) : 0;
    e->x = (max_x > 0) ? (r % max_x) : 0;

    e->y = 2;
}

void enemies_spawn(enemy enemy_pool[MAX_ENEMIES]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemy_pool[i].alive = 0;
        enemy_pool[i].x = 0;
        enemy_pool[i].y = 0;
        enemy_pool[i].sx = 5;
        enemy_pool[i].sy = 3;
        enemy_pool[i].hp = 1;
        enemy_pool[i].smoke_ticks = 0;
    }

    int n = ACTIVE_ENEMIES;
    if (n > MAX_ENEMIES) n = MAX_ENEMIES;

    for (int i = 0; i < n; i++) {
        spawn_one(&enemy_pool[i]);
        enemy_pool[i].y = 2 + (i * 4); // lidt spredning
    }
}

void enemies_update_pos(enemy enemy_pool[MAX_ENEMIES]) {
    static uint32_t tick = 0;
    tick++;

    // Flyt ned hver N ticks (større = langsommere)
    // Hvis du stadig synes de kommer for hurtigt, sæt 24 eller 30
    if ((tick % 24u) != 0u) {
        // røg skal stadig tælle ned
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemy_pool[i].alive && enemy_pool[i].smoke_ticks > 0) {
                enemy_pool[i].smoke_ticks--;
            }
        }
        return;
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {

        // smoke countdown
        if (!enemy_pool[i].alive) {
            if (enemy_pool[i].smoke_ticks > 0) enemy_pool[i].smoke_ticks--;
            continue;
        }

        // move down
        enemy_pool[i].y += 1;

        // If it reaches bottom -> respawn same enemy (keeps count constant)
        if (enemy_pool[i].y >= (SCREEN_ROWS - 3)) {
            spawn_one(&enemy_pool[i]);
        }
    }

    // Ensure we always have ACTIVE_ENEMIES alive (but never more)
    int alive_count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_pool[i].alive) alive_count++;
    }

    int target = ACTIVE_ENEMIES;
    if (target > MAX_ENEMIES) target = MAX_ENEMIES;

    while (alive_count < target) {
        int found = -1;
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (!enemy_pool[i].alive && enemy_pool[i].smoke_ticks == 0) {
                found = i;
                break;
            }
        }
        if (found < 0) break;

        spawn_one(&enemy_pool[found]);
        alive_count++;
    }
}

void enemies_push_buffer(uint8_t buffer[][SCREEN_COLS],
                         enemy enemy_pool[MAX_ENEMIES]) {

    for (int i = 0; i < MAX_ENEMIES; i++) {

        int x = (int)enemy_pool[i].x;
        int y = (int)enemy_pool[i].y;

        if (enemy_pool[i].alive) {
            // draw enemy 5x3
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 5; c++) {
                    int bx = x + c;
                    int by = y + r;

                    if (bx >= 0 && bx < SCREEN_COLS &&
                        by >= 0 && by < SCREEN_ROWS) {

                        char ch = enemy_sprite[r][c];
                        if (ch != ' ') buffer[by][bx] = (uint8_t)ch;
                    }
                }
            }
        }
        else if (enemy_pool[i].smoke_ticks > 0) {
            // draw smoke 5x3 (same size as enemy)
            int frame = enemy_pool[i].smoke_ticks % 3;
            const char (*spr)[5] = smoke_frames[frame];

            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 5; c++) {
                    int bx = x + c;
                    int by = y + r;

                    if (bx >= 0 && bx < SCREEN_COLS &&
                        by >= 0 && by < SCREEN_ROWS) {

                        char ch = spr[r][c];
                        if (ch != ' ') buffer[by][bx] = (uint8_t)ch;
                    }
                }
            }
        }
    }
}
