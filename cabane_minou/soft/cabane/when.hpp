#ifndef WHEN_HEADER_INCLUDED
#define WHEN_HEADER_INCLUDED

class When
{
  private:
    bool soiree;
    bool matin;
    bool pile;
    bool demi;
    
  public:  
    When(DateTime &dte) 
    {
      soiree=false;
      matin=false;
      pile=false;
      demi=false;

      if ( (dte.hour()>=17) || (dte.hour()<1) )
      {
        soiree=true;
      }
      else if ( (dte.hour()>=6) && (dte.hour()<8) )
      {
        matin=true;
      }
  
      if ( dte.minute()==0)
      {
        pile=true;
      }
      else
      {
        pile=false;
      }
      
      if ( dte.minute()==0)
      {
        demi=true;
      }
      else
      {
        demi=false;
      }
    }

    bool isSoiree(void) {return soiree;}
    bool isMatin(void) {return matin;}
    bool isPile(void) {return pile;}
    bool isDemi(void) {return demi;}
};

#endif
