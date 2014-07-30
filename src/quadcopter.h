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

#define COMMAND_WAKEUP   0x01
#define COMMAND_FORWARD  0x02
#define COMMAND_BACKWARD 0x03
#define COMMAND_HOVER    0x04
#define COMMAND_DOWN     0x05
#define COMMAND_UP       0x06
#define COMMAND_CCW      0x07
#define COMMAND_CW       0x08
#define COMMAND_RIGHT    0x09
#define COMMAND_LEFT     0x0A
#define COMMAND_SHUTDOWN 0x0B
#define COMMAND_REQUEST  0x0C
#define COMMAND_TAKEOFF  0x0D
#define COMMAND_LAND     0x0E

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