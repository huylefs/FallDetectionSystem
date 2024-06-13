#include "MKL46Z4.h"
#include "I2C.h"
#include "MMA8451.h"
#include "delay.h"
#include <math.h>

//MMA8451Q address: 0x1D

void MMA8451_Init(void) {
    I2C_WriteRegister(MMA8451Q_address, 0x2A, 0x1);
}

Data MMA8451_Read(void) {
    Data data;
	
    uint8_t X_MSB = I2C_ReadRegister(MMA8451Q_address, OUT_X_MSB);
		delay(1);
    uint8_t X_LSB = I2C_ReadRegister(MMA8451Q_address, OUT_X_LSB);
		delay(1);
    uint8_t Y_MSB = I2C_ReadRegister(MMA8451Q_address, OUT_Y_MSB);
		delay(1);
    uint8_t Y_LSB = I2C_ReadRegister(MMA8451Q_address, OUT_Y_LSB);
		delay(1);
    uint8_t Z_MSB = I2C_ReadRegister(MMA8451Q_address, OUT_Z_MSB);
		delay(1);
    uint8_t Z_LSB = I2C_ReadRegister(MMA8451Q_address, OUT_Z_LSB);
    delay(1);
	
    int16_t X = ((int16_t)(X_MSB << 8 | X_LSB)) >> 2;
    int16_t Y = ((int16_t)(Y_MSB << 8 | Y_LSB)) >> 2;
    int16_t Z = ((int16_t)(Z_MSB << 8 | Z_LSB)) >> 2;
	
		data.x = (float)X / 8192 * 2;
    data.y = (float)Y / 8192 * 2;
		data.z = (float)Z / 8192 * 2;
	
    return data;
}

float SVM(Data data) {
	return sqrt(data.x * data.x + data.y * data.y + data.z * data.z);
}
