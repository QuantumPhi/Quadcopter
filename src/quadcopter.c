#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

int main()
{
  xbee_init();

  cog_run(&ultrasonic_run, 0);
  cog_run(&motor_run, 1);

  imu_init();
  cog_run(&imu_run, 2);

  //signed short gx,gy,gz,ax,ay,az;
  //
  //while(1)
  //{
  //  waitcnt(CNT + CLKFREQ/10);
  //  get_imu_data(&gx, &gy, &gz, &ax, &ay, &az);
  //  printf("G: %5d %5d %5d\t", gx, gy, gz);
  //  printf("A: %5d %5d %5d\n", ax, ay, az);
  //}
}