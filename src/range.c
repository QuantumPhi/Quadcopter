#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void ultrasonic_run()
{
  while(1)
  {
    high(PIN_RANGE_TRIG);
    waitcnt(CNT + CLKFREQ/100000); // 10 uS
    low(PIN_RANGE_TRIG);

    int duration = pulse_in(PIN_RANGE_ECHO, 1)/58;

    if (duration > 400)
      duration = 400
    range = duration;
    waitcnt(CNT + CLKFREQ/17);
  }
}
