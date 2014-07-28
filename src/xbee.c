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

unsigned int xbeeGetByte()
{
  int current = 1;

  while(current)
  {
    current = input(PIN_XBEE_IN);
  }

  unsigned int currentByte = 0;

  for (int i=0;i<7;i++)
  {
    waitcnt(CNT + CLKFREQ/9600);
    current = input(PIN_XBEE_IN);
    currentByte += current<<i;
  }

  return currentByte;
}