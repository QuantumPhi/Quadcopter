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

int main()
{
  i2c imu;

  waitcnt(CNT + CLKFREQ); 
  
  printf("Opening.\n");
  i2c_open(&imu, PIN_IMU_SCL, PIN_IMU_SDA, 0);
  
  /*
  //Set internal clock to 1kHz with 42Hz LPF and Full Scale to 3 for proper operation
  i2c_writeByte(&imu, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0); // 0x16 25

  //Set sample rate divider for 100 Hz operation
  i2c_writeByte(&imu, SMPLRT_DIV, 9);   //Fsample = Fint / (divider + 1) where Fint is 1kHz // 0x15 9

  //Setup the interrupt to trigger when new data is ready.
  i2c_writeByte(&imu, INT_CFG, INT_CFG_RAW_RDY_EN | INT_CFG_ITG_RDY_EN); // 0x17 5

  //Select X gyro PLL for clock source
  i2c_writeByte(&imu, PWR_MGM, PWR_MGM_CLK_SEL_0);
  */

  while(1)
  {
    waitcnt(CNT + CLKFREQ/10);
    
    i2c_start(&imu);
    i2c_writeByte(&imu, GYRO_WRITE);
    i2c_writeByte(&imu, GYRO_REGISTER);
    i2c_start(&imu);
    i2c_writeByte(&imu, GYRO_READ);
    int read1 = i2c_readByte(&imu, 0);
    int read2 = i2c_readByte(&imu, 0);
    i2c_stop(&imu);

    printf("%d %d\n", read1, read2);
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