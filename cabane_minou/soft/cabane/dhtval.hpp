class DhtVal
{
  private:
    float m_value;
    int m_errs;
    
  public:
    DhtVal() {}

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
};
