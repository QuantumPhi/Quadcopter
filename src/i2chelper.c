#include "quadcopter.h"
#include "simpletools.h"
#include "simplei2c.h"
#include <propeller.h>
#include <stdio.h>

unsigned short readValue(i2c* bus, int address, int regAddr, int sb)
{
  i2c_start(bus);
  i2c_writeByte(bus, address);
  i2c_writeByte(bus, regAddr);
  i2c_start(bus);
  i2c_writeByte(bus, address+1);

  char b1 = i2c_readByte(bus, 0);
  char b2 = i2c_readByte(bus, 1);

  unsigned short val;
  if (sb == 0)
    val = combine(b1, b2);
  else
    val = combine(b2, b1);

  i2c_stop(bus);

  return val;
}

unsigned short readFromRegister(i2c* bus, int address, int regAddr)
{
  i2c_start(bus);
  i2c_writeByte(bus, address);
  i2c_writeByte(bus, regAddr);
  i2c_start(bus);
  i2c_writeByte(bus, address+1);

  unsigned short val = i2c_readByte(bus, 1);

  i2c_stop(bus);

  return val;
}

unsigned short combine(char h, char l) {
  return (h<<8)|l;
}

void writeToRegister(i2c* bus, int address, int regAddr, int val)
{
  i2c_start(bus);
  i2c_writeByte(bus, address);
  i2c_writeByte(bus, regAddr);
  i2c_writeByte(bus, val);
  i2c_stop(bus);
}