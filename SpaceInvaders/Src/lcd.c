#include "lcd.h"
#include "timer.h"

//Pixel art information for LCD heart (filled)
const uint8_t heart_filled_top[13] = {
    0xF0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFE, 0xFC,
    0xFE, 0xFF, 0xFF, 0xFE, 0xFC, 0xF0
};
const uint8_t heart_filled_bot[13] = {
    0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F,
    0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
};

//Pixel art information for LCD heart (hollow/damaged)
const uint8_t heart_hollow_top[13] = {
    0xF0, 0x0C, 0x02, 0x01, 0x01, 0x02, 0x0C,
    0x02, 0x01, 0x01, 0x02, 0x0C, 0xF0
};
const uint8_t heart_hollow_bot[13] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x10, 0x08, 0x04, 0x02, 0x01, 0x00
};

void lcd_draw_heart (uint8_t health, uint8_t buffer[]){
	for(uint8_t i = 0; i < 3; i++){

		int8_t container_fill = health - i - 1; //When value below zero hollow hearts should be printed

		for (int j = 0; j < 13; j++) {
			if (j >= 128) break; // Boundary check

	        //Find index for pages (lines on LCD) (1 * 128 is the width of a page)
	        int p0_idx = j + (i * 14);
	        int p1_idx = (1 * 128) + j + (i * 14);

	        if(container_fill < 0){
	        	//Boundery check into buffer update
	        	if (p0_idx < 512) buffer[p0_idx] = heart_hollow_top[j];
	        	if (p1_idx < 512) buffer[p1_idx] = heart_hollow_bot[j];
	        }else{
	        	//Boundery check into buffer update
	        	if (p0_idx < 512) buffer[p0_idx] = heart_filled_top[j];
	        	if (p1_idx < 512) buffer[p1_idx] = heart_filled_bot[j];
	        }
	    }
	}
	    lcd_push_buffer(buffer);
}




void lcd_draw_score(int32_t score, uint8_t buffer[]) {
    char score_str[20];
    // Format: Literal "Score: " followed by 8-digit padded integer
    sprintf(score_str, "Score: %08ld", score);

    int page3_offset = 256; // Page 2
    int x_start = 0;        //

    for (int i = 0; score_str[i] != '\0'; i++) {
        // Map ASCII to your character_data table offset
        uint8_t ascii_idx = (uint8_t)score_str[i] - 32;

        // Draw each column of the character
        for (int j = 0; j < 5; j++) {
            // (i * 6) provides a 1-pixel gap between characters
            int x_pos = x_start + (i * 6) + j;

            if (x_pos < 128) {
                int buffer_idx = page3_offset + x_pos;
                buffer[buffer_idx] = character_data[ascii_idx][j];
            }
        }
    }
    lcd_push_buffer(buffer);
}

void set_led(uint8_t input){
	if(input == 0){
		//reset pins to 0
		GPIOA->ODR &= ~(0x00000001 << (9));
		GPIOC->ODR &= ~(0x00000001 << (7));
		GPIOB->ODR &= ~(0x00000001 << (4));

		//Set to 1 to turn off LED
		GPIOA->ODR |= (0x00000001 << (9));
		GPIOC->ODR |= (0x00000001 << (7));
		GPIOB->ODR |= (0x00000001 << (4));
	}

    if (input == 0b00000001) GPIOA->ODR &= ~(0x00000001 << (9));; // Up joy = set PA9 (blue) = 0 ON
    if (input == 0b00000010) GPIOC->ODR &= ~(0x00000001 << (7)); // Up joy = set PC7 (green) = 0 ON
    if (input == 0b00000100) GPIOB->ODR &= ~(0x00000001 << (4)); // Up joy = set PB4 (red) = 0 ON

}

