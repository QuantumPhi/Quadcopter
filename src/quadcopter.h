#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

#define PIN_IMU_SDA 0
#define PIN_IMU_SCL 1

#define GYRO_ADDR  0xD0
#define GYRO_REG_X 0x1D
#define GYRO_REG_Y 0x1F
#define GYRO_REG_Z 0x21

#define ACCL_ADDR  0xA6
#define ACCL_REG_X 0x32
#define ACCL_REG_Y 0x34
#define ACCL_REG_Z 0x36

#define PIN_XBEE_IN  30
#define PIN_XBEE_OUT 29

unsigned short readFromRegister(i2c*, int, int);
void writeToRegister(i2c*, int, int, int);
unsigned short readValue(i2c*, int, int, int);
unsigned short combine(char, char);

void imuInit();
void imuUpdate();
int computePID(int, int);
void imuRun();
void getImuData(signed short*, signed short*, signed short*, signed short*, signed short*, signed short*);

void xbeeRun();
unsigned int xbeeGetByte();
void xbeeSendByte(int);

void motorRun(int, double);