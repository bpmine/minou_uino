#ifndef MOVE_HEADER_INCLUDED
#define MOVE_HEADER_INCLUDED

class Move
{
  private:
    int m_pin;    
    int state;
    unsigned long t0;

    unsigned long delta(void);
  
  public:
    Move();
  
    void begin(int pin);
    bool tick(void);
    void reset(void);
};

#endif
