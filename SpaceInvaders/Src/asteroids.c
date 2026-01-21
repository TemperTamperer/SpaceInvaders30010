#include "asteroids.h"

static void asteroid_push_buffer(uint8_t buffer[][SCREEN_COLS], asteroid ast)
{
    static const uint8_t sprite[7][9] = {
        {' ', ' ', '.', '-', '-', '-', '.', ' ', ' '},
        {' ', '/', ' ', '*', ' ', ' ', ' ', '\\', ' '},
        {'(', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ')'},
        {'|', ' ', ' ', '(', ' ', ')', ' ', ' ', '|'},
        {'(', ' ', ' ', ' ', ' ', ' ', ' ', '*', ')'},
        {' ', '\\', ' ', ' ', '_', ' ', ' ', '/', ' '},
        {' ', ' ', '\'', '-', '-', '-', '\'', ' ', ' '}
    };

    if (!ast.alive) return;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 9; j++) {
            int ty = ast.y - i;
            int tx = ast.x + j;

            if (ty >= 0 && ty < SCREEN_ROWS && tx >= 0 && tx < SCREEN_COLS) {
                uint8_t c = sprite[6 - i][j];
                if (c != ' ') buffer[ty][tx] = c;
            }
        }
    }
}

static void asteroid_enemies_collision(asteroid *ast, enemy enemy_pool[MAX_ENEMIES])
{
    if (!ast->alive) return;

    for (int e = 0; e < MAX_ENEMIES; e++) {
        if (enemy_pool[e].alive == 1) {
            if (enemy_pool[e].x > ast->x && enemy_pool[e].x < ast->x + ast->sx) {
                if (enemy_pool[e].y < ast->y && enemy_pool[e].y > ast->y - ast->sy) {
                    enemy_pool[e].alive = 0;
                }
            }
        }
    }
}

void asteroids_init(AsteroidSystem *s)
{
    s->ast.alive = 0;
    s->ast.sx = 9;
    s->ast.sy = 7;
    s->ast.x = 0;
    s->ast.y = 0;

    s->move_counter = 0;
    s->spawn_counter = 0;
    s->next_spawn = 300;
    s->rng = 2463534242u;
}

void asteroids_tick(AsteroidSystem *s, enemy enemy_pool[MAX_ENEMIES])
{
    s->rng ^= s->rng << 13;
    s->rng ^= s->rng >> 17;
    s->rng ^= s->rng << 5;

    if (s->ast.alive == 0) {
        s->spawn_counter++;
        if (s->spawn_counter >= s->next_spawn) {
            s->spawn_counter = 0;
            s->next_spawn = 250 + (s->rng % 500);

            s->ast.alive = 1;
            s->ast.x = -9;
            s->ast.sx = 9;
            s->ast.sy = 7;

            int center = SCREEN_ROWS / 2;
            int range = 6;
            s->ast.y = (int16_t)(center - range + (s->rng % (uint32_t)(range * 2 + 1)));

            s->move_counter = 0;
        }
    } else {
        s->move_counter++;
        if (s->move_counter >= 4) {
            s->move_counter = 0;
            s->ast.x += ASTEROID_SPEED;
        }

        if (s->ast.x + s->ast.sx > 0) {
            asteroid_enemies_collision(&s->ast, enemy_pool);
        }

        if (s->ast.x > SCREEN_COLS) {
            s->ast.alive = 0;
            s->spawn_counter = 0;
        }
    }
}

void asteroids_draw(const AsteroidSystem *s, uint8_t buffer[][SCREEN_COLS])
{
    if (s->ast.alive == 1)
        asteroid_push_buffer(buffer, s->ast);
}
