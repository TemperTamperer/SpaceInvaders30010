#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "30010_io.h"

void GPIO_init();
uint8_t read_joystick();
void set_led(uint8_t input);


#endif /* JOYSTICK_H_ */
