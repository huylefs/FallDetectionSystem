#include "MKL46Z4.h"
#include "I2C.h"
#include "MMA8451.h"
#include "delay.h"
#include "SLCD.h"
#define threshold 3
	
volatile int fall_detected;

volatile uint8_t state;
void Switch_Init(void);

void LED_Init(void);
void RED_LED_Reset(void);
void GREEN_LED_Reset(void);

int main(void) {
	LED_Init();
	Switch_Init();
	Systick_Init();
  I2C_Init();
  MMA8451_Init();
	delay(1);
	SLCD_Init();
	SLCD_EnablePins();
	SLCD_WriteChar('0');
	fall_detected = 0;
	
    while (1) {
		if (state ==0) {
			GREEN_LED_Reset();
			Systick_Shutdown();
			PTE->PSOR |=1<<29;
			
		} else {
			Systick_Init();
			Data data = MMA8451_Read();
			float accMag = SVM(data);
			
			if (accMag > threshold) {
				fall_detected = 1; 
        SLCD_WriteChar('1'); // Hi?n th? tr?ng thái ngã
      }
			delay(10);

			
		}
    }
}
//Switch
void Switch_Init(void) {
	state = 0;
	//PTC3 - SW1
	//PTC12 - SW2
	
	//Eable clock portC
	SIM->SCGC5 |= (1<<11);
	
	// SW1
	// Configure GPIO
	PORTC->PCR[3] |= (1<<8);
	//PE: Internal pullup or pulldown resistor is enabled
	PORTC->PCR[3] |= (1<<1); 
	//PS: Internal pullup resistor is enabled
	PORTC->PCR[3] |= (1<<0); 
	//IRCQ = 1010: Interupt on falling edge
	PORTC->PCR[3] |= (1<<17) | (1<<19); 
	//Set input mode
	PTC->PDDR &= (~(uint32_t)(1<<3)); 

	
	//SW2
  // Configure GPIO
	PORTC->PCR[12] |= (1<<8);
	//PE: Internal pullup or pulldown resistor is enabled
	PORTC->PCR[12] |= (1<<1);
	//PS: Internal pullup resistor is enabled
	PORTC->PCR[12] |= (1<<0);
	//IRCQ = 1010: Interupt on falling edge
	PORTC->PCR[12] |= (1<<17) | (1<<19);
	//Set input mode
	PTC->PDDR &= (~(uint32_t)(1<<12));
	
	
	
	//PORTC_PORTD_IRQn 31
	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}

void PORTC_PORTD_IRQHandler(void) {
	//Pressed SW1 
	if ((PTC->PDIR & (1<<3)) == 0) { 
		if (state == 0) {
			state = 1;
		}
		else if (state == 1) {
			state = 0;
		}
	}

	//ISF: Clear interrupt flag
	PORTC->PCR[3] |= (1 << 24);
	
	//Pressed SW2
	if ((PTC->PDIR & (1<<12)) == 0) {  
		state = 0;
		NVIC_SystemReset();
	}
	
	//ISF: Clear interrupt flag
	PORTC->PCR[12] |= (1<<24);
}

//Led
void LED_Init(void) {
	//Clock port D (Led green)
	SIM->SCGC5 |= (1<<12);
	//Clock port E (Led red)
	SIM->SCGC5 |= (1<<13);
	
	//GPIO alternative function
	PORTD->PCR[5] |= (1<<8);
	PORTE->PCR[29] |= (1<<8);
	
	//Set Output mode 
	// green led PTD[5]
	// red led PTE[29]
	PTD->PDDR |= 1<<5;
	PTE->PDDR |= 1<<29;
	
	// turn off led red
	PTE->PSOR |= 1<<29;
}

void GREEN_LED_Reset(void) {
	// turn off led green
	PTD->PSOR |= 1 << 5;
}

void RED_LED_Reset(void) {
	// turn off led red
	PTE->PSOR |= 1<<29;
}
