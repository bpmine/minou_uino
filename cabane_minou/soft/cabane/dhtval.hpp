#ifndef DHTVAL_HEADER_INCLUDED
#define DHTVAL_HEADER_INCLUDED

#include <arduino.h>

class DhtVal
{
  private:
    float m_value;
    int m_errs;
    
  public:
    DhtVal() : m_value(0),m_errs(0) {}

    void set(float val)
    {
      if (!isnan(val))
      {
        m_value=val;
        m_errs=0;
      }
      else
      {
        if (m_errs<10)
        {
          m_errs++;
        }
        else
        {
          m_value=0.0/0.0;
        }
      }
    }
    
    float get(void)
    {
      return m_value;
    }

    void getStr(char *str)
    {
      dtostrf(get(),3,1,str);
    }
};

#endif
