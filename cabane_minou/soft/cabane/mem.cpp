#include "mem.h"

#include <EEPROM.h>

void Mem::load(void)
{
  unsigned char magic1=EEPROM.read(0);
  unsigned char val=EEPROM.read(1);
  unsigned char magic2=EEPROM.read(2);

  if ((magic1==0xAA) && (magic2==0x55))
  {
    mode=val;
  }
  else
  {
    mode=0;
    save();
  }
}

void Mem::save(void)
{
  EEPROM.write(0,0xAA);
  EEPROM.write(1,mode);
  EEPROM.write(2,0x55);
}

int Mem::getMode(void)
{
  return (int)mode;
}

void Mem::setMode(int mode)
{
  if ( (mode>=0) && (mode<256) )
  {
    this->mode=(unsigned char)(mode&0xFF);
    save();
  }    
}
