#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h> 
#include <stdio.h> 

#define DLPF_FS     0x16
#define DLPF_CFG_0  (1<<0)
#define DLPF_CFG_1  (1<<1)
#define DLPF_CFG_2  (1<<2)
#define DLPF_FS_SEL_0   (1<<3)
#define DLPF_FS_SEL_1   (1<<4)

#define SMPLRT_DIV  0x15

#define INT_CFG_ITG_RDY_EN  (1<<2)
#define INT_CFG_RAW_RDY_EN  (1<<0)
#define INT_CFG     0x17

#define PWR_MGM     0x3E
#define PWR_MGM_CLK_SEL_0   (1<<0)

#define PIN_IMU_SDA 0
#define PIN_IMU_SCL 1

#define GYRO_ADDR 0xD0 // Write address.
#define GYRO_REG_X 0x1D // X register.
#define GYRO_REG_Y 0x1F // Y register.
#define GYRO_REG_Z 0x21 // Z register.

#define ACCEL_WRITE 0xA6
#define ACCEL_REGISTER 0

int readFromRegister(i2c*, int, int);
void writeToRegister(i2c*, int, int, int);
int readValue(i2c*, int, int);
int combine(int, int);

int main()
{
  i2c imu;

  waitcnt(CNT + CLKFREQ); 
  
  printf("Opening.\n");
  i2c_open(&imu, PIN_IMU_SCL, PIN_IMU_SDA, 0);
  
  //Set internal clock to 1kHz with 42Hz LPF and Full Scale to 3 for proper operation
  writeToRegister(&imu, GYRO_ADDR, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0); // 0x16 25

  //Set sample rate divider for 100 Hz operation
  writeToRegister(&imu, GYRO_ADDR, SMPLRT_DIV, 9);   //Fsample = Fint / (divider + 1) where Fint is 1kHz // 0x15 9

  //Setup the interrupt to trigger when new data is ready.
  //writeToRegister(&imu, GYRO_ADDR, INT_CFG, INT_CFG_RAW_RDY_EN | INT_CFG_ITG_RDY_EN); // 0x17 5

  //Select X gyro PLL for clock source
  writeToRegister(&imu, GYRO_ADDR, PWR_MGM, PWR_MGM_CLK_SEL_0);
  
  while(1)
  {
    waitcnt(CNT + CLKFREQ/10);
    
    int gx = readValue(&imu, GYRO_ADDR, GYRO_REG_X);
    int gy = readValue(&imu, GYRO_ADDR, GYRO_REG_Y);
    int gz = readValue(&imu, GYRO_ADDR, GYRO_REG_Z);

    printf("%6d %6d %6d\n", gx, gy, gz);
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

/*
1. Send a start sequence
2. Send 0xC0 ( I2C address of the CMPS03 with the R/W bit low (even address)
3. Send 0x01 (Internal address of the bearing register)
4. Send a start sequence again (repeated start)
5. Send 0xC1 ( I2C address of the CMPS03 with the R/W bit high (odd address)
6. Read data byte from CMPS03
7. Send the stop sequence.
*/