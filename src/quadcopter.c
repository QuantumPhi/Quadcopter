#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h> 
#include <stdio.h> 

#define PIN_IMU_SCL 1
#define PIN_IMU_SDA 0

#define GYRO_WRITE 0xD0
#define GYRO_READ  0xD1
#define GYRO_REGISTER 0x1E

#define ACCEL_WRITE 0xA6
#define ACCEL_READ  0xA7
#define ACCEL_REGISTER 0

int main()
{
  i2c* imu;
  
  i2c_open(imu, PIN_IMU_SCL, PIN_IMU_SDA, 0);
  
  while(1)
  {
    printf("%d\n", CNT);
    pause(100);
    //waitcnt(CNT + CLKFREQ);
    
    i2c_start(imu);
    i2c_writeByte(imu, GYRO_WRITE);
    i2c_writeByte(imu, GYRO_REGISTER);
    i2c_start(imu);
    i2c_writeByte(imu, GYRO_READ);
    int read = i2c_readByte(imu, 0);
    i2c_stop(imu);

    //printf("%d\n", read);
  }
}

/*
1. Send a start sequence
2. Send 0xC0 ( I2C address of the CMPS03 with the R/W bit low (even address)
3. Send 0x01 (Internal address of the bearing register)
4. Send a start sequence again (repeated start)
5. Send 0xC1 ( I2C address of the CMPS03 with the R/W bit high (odd address)
6. Read data byte from CMPS03
7. Send the stop sequence.
*/