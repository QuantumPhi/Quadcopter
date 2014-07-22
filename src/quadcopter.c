#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h> 
#include <stdio.h> 

int main()
{
  waitcnt(CNT + CLKFREQ); // Wait one second.
  
  imuInit();
  while(1)
  {
    waitcnt(CNT + CLKFREQ/10);
    imuUpdate();
    printf("G: %6d %6d %6d\t", gx, gy, gz);
    printf("A: %6d %6d %6d\n", ax, ay, az);
  }
}