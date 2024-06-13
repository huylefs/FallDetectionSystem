#include "MKL46Z4.h"
#include "delay.h"

volatile uint32_t msTicks;
volatile uint32_t LED_tick_green;
volatile uint32_t LED_tick_red;


void Systick_Init(void) {
	SysTick->CTRL |= (1 << 0);
	SysTick->CTRL |= (1 << 1);
	SysTick->CTRL |= (1 << 2);
	SysTick->LOAD = SystemCoreClock/1000;
	
	NVIC_SetPriority(-1, 15);
}

void Systick_Shutdown(void) {
	SysTick->CTRL &= ~((uint32_t)(1 << 0));
}

void SysTick_Handler(void) {
	msTicks++;
	LED_tick_green++;
	
	// 1Hz => 1s between 2 time led green on =>500ms change from turn on to off
	if (LED_tick_green == 500) { //500ms
		PTD->PTOR |= 1 <<5;//Toggle LED xanh
		LED_tick_green = 0;
	}
	if(fall_detected){
		LED_tick_red++;
	}
	// 2Hz => 0.5s between 2 time led green on =>250ms change from turn on to off
	if (fall_detected && LED_tick_red == 250) { // 250ms
		PTE->PTOR |= 1 << 29; // Toggle LED do
		LED_tick_red = 0;
	}
}

void delay(uint32_t TICK) {
	while (msTicks < TICK);
	msTicks = 0;
}
