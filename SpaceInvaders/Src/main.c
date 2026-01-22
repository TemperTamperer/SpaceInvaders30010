
#include "main.h"


int main(void)
{
    uart_init(115200);
    timer15_init();
    lcd_init();
    GPIO_init();


    printf("\x1B[2J\x1B[H\x1B[?25l");

    buzzer_set_bg(BG_MAIN_THEME, 1);
    buzzer_bg_start();

    memset(current_buffer, ' ', sizeof(current_buffer));
    memset(shadow_buffer,  ' ', sizeof(shadow_buffer));
    memset(lcd_buffer, 0, sizeof(lcd_buffer));

    game_reset();

    while (1) {
        buzzer_update(TICK_MS);
        uint8_t input = read_joystick();

        handle_state_entry();
        handle_state_logic(input);
    }
}
