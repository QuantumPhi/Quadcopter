#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void motorRun(int pin, double speed)
{
  pwm_start(2083);
  pwm_set(4, 0, 1958);
  //run(8000);
}

void run(int pulse)
{
  //printf("%d\n", pulse);
  int temp;
  for(int i=0;i<150+300;i++)
  {
    temp = CNT;
    low(4);
    waitcnt(temp + CLKFREQ/pulse);
    high(4);
    waitcnt(temp + CLKFREQ/480); // 50hz
  }
}