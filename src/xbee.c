//0000 0110 -> 0000 0110
//0000 1000 -> 0000 0000
//1111 1111 -> 0111 1111
//0011 1111 -> 0001 1111

//0110 0000 -> 0110 0000
//0001 0000 -> 0000 0000
//1111 1111 -> 1111 1110
//1111 1100 -> 1111 1000
//0110 1001 -> 0111 0010

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

  waitcnt(CNT + CLKFREQ/2400);

  unsigned int currentByte = 0;
  unsigned long time = CNT;

  for (int i=0;i<8;i++)
  {
    waitcnt(time + CLKFREQ/1200 * (i+1));
    current = input(PIN_XBEE_IN);
    currentByte += current<<i;
  }

  return currentByte;
}

void xbeeSendByte(int byte)
{
  waitcnt(CNT + CLKFREQ/9600);
  low(PIN_XBEE_OUT);

  for (int i=0;i<8;i++)
  {
    int bit = byte<<i;
    if (bit)
      high(PIN_XBEE_OUT);
    else
      low(PIN_XBEE_OUT);
  }

  high(PIN_XBEE_OUT);
}