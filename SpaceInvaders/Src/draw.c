#include "draw.h"

void draw_buffer(uint8_t current[][SCREEN_COLS], uint8_t shadow_buffer[][SCREEN_COLS]) {
    for (int r = 0; r < SCREEN_ROWS; r++) {
        for (int c = 0; c < SCREEN_COLS; c++) {
            // ONLY print char if the pixel changed since the last frame
            if (current[r][c] != shadow_buffer[r][c]) {
                gotoxy(c + BOARD_OFFSET, r + BOARD_OFFSET);
                putchar(current[r][c]);

                // Update the shadow
                shadow_buffer[r][c] = current[r][c];
            }
        }
    }
}

void clear_buffer(uint8_t current_buffer[][SCREEN_COLS]){
	memset(current_buffer, ' ', SCREEN_ROWS * SCREEN_COLS);
}

void draw_border() {
    // 1. Move cursor to top-left
    gotoxy(1, 1);

    // 2. TOP LINE:
    // \xC9 = ╔, \xCD = ═, \xBB = ╗
    printf("\xC9");
    for (int i = 0; i < SCREEN_COLS; i++) printf("\xCD");
    printf("\xBB\n");

    // 3. MIDDLE ROWS:
    // \xBA = ║
    for (int r = 0; r < SCREEN_ROWS; r++) {
        printf("\xBA"); // Left side

        // Use a precision specifier to print a string of spaces effectively
        // or just a simple loop for clarity:
        for (int c = 0; c < SCREEN_COLS; c++) printf(" ");

        printf("\xBA\n"); // Right side
    }

    // 4. BOTTOM LINE:
    // \xC8 = ╚, \xCD = ═, \xBC = ╝
    printf("\xC8");
    for (int i = 0; i < SCREEN_COLS; i++) printf("\xCD");
    printf("\xBC\n");
}
