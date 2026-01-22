#include "lcd.h"
#include "timer.h"

// Heart pixel data
#define HEART_W 13
#define HEARTS_MAX 3
#define LCD_PAGE_W 128
#define LCD_BUF_SIZE 512

const uint8_t heart_filled_top[HEART_W] = {
    0xF0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFE, 0xFC,
    0xFE, 0xFF, 0xFF, 0xFE, 0xFC, 0xF0
};
const uint8_t heart_filled_bot[HEART_W] = {
    0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F,
    0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
};

const uint8_t heart_hollow_top[HEART_W] = {
    0xF0, 0x0C, 0x02, 0x01, 0x01, 0x02, 0x0C,
    0x02, 0x01, 0x01, 0x02, 0x0C, 0xF0
};
const uint8_t heart_hollow_bot[HEART_W] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x10, 0x08, 0x04, 0x02, 0x01, 0x00
};

// Draw hearts
void lcd_draw_heart(uint8_t health, uint8_t buffer[])
{
    for (uint8_t i = 0; i < HEARTS_MAX; i++)
    {
        int8_t container_fill = health - i - 1;

        for (int j = 0; j < HEART_W; j++)
        {
            int p0_idx = j + (i * 14);
            int p1_idx = LCD_PAGE_W + j + (i * 14);

            if (container_fill < 0) {
                if (p0_idx < LCD_BUF_SIZE) buffer[p0_idx] = heart_hollow_top[j];
                if (p1_idx < LCD_BUF_SIZE) buffer[p1_idx] = heart_hollow_bot[j];
            } else {
                if (p0_idx < LCD_BUF_SIZE) buffer[p0_idx] = heart_filled_top[j];
                if (p1_idx < LCD_BUF_SIZE) buffer[p1_idx] = heart_filled_bot[j];
            }
        }
    }

    lcd_push_buffer(buffer);
}

// Draw score
void lcd_draw_score(int32_t score, uint8_t buffer[])
{
    char score_str[20];
    sprintf(score_str, "Score: %08ld", score);

    int page3_offset = 256; // Page 2
    int x_start = 0;

    for (int i = 0; score_str[i] != '\0'; i++)
    {
        uint8_t ascii_idx = (uint8_t)score_str[i] - 32;

        for (int j = 0; j < 5; j++)
        {
            int x_pos = x_start + (i * 6) + j;

            if (x_pos < LCD_PAGE_W)
            {
                int buffer_idx = page3_offset + x_pos;
                buffer[buffer_idx] = character_data[ascii_idx][j];
            }
        }
    }

    lcd_push_buffer(buffer);
}

// LED control
void set_led(uint8_t input)
{
    if (input == 0)
    {
        GPIOA->ODR |= (0x00000001 << 9);
        GPIOC->ODR |= (0x00000001 << 7);
        GPIOB->ODR |= (0x00000001 << 4);
    }

    if (input == 0b00000001) GPIOA->ODR &= ~(0x00000001 << 9); // PA9 ON
    if (input == 0b00000010) GPIOC->ODR &= ~(0x00000001 << 7); // PC7 ON
    if (input == 0b00000100) GPIOB->ODR &= ~(0x00000001 << 4); // PB4 ON
}
