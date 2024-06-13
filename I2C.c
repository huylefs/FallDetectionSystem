#include "MKL46Z4.h"

void I2C_Init(void) {
    SIM->CLKDIV1 |= (1u<<17) | (1u<<16); //bus clock is 24/3 = 8MHz

    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //clock to PTE24 and PTE25 for I2C0
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; //clock to I2C0
    
    PORTE->PCR[24] = PORT_PCR_MUX(5); 
    PORTE->PCR[25] = PORT_PCR_MUX(5);
    
    I2C0->F = 0x80; //mult=2h ICR=00h
    
    I2C0->C1 = 0xB0; //10110000 - module enable, interrupt disable, master, transmit,

    //acknowledge bit sent,repeated start off, wake up off, DMA off
    I2C0->C2 = 0x00;
}

//Wait for the I2C interrupt flag to set
void I2C_Wait(void) {
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;
}

//Send start signal
void I2C_Start(void) {
	// set to transmit mode , send start signal by set to master.
    I2C0->C1 |= I2C_C1_TX_MASK;
    I2C0->C1 |= I2C_C1_MST_MASK;
}

//Send stop signal
void I2C_Stop(void) {
	// send stop signal by set to slave
    I2C0->C1 &= ~I2C_C1_MST_MASK;
	// set to receive mode
    I2C0->C1 &= ~I2C_C1_TX_MASK;
}

//Send repeated start signal
void I2C_RepeatedStart(void) {
    I2C0->C1 |= I2C_C1_RSTA_MASK;
}

//Send address
void I2C_SendAddress(uint8_t address) {
    I2C0->D = address;
    I2C_Wait();
}

//Send data
void I2C_SendData(uint8_t data) {
    I2C0->D = data;
    I2C_Wait();
}


void I2C_WriteRegister(uint8_t device_address, uint8_t register_address, uint8_t data) {
    I2C_Start();
    I2C_SendAddress(device_address << 1);  // Send device address with write bit
    I2C_SendAddress(register_address);        // Send register address
    I2C_SendData(data);                    // Send data
    I2C_Stop();
}

uint8_t I2C_ReadRegister(uint8_t device_address, uint8_t register_address) {
    uint8_t result;
	
		//start 
    I2C_Start();
	
		// Send device address with write bit
    I2C_SendAddress(device_address << 1);
		
		// send register address
		I2C_SendAddress(register_address);
	
		//Repeated start to change to read mode
    I2C_RepeatedStart();
	
		// Send device address with write bit
     I2C_SendAddress((device_address << 1) | 1);
	
		//Recieve mode
    I2C0->C1 &= ~I2C_C1_TX_MASK;
		
		//Send NACK
    I2C0->C1 |= I2C_C1_TXAK_MASK;
	
		//Fake read
    result = I2C0->D;
		I2C_Wait();

    //Read data
    result = I2C0->D;
		I2C_Wait();
		
		//Send stop signal
    I2C_Stop();
    
    return result;
}


