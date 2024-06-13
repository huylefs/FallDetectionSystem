#include "MKL46Z4.h"

#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06
#define WHO_AM_I  0x0D 
#define MMA8451Q_address 0x1D

typedef struct {
    float x;
    float y;
    float z;
} Data;

void MMA8451_Init(void);
Data MMA8451_Read(void);
float SVM(Data data);
