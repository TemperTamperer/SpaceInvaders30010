#include "joystick.h"

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

uint16_t adc_read(uint8_t channel) {
    // Clearing 0x00000FFF sets length to 1 and clears the first channel slot.
    ADC1->SQR1 &= ~0x00000FFF;

    // Map the channel to the first slot (Bits 10:6)
    ADC1->SQR1 |= (channel << 6);

    // Start the conversion
    ADC1->CR |= (1 << 2); // ADSTART

    // Wait for End of Conversion (EOC)
    while(!(ADC1->ISR & (1 << 2)));

    // Read the result
    uint16_t result = (uint16_t)ADC1->DR;

    // Clear the EOC flag (Write 1 to clear)
    ADC1->ISR |= (1 << 2);

    return result;
}

void GPIO_init(void)
{
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

    GPIOA->MODER &= ~(0x00000003 << (0 * 2));
    GPIOA->MODER |=  (0x00000000 << (0 * 2));
    GPIOA->PUPDR &= ~(0x00000003 << (0 * 2));
    GPIOA->PUPDR |=  (0x00000002 << (0 * 2));

    GPIOB->MODER &= ~(0x00000003 << (0 * 2));
    GPIOB->MODER |=  (0x00000000 << (0 * 2));
    GPIOB->PUPDR &= ~(0x00000003 << (0 * 2));
    GPIOB->PUPDR |=  (0x00000002 << (0 * 2));

    GPIOC->MODER &= ~(0x00000003 << (0 * 2));
    GPIOC->MODER |=  (0x00000000 << (0 * 2));
    GPIOC->PUPDR &= ~(0x00000003 << (0 * 2));
    GPIOC->PUPDR |=  (0x00000002 << (0 * 2));

    GPIOA->MODER &= ~(0x00000003 << (9 * 2));
    GPIOA->MODER |=  (0x00000001 << (9 * 2));

    GPIOC->MODER &= ~(0x00000003 << (7 * 2));
    GPIOC->MODER |=  (0x00000001 << (7 * 2));

    GPIOB->MODER &= ~(0x00000003 << (4 * 2));
    GPIOB->MODER |=  (0x00000001 << (4 * 2));

	//LED

	//Set PA9 (red) to read
	GPIOA->MODER &= ~(0x00000003 << (9 * 2)); // Clear mode register
	GPIOA->MODER |= (0x00000001 << (9 * 2)); // Set mode register (0x00 –Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)

	//Set PC7 (green) to read
	GPIOC->MODER &= ~(0x00000003 << (7 * 2));
	GPIOC->MODER |= (0x00000001 << (7 * 2));

	//Set PB4 (blue) to read
	GPIOB->MODER &= ~(0x00000003 << (4 * 2));
	GPIOB->MODER |= (0x00000001 << (4 * 2));
}

void GPIO_30010_init(void) {
    // Enable Clocks for Ports A, B, C and ADC
    RCC->AHBENR |= (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_ADC12);

    // ADC Clock Configuration
    RCC->CFGR2 &= ~0x1F;
    RCC->CFGR2 |= 0x10;
    ADC1_2->CCR &= ~(0x3 << 16);
    ADC1_2->CCR |= (0x1 << 16);

    // LED Configuration (Outputs)
    GPIOA->MODER &= ~(0x3 << (9 * 2)); // Red PA9
    GPIOA->MODER |=  (0x1 << (9 * 2));
    GPIOC->MODER &= ~(0x3 << (7 * 2)); // Green PC7
    GPIOC->MODER |=  (0x1 << (7 * 2));
    GPIOB->MODER &= ~(0x3 << (4 * 2)); // Blue PB4
    GPIOB->MODER |=  (0x1 << (4 * 2));

    // Joystick X/Y (Analog PA0, PA1)
    GPIOA->MODER |= (0xF << (0 * 2));

    // BUTTONS CONFIGURATION
    // Setup PC0 (Shoot) 
    GPIOC->MODER &= ~(0x3 << (0 * 2)); // Mode: Input
    GPIOC->PUPDR &= ~(0x3 << (0 * 2)); // Clear
    GPIOC->PUPDR |=  (0x2 << (0 * 2)); // Set to 0x2 for Pull-down


    // ADC Startup
    ADC1->CR &= ~(0x3 << 28);
    ADC1->CR |= (0x1 << 28);
    for(volatile int i = 0; i < 10000; i++);
    ADC1->CR |= (1U << 31);
    while(ADC1->CR & (1U << 31));
    ADC1->CR |= (1 << 0);
    while(!(ADC1->ISR & (1 << 0)));
}

uint8_t read_joystick(void)
{
    if (GPIOA->IDR & (1 << 4)) return JOY_UP;
    if (GPIOB->IDR & (1 << 0)) return JOY_DOWN;
    if (GPIOC->IDR & (1 << 1)) return JOY_LEFT;
    if (GPIOC->IDR & (1 << 0)) return JOY_RIGHT;
    if (GPIOB->IDR & (1 << 5)) return JOY_CENTER;
    return 0;
}

uint8_t read_30010_joystick(void) {
    uint8_t res = 0;

    // Read ONLY the X-axis (PA0) for Left/Right
    uint16_t x_val = adc_read(1);
    if (x_val < 2600) res |= JOY_LEFT;
    if (x_val > 3300) res |= JOY_RIGHT;

    // SHOOT BUTTON (Connected to C0)
    // We map C0 to JOY_CENTER
    if ((GPIOC->IDR & (1 << 0))) {
        res |= JOY_CENTER;
    }


    return res;
}

