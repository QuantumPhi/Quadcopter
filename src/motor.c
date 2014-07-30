#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void motorRun(int pin, double speed)
{
  set_direction(4, 1);
  run();
}

void run()
{
  int temp;
  while(1)
  {
    temp = CNT;
    set_output(4, 0);
    waitcnt(temp + CLKFREQ*0.0015);
    set_output(4, 1);
    waitcnt(temp + CLKFREQ*0.02); // 50hz
  }
}