#include "move.h"
#include <arduino.h>

#define ST_BOOT       0
#define ST_BOOTING    1
#define ST_READY      2
#define ST_RISING     3
#define ST_DETECTED   4

Move::Move()
{
  m_pin=-1;
  state=ST_BOOT;
  t0=0;
}

void Move::begin(int pin)
{  
  m_pin=pin;
  pinMode(m_pin,INPUT);
  state=ST_BOOT;
  t0=millis();
}

unsigned long Move::delta(void)
{
  unsigned long t=millis();

  if (t>=t0)
    return t-t0;
  else
    return 0xFFFFFFFF-t0+t;  
}

bool Move::tick(void)
{
  if (m_pin==-1)
    return false;

  //Serial.println(state);
  switch (state)
  {
    case ST_BOOT:
    default:
    {
      if (digitalRead(m_pin)==LOW)
      {
        state=ST_BOOTING;
        t0=millis();        
      }
      
      break;
    }

    case ST_BOOTING:
    {
      if (digitalRead(m_pin)!=LOW)
        state=ST_BOOT;
      else if ( (delta()>100) && (digitalRead(m_pin)==LOW))
        state=ST_READY;        
      
      break;
    }

    case ST_READY:
    {
      if (digitalRead(m_pin)==HIGH)
      {
        state=ST_RISING;
        t0=millis();
      }
      
      break;
    }

    case ST_RISING:
    {
      if (digitalRead(m_pin)==LOW)
        state=ST_READY;
      else if (delta()>500)
        state=ST_DETECTED; 

      break;
    }

    case ST_DETECTED:
    {
      if (digitalRead(m_pin)==LOW)
        state=ST_READY;      
        
      break;
    }
  }

  if (state==ST_DETECTED)
    return true;
  else
    return false;
}

void Move::reset(void)
{
  if (m_pin==-1)
    return;

  state=ST_BOOT;
}
