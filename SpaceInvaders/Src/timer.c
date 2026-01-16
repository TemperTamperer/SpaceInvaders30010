#include "timer.h"

volatile time_tracker system_time = {0, 0, 0, 0};
volatile uint8_t timer_flag;

void timer15_init(){
	RCC->APB2ENR |= RCC_APB2Periph_TIM15; // Enable clock line to timer 15;
	TIM15->CR1 = 0x0000; // Configure timer 15
	//Interrupt happens every 50 miliseconds
	TIM15->ARR = 0xFA00; // Set reload value 64000 decimal
	TIM15->PSC = 0x0031; // Set prescale value 49 decimal

	// --- ADD THIS LINE ---
	TIM15->EGR |= 0x0001; // Force an update to load PSC and ARR values
	TIM15->SR  &= ~0x0001; // The line above triggers a 'fake' interrupt flag; clear it!

	// Enable interrupt in timer and in NVIC
	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts
	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0x0); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
	TIM15->CR1 = 0x0001; // turn on timer
}



void TIM1_BRK_TIM15_IRQHandler(void) {
// 3. Update the values
	timer_flag = 1;

	system_time.hundredths++;

if (system_time.hundredths >= 100) {
	system_time.hundredths = 0;
	system_time.seconds++;

	if (system_time.seconds >= 60) {
		system_time.seconds = 0;
		system_time.minutes++;

		if (system_time.minutes >= 60) {
			system_time.minutes = 0;
			system_time.hours++;

			if (system_time.hours >= 24) {
				system_time.hours = 0; // Reset at midnight
			}
		}
	}
}
TIM15->SR &= ~0x0001; // Clear interrupt bit
}
