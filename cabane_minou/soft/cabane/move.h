class Move
{
  private:
    int m_pin;
    long m_cnt;  
    
    bool m_last_state;
    bool m_falling;
  
  public:
    Move(bool falling=false);
  
    void begin(int pin);

    bool tick(void);
    void reset(void);

    long getCount(void);
};
