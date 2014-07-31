#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void motorInit()
{

}

void motorRun()
{
  while(1)
  {
    int signal = lastCommand;
    if (signal == 0x01)
      motorWakeup();
    else if (signal == 0x02)
      motorGo(1);
    else if (signal == 0x03)
      motorStop();
  }
}
 
void motorGo(double speed)
{
  pwm_set(4, 0, (MOTOR_HIGH-MOTOR_LOW)*speed + MOTOR_LOW);
}

void motorStop()
{
  pwm_set(4, 0, MOTOR_START);
}

void motorWakeup()
{
  pwm_start(20000);
  pwm_set(4, 0, MOTOR_START);
}