#include "joystick.h"

void GPIO_init(){
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A
	// Set pin PA0 to input

	GPIOA->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
	GPIOA->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 –Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOA->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
	GPIOA->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 -No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port B


	GPIOB->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 –Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOB->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
	GPIOB->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 -No pull, 0x01 - Pull-up, 0x02 - Pull-down)

	RCC->AHBENR |= RCC_AHBPeriph_GPIOC; // Enable clock for GPIO Port C


	GPIOC->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 –Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
	GPIOC->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 -No pull, 0x01 - Pull-up, 0x02 - Pull-down)

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

uint8_t read_joystick() {
    // Check if the specific bit is NOT zero
    if (GPIOA->IDR & (1 << 4)) return 0b00000001; // Up    (PA4)
    if (GPIOB->IDR & (1 << 0)) return 0b00000010; // Down  (PB0)
    if (GPIOC->IDR & (1 << 1)) return 0b00000100; // Left  (PC1)
    if (GPIOC->IDR & (1 << 0)) return 0b00001000; // Right (PC0)
    if (GPIOB->IDR & (1 << 5)) return 0b00010000; // Center(PB5)

    return 0; // Nothing pressed
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
