#include "timer.h"

volatile time_tracker system_time = {0, 0, 0, 0};
volatile uint8_t timer_flag;

void timer_wait_for_tick(void)
{
    while (!timer_flag) { }
    timer_flag = 0;
}

// TIM15 tick at 20 Hz (50 ms)
void timer15_init()
{
	RCC->APB2ENR |= RCC_APB2Periph_TIM15; // enable clock for TIM15
	TIM15->CR1 = 0x0000; // reset control register

	// interrupt every 50 ms
	TIM15->ARR = 0xFA00; // reload value
	TIM15->PSC = 0x0031; // prescaler

	// load ARR/PSC immediately
	TIM15->EGR |= 0x0001;
	TIM15->SR  &= ~0x0001;

	// enable interrupt in TIM15 + NVIC
	TIM15->DIER |= 0x0001;
	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0x0);
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);

	TIM15->CR1 = 0x0001; // start timer
}

void TIM1_BRK_TIM15_IRQHandler(void)
{
	timer_flag = 1;

	// timekeeping (hundredths)
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
					system_time.hours = 0;
				}
			}
		}
	}

	TIM15->SR &= ~0x0001; // clear interrupt flag
}
