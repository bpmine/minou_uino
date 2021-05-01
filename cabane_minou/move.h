class Move
{
  private:
    int m_pin;
    long m_cnt;  
    
    bool m_last_state;
  
  public:
    Move();
  
    void begin(int pin);

    bool tick(void);
    void reset(void);

    long getCount(void);
};
