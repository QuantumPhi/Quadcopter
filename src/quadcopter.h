#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

#define PIN_IMU_SDA  0
#define PIN_IMU_SCL  1
#define PIN_MOTOR    4
#define PIN_XBEE_IN  30
#define PIN_XBEE_OUT 29

#define GYRO_ADDR  0xD0
#define GYRO_REG_X 0x1D
#define GYRO_REG_Y 0x1F
#define GYRO_REG_Z 0x21

#define ACCL_ADDR  0xA6
#define ACCL_REG_X 0x32
#define ACCL_REG_Y 0x34
#define ACCL_REG_Z 0x36

#define XBEE_BAUD_RATE 9600

#define MOTOR_START 1000
#define MOTOR_LOW   1100
#define MOTOR_HIGH  2000

#define COMMAND_WAKEUP   0x10 // 1
#define COMMAND_FORWARD  0x20 // P
#define COMMAND_BACKWARD 0x30 // P
#define COMMAND_HOVER    0x40 // 1
#define COMMAND_DOWN     0x50 // P
#define COMMAND_UP       0x60 // P
#define COMMAND_CCW      0x70 // P
#define COMMAND_CW       0x80 // P
#define COMMAND_RIGHT    0x90 // P
#define COMMAND_LEFT     0xA0 // P
#define COMMAND_SHUTDOWN 0xB0 // 1
#define COMMAND_REQUEST  0xC0 // 1
#define COMMAND_TAKEOFF  0xD0 // A
#define COMMAND_LAND     0xE0 // A

#define PARAM_ONCE  0x00
#define PARAM_POWER 0x01
#define PARAM_AUTON 0x02

unsigned short readFromRegister(i2c*, int, int);
void writeToRegister(i2c*, int, int, int);
unsigned short readValue(i2c*, int, int, int);
unsigned short combine(char, char);

void imuInit();
void imuUpdate();
int computePID(int, int);
void imuRun();
void getImuData(signed short*, signed short*, signed short*, signed short*, signed short*, signed short*);

void xbeeInit();
void xbeeRun();
unsigned int xbeeGetByte();
void xbeeSendByte(unsigned int);

void motorRun();
void motorGo(double);
void motorWakeup();
void motorStop();