#ifndef WHEN_HEADER_INCLUDED
#define WHEN_HEADER_INCLUDED

class When
{
  private:
    bool soiree;
    bool pile;
    bool demi;
    
  public:  
    When(DateTime &dte) 
    {
      soiree=false;
      pile=false;
      demi=false;
      
      if ( (dte.hour()>17) || (dte.hour()<2) )
      {
        soiree=true;
      }
      else
      {
        soiree=false;
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

      /// @todo A retirer
      soiree=true;
    }

    bool isSoiree(void) {return soiree;}
    bool isPile(void) {return pile;}
    bool isDemi(void) {return demi;}
};

#endif
