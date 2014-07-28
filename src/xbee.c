#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

void xbeeInit()
{
  high(PIN_XBEE_OUT);
}

volatile unsigned int data;

unsigned int xbeeGetByte()
{
  int current = 1;

  while(current)
  {
    current = input(PIN_XBEE_IN);
  }

  waitcnt(CNT + CLKFREQ/(XBEE_BAUD_RATE*2));

  unsigned int currentByte = 0;
  unsigned long time = CNT;

  for (int i=0;i<8;i++)
  {
    waitcnt(time + CLKFREQ/XBEE_BAUD_RATE * (i+1));
    current = input(PIN_XBEE_IN);
    currentByte += current<<i;
  }

  return currentByte;
}

void xbeeSendByte(int byte)
{
  waitcnt(CNT + CLKFREQ/XBEE_BAUD_RATE);
  low(PIN_XBEE_OUT);

  unsigned long time = CNT;

  for (int i=0;i<8;i++)
  {
    waitcnt(time + CLKFREQ/XBEE_BAUD_RATE * (i+1));
    int bit = byte>>i & 1;
    if (bit)
      high(PIN_XBEE_OUT);
    else
      low(PIN_XBEE_OUT);
  }

  waitcnt(CNT + CLKFREQ/XBEE_BAUD_RATE);
  high(PIN_XBEE_OUT);
}