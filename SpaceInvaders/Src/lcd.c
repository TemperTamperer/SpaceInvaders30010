#include "lcd.h"
#include "timer.h"


void lcd_write_string(uint8_t string[], int32_t line){
	uint8_t buffer[512];
	memset(buffer,0x00,512);
	uint8_t text_width = 3 * 5; //3 letters 5 wide
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 5; j++){
			uint8_t written_width = j + i * 5; //width of the characters that has already been written before this step
			if(line - written_width < 0){
				continue;
			}
			buffer[line + written_width] = character_data[string[i]][j];
			if((line + written_width) > 128){
				buffer[line + written_width] = 0x00;
			}
		}
	}

	lcd_push_buffer(buffer);

}

void lcd_update() {
    if (timer_flag == 1) {

        timer_flag = 0;

        char display_str[4];

        display_str[0] = '0' + (system_time.seconds / 10) - 32; //-32 magic number fixes ASCII offset
        display_str[1] = '0' + (system_time.seconds % 10) - 32;
        display_str[2] = '!' - 32;
        display_str[3] = '\0';


        lcd_write_string((uint8_t*)display_str, 0);
    }
}

void lcd_scroll(uint8_t string[], int32_t *line){
			(*line)--;
			uint8_t text_width = 3 * 5; //3 letters 5 wide
			if((*line) < 0) (*line) = 128;
			lcd_write_string(string,*line);
}
