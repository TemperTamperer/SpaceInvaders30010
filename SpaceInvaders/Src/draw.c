#include "draw.h"
#include "ansi.h"
#include "game_settings.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "level.h"
#include "asteroids.h"
#include <stdio.h>
#include <string.h>

#define LEVEL_BOX_W 20
#define LEVEL_BOX_H 3
#define LEVEL_BOX_ROW (2 + (SCREEN_ROWS / 2))
#define LEVEL_BOX_COL (2 + ((SCREEN_COLS - LEVEL_BOX_W) / 2))

/* Init screen
   Clears terminal, draws the outer border once, hides cursor */
void draw_game_init_screen(void)
{
    clrscr();
    draw_border();
    printf("\x1B[?25l");
}

/* Game over check
   If hp is 0, show game over screen and freeze */
void draw_check_game_over(uint8_t hp, uint32_t score, uint32_t highscore)
{
    if (hp == 0)
    {
        draw_game_over(score, highscore);
        while (1) { }
    }
}

/* Main frame render
   - Clear frame buffer
   - Push all game objects into buffer
   - Handle level popup box
   - Only print changed chars (shadow buffer)
   - Draw the status line */
void draw_frame(uint8_t current_buffer[SCREEN_ROWS][SCREEN_COLS],
                uint8_t shadow_buffer[SCREEN_ROWS][SCREEN_COLS],
                const player *p,
                enemy enemy_pool[MAX_ENEMIES],
                Bullet enemyBullets[], int enemyBulletCount,
                Bullet playerBullets[], int playerBulletCount,
                const AsteroidSystem *asteroids,
                const LevelState *level,
                uint32_t score, uint32_t highscore)
{
    clear_buffer(current_buffer);

    player_push_buffer(current_buffer, *p);
    enemies_push_buffer(current_buffer, enemy_pool);
    bullets_push_buffer(current_buffer, enemyBullets, enemyBulletCount);
    bullets_push_buffer(current_buffer, playerBullets, playerBulletCount);
    asteroids_draw((AsteroidSystem *)asteroids, current_buffer);

    if (level_popup_active((LevelState *)level))
        draw_level_box(level_get((LevelState *)level));
    else if (level_popup_just_ended((LevelState *)level))
        draw_level_box_clear();

    draw_buffer(current_buffer, shadow_buffer);
    ui_draw_status(p->hp, p->hit_count, score, highscore);
}

/* Level popup box
   Draws a centered 20x3 box with "LEVEL X" */
void draw_level_box(uint8_t level)
{
    char text[16];
    int len = sprintf(text, "LEVEL %u", (unsigned)level);
    int text_col = LEVEL_BOX_COL + (LEVEL_BOX_W - len) / 2;

    printf("\x1B[%d;%dH\xC9", LEVEL_BOX_ROW, LEVEL_BOX_COL);
    for (int i = 0; i < LEVEL_BOX_W - 2; i++) putchar('\xCD');
    printf("\xBB");

    printf("\x1B[%d;%dH\xBA", LEVEL_BOX_ROW + 1, LEVEL_BOX_COL);
    printf("\x1B[%d;%dH%s", LEVEL_BOX_ROW + 1, text_col, text);
    printf("\x1B[%d;%dH\xBA", LEVEL_BOX_ROW + 1, LEVEL_BOX_COL + LEVEL_BOX_W - 1);

    printf("\x1B[%d;%dH\xC8", LEVEL_BOX_ROW + 2, LEVEL_BOX_COL);
    for (int i = 0; i < LEVEL_BOX_W - 2; i++) putchar('\xCD');
    printf("\xBC");
}

/* Level popup clear
   Overwrites the 20x3 area with spaces */
void draw_level_box_clear(void)
{
    for (int r = 0; r < LEVEL_BOX_H; r++)
    {
        printf("\x1B[%d;%dH", LEVEL_BOX_ROW + r, LEVEL_BOX_COL);
        for (int i = 0; i < LEVEL_BOX_W; i++) putchar(' ');
    }
}

/* Status line UI
   Draws HP as red #, hit counter, score and highscore */
void ui_draw_status(uint8_t hp, uint8_t hits, uint32_t score, uint32_t highscore)
{
    const int row = 2;
    const int col = 2;

    printf("\x1B[%d;%dH", row, col);

    printf("HP:");
    for (uint8_t i = 0; i < hp; i++)
        printf("\x1B[31m#\x1B[0m");

    printf("  Hits:%u/5  Score:%lu  High:%lu",
           (unsigned)hits,
           (unsigned long)score,
           (unsigned long)highscore);

    printf("                ");
}

/* Game over screen
   Clears screen, draws border, prints ASCII art, then prints score/high */
void draw_game_over(uint32_t score, uint32_t highscore)
{
    clrscr();
    draw_border();

    static const char *lines[] = {
        "  ######    ###    ##   ##  ####### ",
        " ##       ##   ##  ### ###  ##      ",
        " ##  #### #######  ## # ##  #####   ",
        " ##    ## ##   ##  ##   ##  ##      ",
        "  ######  ##   ##  ##   ##  ####### ",
        "",
        "  ######  ##     ##  #######  ######  ",
        " ##    ##  ##   ##   ##       ##   ## ",
        " ##    ##   ## ##    #####    ######  ",
        " ##    ##    ###     ##       ##   ## ",
        "  ######      #      #######  ##   ## "
    };

    const int n = (int)(sizeof(lines) / sizeof(lines[0]));

    int maxlen = 0;
    for (int i = 0; i < n; i++)
    {
        int len = 0;
        while (lines[i][len] != '\0') len++;
        if (len > maxlen) maxlen = len;
    }

    int inner_w = SCREEN_COLS;
    int inner_h = SCREEN_ROWS;

    int start_row = 2 + (inner_h - (n + 3)) / 2;
    int start_col = 2 + (inner_w - maxlen) / 2;

    if (start_row < 2) start_row = 2;
    if (start_col < 2) start_col = 2;

    printf("\x1B[31m");
    for (int i = 0; i < n; i++)
        printf("\x1B[%d;%dH%s", start_row + i, start_col, lines[i]);
    printf("\x1B[0m");

    char scoreline[64];
    sprintf(scoreline, "Score: %lu   High: %lu",
            (unsigned long)score,
            (unsigned long)highscore);

    int slen = 0;
    while (scoreline[slen] != '\0') slen++;

    int score_col = 2 + (inner_w - slen) / 2;
    if (score_col < 2) score_col = 2;

    printf("\x1B[%d;%dH%s", start_row + n + 2, score_col, scoreline);

    printf("\x1B[?25h");
}

/* Double-buffer print
   Only prints characters that changed since last frame */
void draw_buffer(uint8_t current[][SCREEN_COLS], uint8_t shadow_buffer[][SCREEN_COLS])
{
    for (int r = 0; r < SCREEN_ROWS; r++)
    {
        for (int c = 0; c < SCREEN_COLS; c++)
        {
            if (current[r][c] != shadow_buffer[r][c])
            {
                gotoxy(c + BOARD_OFFSET, r + BOARD_OFFSET);
                putchar(current[r][c]);
                shadow_buffer[r][c] = current[r][c];
            }
        }
    }
}

/* Buffer clear
   Fills the whole frame buffer with spaces */
void clear_buffer(uint8_t current_buffer[][SCREEN_COLS])
{
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
}

/* Border draw
   Draws a single-line box around the whole play area */
void draw_border(void)
{
    gotoxy(1, 1);

    printf("\xC9");
    for (int i = 0; i < SCREEN_COLS; i++) printf("\xCD");
    printf("\xBB\n");

    for (int r = 0; r < SCREEN_ROWS; r++)
    {
        printf("\xBA");
        for (int c = 0; c < SCREEN_COLS; c++) printf(" ");
        printf("\xBA\n");
    }

    printf("\xC8");
    for (int i = 0; i < SCREEN_COLS; i++) printf("\xCD");
    printf("\xBC\n");
}
