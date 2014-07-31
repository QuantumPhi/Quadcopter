#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

#define PIN_IMU_SDA    0
#define PIN_IMU_SCL    1
#define PIN_MOTOR      4
#define PIN_XBEE_IN    30
#define PIN_XBEE_OUT   29
#define PIN_RANGE_TRIG 14
#define PIN_RANGE_ECHO 15

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

volatile unsigned int lastCommand;
volatile unsigned int distanceToGround;

unsigned short read_from_register(i2c*, int, int);
void write_to_register(i2c*, int, int, int);
unsigned short read_value(i2c*, int, int, int);
unsigned short combine(char, char);

void imu_init();
void imu_update();
int compute_pid(int, int);
void imu_run();
void get_imu_data(signed short*, signed short*, signed short*, signed short*, signed short*, signed short*);

void xbee_init();
void xbee_run();
unsigned int xbee_get_byte();
void xbee_send_byte(unsigned int);

void motor_init();
void motor_run();
void motor_go(double);
void motor_wakeup();
void motor_stop();

typedef struct
{
  int raw;
  int errSum;
  int lastErr;
  int output;
} axis;

typedef struct
{
  int pin;
  int current_val;
} motor;