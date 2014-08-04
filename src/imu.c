#include "quadcopter.h"
#include "simpletools.h"

i2c imu;

volatile Gyro g;
volatile Accel a;

volatile int lock;
unsigned long last;

void imu_run()
{
  while(1)
  {
    last = CNT;
    waitcnt(CNT + CLKFREQ/10);
    imu_update();
  }
}

void imu_init()
{
  lock = 0;
  i2c_open(&imu, PIN_IMU_SCL, PIN_IMU_SDA, 0);

  // Gyro initialization.
  // 22 -> 11011  Set internal clock and scale.
  // 21 -> 9      Set sample rate.
  // 23 -> 101    Trigger interrupt when new data is ready (extra int pins).
  // 62 -> 1      Set clock source.
  write_to_register(&imu, GYRO_ADDR, 0x16, 0x1A);
  write_to_register(&imu, GYRO_ADDR, 0x15, 0x09);
  write_to_register(&imu, GYRO_ADDR, 0x17, 0x05);
  write_to_register(&imu, GYRO_ADDR, 0x3E, 1);
  
  // Accel initialization.
  // 45 -> 1000, 100  Wake it from sleep, keep it awake.
  // 49 -> 01    Set the data range. 00->2, 01->4, 10->8, 11->16 (+- g).
  write_to_register(&imu, ACCL_ADDR, 0x2D, 16);
  write_to_register(&imu, ACCL_ADDR, 0x2D, 8);
  write_to_register(&imu, ACCL_ADDR, 0x31, 0); // Might need to change this later.
  //writeToRegister(&imu, ACCL_ADDR, 0x1E, -25/-4); -25 // Tuning values.
  //writeToRegister(&imu, ACCL_ADDR, 0x1F, 12/-4);   12
  //writeToRegister(&imu, ACCL_ADDR, 0x20, 249/4);  249
}

void imu_update()
{
  while(lock==1);
  lock = 1;

  g.x.raw = (signed short) read_value(&imu, GYRO_ADDR, GYRO_REG_X, 0);
  g.y.raw = (signed short) read_value(&imu, GYRO_ADDR, GYRO_REG_Y, 0);
  g.z.raw = (signed short) read_value(&imu, GYRO_ADDR, GYRO_REG_Z, 0);

  a.x.raw = (signed short) read_value(&imu, ACCL_ADDR, ACCL_REG_X, 1);
  a.y.raw = (signed short) read_value(&imu, ACCL_ADDR, ACCL_REG_Y, 1);
  a.z.raw = (signed short) read_value(&imu, ACCL_ADDR, ACCL_REG_Z, 1);

  a.x.filter = a.x.raw*GYRO_FILTER_ALPHA + a.x.filter*(1-GYRO_FILTER_ALPHA);
  a.y.filter = a.y.raw*GYRO_FILTER_ALPHA + a.y.filter*(1-GYRO_FILTER_ALPHA);
  a.z.filter = a.z.raw*GYRO_FILTER_ALPHA + a.z.filter*(1-GYRO_FILTER_ALPHA);
  
  a.roll = atan2(-a.y.filter, a.z.filter);
  a.pitch = atan2(a.x.filter, sqrt(a.y.filter*a.y.filter + a.z.filter*a.z.filter));

  g.x.sum += g.x.raw;
  g.y.sum += g.y.raw;
  g.z.sum += g.z.raw;

  // 1⁄6 ( vali-3 + 2 vali-2 + 2 vali-1 + vali)

  g.x.sum += (g.x.last[2] + 2*g.x.last[1] + 2*g.x.last[0] + g.x.raw)/6;
  g.y.sum += (g.y.last[2] + 2*g.y.last[1] + 2*g.y.last[0] + g.y.raw)/6;
  g.z.sum += (g.z.last[2] + 2*g.z.last[1] + 2*g.z.last[0] + g.z.raw)/6;

  g.x.last[2] = g.x.last[1];
  g.x.last[1] = g.x.last[0];
  g.x.last[0] = g.x.raw;

  g.y.last[2] = g.y.last[1];
  g.y.last[1] = g.y.last[0];
  g.y.last[0] = g.y.raw;

  g.z.last[2] = g.z.last[1];
  g.z.last[1] = g.z.last[0];
  g.z.last[0] = g.z.raw;
  
  lock = 0;
}

double errSum = 0;
double lastErr = 0;
double kp = 1;
double ki = 1;
double kd = 1;
int compute_pid(int actualVal, int expectedVal)
{
  double time = 10;
  
  double error = expectedVal - actualVal;
  errSum += (error * time);
  double dErr = (error - lastErr) / time;
  
  int output = kp * error + ki * errSum + kd * dErr;
  
  lastErr = error;

  return output;
}