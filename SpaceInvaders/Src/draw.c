#include "draw.h"
#include <stdio.h>
#include <string.h>

void ui_draw_status(uint8_t hp, uint8_t hits, uint32_t score, uint32_t highscore)
{
    printf("\x1B[2;3H");        // fast placering (som du havde)
    printf("\x1B[2K");          // ryd hele linjen

    // HP som ###
    printf("HP: ");
    for (uint8_t i = 0; i < hp; i++)
        printf("\x1B[31m###\x1B[0m ");

    // Hits + score + high
    printf(" Hits: %u/5  Score: %lu  High: %lu",
           (unsigned)hits,
           (unsigned long)score,
           (unsigned long)highscore);
}
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

    const int n = (int)(sizeof(lines)/sizeof(lines[0]));

    int maxlen = 0;
    for (int i = 0; i < n; i++) {
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
    for (int i = 0; i < n; i++) {
        printf("\x1B[%d;%dH%s", start_row + i, start_col, lines[i]);
    }
    printf("\x1B[0m");

    char scoreline[64];
    sprintf(scoreline, "Score: %lu   High: %lu",
            (unsigned long)score,
            (unsigned long)highscore);

    int slen = 0; while (scoreline[slen] != '\0') slen++;
    int score_col = 2 + (inner_w - slen) / 2;
    if (score_col < 2) score_col = 2;

    printf("\x1B[%d;%dH%s", start_row + n + 2, score_col, scoreline);

    printf("\x1B[?25h");
}


void draw_buffer(uint8_t current[][SCREEN_COLS], uint8_t shadow_buffer[][SCREEN_COLS])
{
    for (int r = 0; r < SCREEN_ROWS; r++) {
        for (int c = 0; c < SCREEN_COLS; c++) {
            if (current[r][c] != shadow_buffer[r][c]) {
                gotoxy(c + BOARD_OFFSET, r + BOARD_OFFSET);
                putchar(current[r][c]);
                shadow_buffer[r][c] = current[r][c];
            }
        }
    }
}

void clear_buffer(uint8_t current_buffer[][SCREEN_COLS])
{
    memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
}

void draw_border(void)
{
    gotoxy(1, 1);

    printf("\xC9");
    for (int i = 0; i < SCREEN_COLS; i++) printf("\xCD");
    printf("\xBB\n");

    for (int r = 0; r < SCREEN_ROWS; r++) {
        printf("\xBA");
        for (int c = 0; c < SCREEN_COLS; c++) printf(" ");
        printf("\xBA\n");
    }

    printf("\xC8");
    for (int i = 0; i < SCREEN_COLS; i++) printf("\xCD");
    printf("\xBC\n");
}
