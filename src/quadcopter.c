#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h> 
#include <stdio.h> 

#define PIN_IMU_SDA 0
#define PIN_IMU_SCL 1

#define GYRO_ADDR  0xD0 // Write address.
#define GYRO_REG_X 0x1D // X register.
#define GYRO_REG_Y 0x1F // Y register.
#define GYRO_REG_Z 0x21 // Z register.

#define ACCL_ADDR  0xA6
#define ACCL_REG_X 0x32 // X register.
#define ACCL_REG_Y 0x34 // Y register.
#define ACCL_REG_Z 0x36 // Z register.

int readFromRegister(i2c*, int, int);
void writeToRegister(i2c*, int, int, int);
int readValue(i2c*, int, int);
int combine(int, int);

int main()
{
  i2c imu;

  // Wait for output stream to open.
  waitcnt(CNT + CLKFREQ);
  
  i2c_open(&imu, PIN_IMU_SCL, PIN_IMU_SDA, 0);
  
  // Gyro initialization.
  // 22 -> 11011  Set internal clock and scale.
  // 21 -> 9      Set sample rate.
  // 23 -> 101    Trigger interrupt when new data is ready (extra int pins).
  // 62 -> 1      Set clock source.
  writeToRegister(&imu, GYRO_ADDR, 0x16, 0x1A);
  writeToRegister(&imu, GYRO_ADDR, 0x15, 0x09);
  //writeToRegister(&imu, GYRO_ADDR, 0x17, 0x05);
  writeToRegister(&imu, GYRO_ADDR, 0x3E, 1);
  
  // Accel initialization.
  // 45 -> 1000  Set the mode to MEASURE mode.
  // 49 -> 01    Set the data range. 00->2, 01->4, 10->8, 11->16 (+- g).;
  writeToRegister(&imu, ACCL_ADDR, 0x2D, 0x10);
  writeToRegister(&imu, ACCL_ADDR, 0x31, 1);

  while(1)
  {
    waitcnt(CNT + CLKFREQ/10);
    
    float gx = readValue(&imu, GYRO_ADDR, GYRO_REG_X)/14.375;
    float gy = readValue(&imu, GYRO_ADDR, GYRO_REG_Y)/14.375;
    float gz = readValue(&imu, GYRO_ADDR, GYRO_REG_Z)/14.375;

    int ax = readValue(&imu, ACCL_ADDR, ACCL_REG_X);
    int ay = readValue(&imu, ACCL_ADDR, ACCL_REG_Y);
    int az = readValue(&imu, ACCL_ADDR, ACCL_REG_Z);

    printf("G: %6d %6d %6d\n", gx, gy, gz);
    //printf("A: %6d %6d %6d\n", ax, ay, az);
  }
}

int readValue(i2c* bus, int address, int regAddr)
{
  i2c_start(bus);
  i2c_writeByte(bus, address);
  i2c_writeByte(bus, regAddr);
  i2c_start(bus);
  i2c_writeByte(bus, address+1);

  int b1 = i2c_readByte(bus, 0);
  int b2 = i2c_readByte(bus, 1);

  int val = combine(b1, b2);

  i2c_stop(bus);

  return val;
}

int readFromRegister(i2c* bus, int address, int regAddr)
{
  i2c_start(bus);
  i2c_writeByte(bus, address);
  i2c_writeByte(bus, regAddr);
  i2c_start(bus);
  i2c_writeByte(bus, address+1);

  int val = i2c_readByte(bus, 1);

  i2c_stop(bus);

  return val;
}

int combine(int h, int l) {
  int ret = 0;
  ret <<= 8;
  ret |= (int)h & 0xFF;
  ret <<= 8;
  ret |= (int)l & 0xFF;
  return ret;
}

void writeToRegister(i2c* bus, int address, int regAddr, int val)
{
  i2c_start(bus);
  i2c_writeByte(bus, address+1);
  i2c_writeByte(bus, regAddr);
  i2c_writeByte(bus, val);
  i2c_stop(bus);
}