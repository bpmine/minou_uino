class Timer
{
  private:
    unsigned long tick0_ms;
    unsigned long duration_ms;   
    bool running; 
    bool single;
  
  public:
    Timer(long duration_ms,bool single=true);

    bool tick(void);
    void reset(void);
    void setDuration(unsigned long duration_ms); 
    void stop(void);
    void start(void);   

    bool isRunning(void);
    unsigned long getRemaining_ms(void);
    unsigned long getDuration_ms(void);
};
