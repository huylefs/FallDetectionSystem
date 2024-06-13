#include "MKL46Z4.h"
#include "uart.h"
#include <stdio.h>

void UART_Init() {

	SIM->SCGC4 |= 1 << 10; //UART0 Clock enable

	//MCGFLLCLK/2 clock source for UART0 
	SIM->SOPT2 |= 1 << 26;
	
	SIM->SCGC5 |= 1 << 9; //PORTA clock enable

	//PTA1 - UART0_RX, PTA2 - UART0_RX (Alternative Function 2)
	PORTA->PCR[1] |= 1 << 9; // MUX = 010
	PORTA->PCR[2] |= 1 << 9;

	//Disable UART0 transmitter and receiver while configuring
	UART0->C2 &= ~(1 << 3 | 1 << 2); //UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	//Configure for receiver and transmitter use 8 bit or 9 bit data characters
	UART0->C4 &= ~((uint8_t) 1 << 5);
	//Configure for receiver and transmitter use 8 bit data characters
	UART0->C1 &= ~((uint8_t) 1 << 4);
	//Configure for no parity
	UART0->C1 &= ~((uint8_t) 1 << 1);
	//Configure LSB (bit 0) is first bit transmitted following start bit
	UART0->S2 &= ~((uint8_t) 1 << 5);
	//Configure received data not inverted
	UART0->S2 &= ~((uint8_t) 1 << 4);
	//Configure transmitted data not invert
	UART0->C3 &= ~((uint8_t) 1 << 4);
	//Configure for 1 stop bit
	UART0->BDH &= ~((uint8_t) 1 << 5);
	//Set baud rate
	uint16_t sbr = SystemCoreClock / (16 * UART_BAUDRATE);
	UART0->BDH = (uint8_t)(sbr & 0x1F00); //Write 5 low bit of sbr to UARTx_BDL[4:0] 
	UART0->BDL = ((uint8_t)(sbr & 0xFF)); //Write 8 high bit of sbr to UARTx_BDH[7:0] 

	//Enable UART0 transmitter and receiver
	UART0->C2 |= (1 << 3 | 1 << 2);
}

void UART_SendChar(char data) {
	while(!(UART0->S1 & (1 << 7))); // Wait transmit data buffer empty
	UART0->D = data;
}

 void UART_SendString(const char *str) {
 	while (*str) {
         UART_SendChar((uint8_t)*str++);
     }
}

void UART_SendFloat(float value) {
    char buffer[20];
    sprintf(buffer, "%.3f", value);
    UART_SendString(buffer);
}
