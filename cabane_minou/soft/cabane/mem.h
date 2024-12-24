#ifndef MEM_HEADER_INCLUDED
#define MEM_HEADER_INCLUDED

class Mem
{
  private:
    unsigned char mode;
    
  public:
    void load(void);
    void save(void);

    int getMode(void);
    void setMode(int mode);
};

#endif
