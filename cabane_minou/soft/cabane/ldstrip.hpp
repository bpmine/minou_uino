#ifndef LDSTRIP_HEADER_INCLUDED
#define LDSTRIP_HEADER_INCLUDED

#include <FastLED.h>

#define COL_BLACK     CRGB(0,0,0)
#define COL_RED       CRGB(255,0,0)
#define COL_GREEN     CRGB(0,255,0)
#define COL_BLUE      CRGB(0,0,255)
#define COL_WHITE     CRGB(127,127,127)

template <int PIN_DATA,int POW_5V, int NBRE>
class LdStrip
{
  protected:
    CRGB leds[NBRE];
    
  public:
    LdStrip()
    {
    }

    CRGB *getLeds(void)
    {
      return leds;
    }

    int getNbLeds(void)
    {
      return NBRE;
    }

    void setall(CRGB col)
    {
      for (int i=0;i<NBRE;i++)
      {
        leds[i]=col;
      }
    }

    void setrg(void)
    {
      for (int i=0;i<NBRE;i++)
      {
        if ((i%2)==0)
          leds[i]=COL_RED;
        else
          leds[i]=COL_GREEN;
      }      
    }

    /**
     * @brief Pilote la LED
     * @param inx Numero de LED
     * @param r Composante rouge (0-127)
     * @param g Composante verte (0-127)
     * @param b Composante bleue (0-127)
    */
    void setLed(int inx, char r, char g, char b)
    {
      if ( (inx>0) && (inx<NBRE) )
        leds[inx]=CRGB(r,g,b);
    }

    void clrall(void)
    {
      setall(COL_BLACK);
    }

    void begin(void)
    {
      digitalWrite(POW_5V,LOW);
      pinMode(POW_5V,OUTPUT);      
      FastLED.addLeds<NEOPIXEL, PIN_DATA>(leds, NBRE);      
    }

    void pwron(void)
    {
      digitalWrite(POW_5V,HIGH);
    }
    
    void pwroff(void)
    {
      digitalWrite(POW_5V,LOW);
    }
 
};
#endif
