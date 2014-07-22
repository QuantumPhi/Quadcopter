#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

int main()
{
  waitcnt(CNT + CLKFREQ); // Wait one second.

  imuInit();
  cog_run(&imuRun, 0);

  while(1)
  {
    waitcnt(CNT + CLKFREQ/10);
    printf("G: %5d %5d %5d\t", gx, gy, gz);
    printf("A: %5d %5d %5d\n", ax, ay, az);
  }
}