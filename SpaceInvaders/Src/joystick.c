#include "joystick.h"

// Input helpers
uint8_t joystick_just_pressed(uint8_t input, uint8_t mask, uint8_t *prev)
{
    uint8_t now = (input & mask) ? 1 : 0;
    uint8_t jp = (now && !(*prev)) ? 1 : 0;
    *prev = now;
    return jp;
}

uint8_t joystick_get_move(uint8_t raw)
{
    return raw & (JOY_UP | JOY_DOWN | JOY_LEFT | JOY_RIGHT);
}

uint8_t joystick_center_just_pressed(uint8_t raw, uint8_t *prev)
{
    return joystick_just_pressed(raw, JOY_CENTER, prev);
}

void joystick_update(uint8_t *move,
                     uint8_t *center_just_pressed,
                     uint8_t *prev_center)
{
    uint8_t raw = read_joystick();
    *move = joystick_get_move(raw);
    *center_just_pressed = joystick_center_just_pressed(raw, prev_center);
}

// GPIO setup
#define GPIO_MODE_BITS   2
#define GPIO_MODE_MASK   0x00000003
#define GPIO_INPUT       0x00000000
#define GPIO_OUTPUT      0x00000001
#define GPIO_PULLDOWN    0x00000002

#define PIN0  0
#define PIN4  4
#define PIN5  5
#define PIN7  7
#define PIN9  9
#define PIN1  1

void GPIO_init(void)
{
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

    GPIOA->MODER &= ~(GPIO_MODE_MASK << (PIN0 * GPIO_MODE_BITS));
    GPIOA->MODER |=  (GPIO_INPUT     << (PIN0 * GPIO_MODE_BITS));
    GPIOA->PUPDR &= ~(GPIO_MODE_MASK << (PIN0 * GPIO_MODE_BITS));
    GPIOA->PUPDR |=  (GPIO_PULLDOWN  << (PIN0 * GPIO_MODE_BITS));

    GPIOB->MODER &= ~(GPIO_MODE_MASK << (PIN0 * GPIO_MODE_BITS));
    GPIOB->MODER |=  (GPIO_INPUT     << (PIN0 * GPIO_MODE_BITS));
    GPIOB->PUPDR &= ~(GPIO_MODE_MASK << (PIN0 * GPIO_MODE_BITS));
    GPIOB->PUPDR |=  (GPIO_PULLDOWN  << (PIN0 * GPIO_MODE_BITS));

    GPIOC->MODER &= ~(GPIO_MODE_MASK << (PIN0 * GPIO_MODE_BITS));
    GPIOC->MODER |=  (GPIO_INPUT     << (PIN0 * GPIO_MODE_BITS));
    GPIOC->PUPDR &= ~(GPIO_MODE_MASK << (PIN0 * GPIO_MODE_BITS));
    GPIOC->PUPDR |=  (GPIO_PULLDOWN  << (PIN0 * GPIO_MODE_BITS));

    // LED outputs
    GPIOA->MODER &= ~(GPIO_MODE_MASK << (PIN9 * GPIO_MODE_BITS));
    GPIOA->MODER |=  (GPIO_OUTPUT    << (PIN9 * GPIO_MODE_BITS));

    GPIOC->MODER &= ~(GPIO_MODE_MASK << (PIN7 * GPIO_MODE_BITS));
    GPIOC->MODER |=  (GPIO_OUTPUT    << (PIN7 * GPIO_MODE_BITS));

    GPIOB->MODER &= ~(GPIO_MODE_MASK << (PIN4 * GPIO_MODE_BITS));
    GPIOB->MODER |=  (GPIO_OUTPUT    << (PIN4 * GPIO_MODE_BITS));
}

// Read joystick (one direction at a time)
uint8_t read_joystick(void)
{
    if (GPIOA->IDR & (1 << PIN4)) return JOY_UP;
    if (GPIOB->IDR & (1 << PIN0)) return JOY_DOWN;
    if (GPIOC->IDR & (1 << PIN1)) return JOY_LEFT;
    if (GPIOC->IDR & (1 << PIN0)) return JOY_RIGHT;
    if (GPIOB->IDR & (1 << PIN5)) return JOY_CENTER;
    return 0;
}
