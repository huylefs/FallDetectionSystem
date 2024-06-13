#include "MKL46Z4.h"
#ifndef I2C_H
#define I2C_H

void I2C_Wait(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_RepeatedStart(void);
void I2C_SendAddress(uint8_t address);

void I2C_SendData(uint8_t data);

void I2C_WriteRegister(uint8_t device_address, uint8_t register_address, uint8_t data) ;
uint8_t I2C_ReadRegister(uint8_t device_address, uint8_t register_address);
void I2C_Init(void);

#endif