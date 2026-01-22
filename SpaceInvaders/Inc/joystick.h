#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>
#include "30010_io.h"

/* Joystick bit masks */
#define JOY_UP     (1 << 0)
#define JOY_DOWN   (1 << 1)
#define JOY_LEFT   (1 << 2)
#define JOY_RIGHT  (1 << 3)
#define JOY_CENTER (1 << 4)

/* Basic GPIO setup for joystick and LEDs */
void GPIO_init(void);

/* Extended GPIO + ADC setup for 30010 board */
void GPIO_30010_init(void);

/* Read joystick using digital inputs */
uint8_t read_joystick(void);

/* Read joystick using ADC (30010 setup) */
uint8_t read_30010_joystick(void);

/* Control RGB LED based on input */
void set_led(uint8_t input);

/* Detect rising edge (just pressed) on a button */
uint8_t joystick_just_pressed(uint8_t input, uint8_t mask, uint8_t *prev);

/* Extract movement directions from raw input */
uint8_t joystick_get_move(uint8_t raw);

/* Detect just-pressed event for center button */
uint8_t joystick_center_just_pressed(uint8_t raw, uint8_t *prev);

/* Update movement and center press state */
void joystick_update(uint8_t *move,
                     uint8_t *center_just_pressed,
                     uint8_t *prev_center);

#endif /* JOYSTICK_H_ */
