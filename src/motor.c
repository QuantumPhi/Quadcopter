#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

Axis axisX;
Axis axisY;
Axis axisZ;

Motor fr = { .pin = PIN_MOTOR_FR, .current_val = 0 };
Motor fl = { .pin = PIN_MOTOR_FL, .current_val = 0 };
Motor br = { .pin = PIN_MOTOR_BR, .current_val = 0 };
Motor bl = { .pin = PIN_MOTOR_BL, .current_val = 0 };

Motor* motors[4];

void motor_init()
{
  motors[0] = &fr;
  motors[1] = &fl;
  motors[2] = &br;
  motors[3] = &bl;
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