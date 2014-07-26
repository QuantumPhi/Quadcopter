#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void xbeeInit()
{
  // init
}

volatile int transmitting = 0;
volatile int currentByte;
volatile int dataCounter;
//volatile byte* data;

void xbeeRun()
{
  while(1)
  {
    xbeeGetByte();
  }
}

int xbeeGetByte()
{
  while(1) 
  {
    waitcnt(CNT + CLKFREQ/9600);
    int current = input(PIN_XBEE_IN);

    if (!transmitting)
    {
      if (current)
      {
        continue;
      }
      else
      {
        transmitting = 1;
        currentByte = 0;
        dataCounter = 0;
      }
    }

    if (dataCounter == 8)
    {
      if (!current)
      {
        // There's a problem
        continue;
      }
      else
      {
        break;
      }
    }

    currentByte = (currentByte<<1)|(current<<dataCounter);
    dataCounter++;
  }
  return currentByte;
}