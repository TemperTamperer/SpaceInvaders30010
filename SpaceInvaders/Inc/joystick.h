#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "30010_io.h"
#include "joystick.h"

#define JOY_UP     (1 << 0)
#define JOY_DOWN   (1 << 1)
#define JOY_LEFT   (1 << 2)
#define JOY_RIGHT  (1 << 3)
#define JOY_FIRE (1 << 4)


uint8_t read_joystick(void);
uint8_t joystick_just_pressed(uint8_t input, uint8_t mask, uint8_t *prev);

void GPIO_init();
uint8_t read_joystick();
void set_led(uint8_t input);


#endif /* JOYSTICK_H_ */
