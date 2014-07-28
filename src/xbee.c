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

unsigned char xbeeGetByte()
{
  int current = 1;

  while(current)
  {
    waitcnt(CNT + CLKFREQ/9600);
    current = input(PIN_XBEE_IN);
  }

  unsigned char currentByte = 0;

  for (int i=0;i<8;i++)
  {
    waitcnt(CNT + CLKFREQ/9600);
    current = input(PIN_XBEE_IN);
    currentByte = currentByte<<1 + current;
  }

  return currentByte;
}