#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void motor_init()
{

}

void motor_run()
{
  while(1)
  {
    int signal = lastCommand;
    if (signal == 0x01)
      motor_wakeup();
    else if (signal == 0x02)
      motor_go(1);
    else if (signal == 0x03)
      motor_stop();
  }
}
 
void motor_go(double speed)
{
  pwm_set(4, 0, (MOTOR_HIGH-MOTOR_LOW)*speed + MOTOR_LOW);
}

void motor_stop()
{
  pwm_set(4, 0, MOTOR_START);
}

void motor_wakeup()
{
  pwm_start(20000);
  pwm_set(4, 0, MOTOR_START);
}