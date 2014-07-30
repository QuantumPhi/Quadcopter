#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void motorRun(int pin, double speed)
{
  run(1);
}

void run(int pulse)
{
  //printf("%d\n", pulse);
  int temp;
  for(int i=0;i<150+300;i++)
  {
    temp = CNT;
    low(4);
    waitcnt(CNT + pulse*CLKFREQ/1000);
    high(4);
    waitcnt(CNT + CLKFREQ/50); // 50hz
  }
}