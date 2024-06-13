#include "MKL46Z4.h"

extern volatile uint32_t msTick;
extern volatile uint32_t LED_tick_green;
extern volatile uint32_t LED_tick_red;
extern volatile int fall_detected;

void Systick_Init(void);
void Systick_Shutdown(void);

void delay(uint32_t TICK);
