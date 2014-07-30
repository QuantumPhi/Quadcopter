#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void motorRun(int pin, double speed)
{
  pwm_start(20000);
  pwm_set(4, 0, 1000);
  waitcnt(CNT + CLKFREQ*5);
  pwm_set(4, 0, 2000);
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