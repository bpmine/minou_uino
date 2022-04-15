#include "move.h"
#include <arduino.h>

Move::Move(bool falling)
{
  m_falling=falling;
  m_pin=-1;
  m_cnt=0;
  m_last_state=false;
}

void Move::begin(int pin)
{  
  m_pin=pin;
  m_cnt=0;
  m_last_state=digitalRead(m_pin)==HIGH?true:false;
}

bool Move::tick(void)
{
  if (m_pin==-1)
    return false;
    
  bool st;
  if (m_falling==false)
    st=digitalRead(m_pin)==HIGH?true:false;
  else  
    st=digitalRead(m_pin)==HIGH?false:true;

  if ((st==true) && (m_last_state==false) )
  {
    m_cnt++;
    m_last_state=st;
    return true;
  }

  m_last_state=st;
  return false;
}

void Move::reset(void)
{
  if (m_pin==-1)
    return;

  m_cnt=0;
  m_last_state=digitalRead(m_pin)==HIGH?true:false;
}

long Move::getCount(void)
{
  return m_cnt;
}
