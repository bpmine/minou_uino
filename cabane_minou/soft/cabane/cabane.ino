/**
 * @brief PROGRAMME CABANE DE MINOOU
 * 
 * ATMEGA 2560
 * 
*/
#include "pins.h"
#include "timer.h"
#include "move.h"
#include "dhtval.hpp"
#include "scheduler.h"
//#include "ldstrip.hpp"
#include "when.hpp"
#include "mem.h"

#include <arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "ldanim.hpp"


/**
 * @brief Configuration des bandeau de LEDs
 * @{
*/
#define POWER_MAX         80
#define POWER_MIN         50

int g_power=POWER_MIN;
LdStrip<PIN_OUT_LEDS_BAS, PIN_OUT_5V_LEDS_BAS, NB_LEDS_CABANE> ldBas;
LdStrip<PIN_OUT_LEDS_HAUT, PIN_OUT_5V_LEDS_HAUT, NB_LEDS_CABANE> ldHaut;
LdStrip<PIN_OUT_LEDS_TOUR, PIN_OUT_5V_LEDS_TOUR, NB_LEDS_TOUR> ldTour;
LdStrip<PIN_OUT_LEDS_RES, PIN_OUT_5V_LEDS_RES, NB_LEDS_TOUR> ldRes;

//#define SIZE_PROG (500)
//char prog[SIZE_PROG] = "S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*";


/**
 * @}
*/
Timer tmrTemp(1000,false);
Timer tmrRefresh(2000,false);
Timer tmrPresence(60000,true);
Timer tmrOutOffBoot(4000,true);

Scheduler scheduler;
Mem mem;

DHT dht_ext(PIN_IN_TEMP_EXT,DHT22);
DHT dht_bas(PIN_IN_TEMP_BAS,DHT22);

Adafruit_SSD1306 scr(128,64,&Wire,-1);

DhtVal g_temp_ext;
DhtVal g_hum_ext;
DhtVal g_temp_bas;
DhtVal g_hum_bas;

Move moveBas;
Move moveHaut;

bool g_capot=false;
bool g_veilleuse=false;

bool g_presence_bas=false;
bool g_presence_haut=false;
bool g_presence_tour=false;

#define MODE_TEST     1
#define MODE_NORMAL   0
int g_mode;

#define AFF_OFF         0
#define AFF_DEMO        1
#define AFF_AUTO        2
#define AFF_GUIRLANDE   3
int g_aff;

inline void _init_output_low(int pin)
{
  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);  
}

inline void _init_input(int pin)
{
  pinMode(pin,INPUT);
}

inline void _init_input_pullup(int pin)
{
  pinMode(pin,INPUT_PULLUP);
}

void setup() 
{  
  _init_output_low(PIN_OUT_LEDS_TOUR);
  _init_output_low(PIN_OUT_LEDS_RES);
  _init_output_low(PIN_OUT_LEDS_HAUT);
  _init_output_low(PIN_OUT_3V_ESP);
  _init_output_low(PIN_OUT_LEDS_BAS);
  _init_output_low(PIN_OUT_LED1);

  _init_output_low(PIN_OUT_CMD_CHAUFFAGE);
  _init_output_low(PIN_OUT_LED2);
  _init_output_low(PIN_OUT_LED3);
  _init_output_low(PIN_OUT_5V_LEDS_TOUR);
  _init_output_low(PIN_OUT_5V_SER);
  _init_output_low(PIN_OUT_5V_LEDS_RES);
  _init_output_low(PIN_OUT_5V_LEDS_HAUT);
  _init_output_low(PIN_OUT_5V_LEDS_BAS);
  _init_output_low(PIN_OUT_LED_CAPOT_OUVERT);
  
  _init_input(PIN_IN_SW1_BUTTON);
  _init_input(PIN_IN_SW2_BUTTON);
  _init_input(PIN_IN_SW3_BUTTON); 
  _init_input(PIN_IN_PRESENCE_HAUT);
  _init_input(PIN_IN_PRESENCE_TOUR_BAS);
  _init_input(PIN_IN_PRESENCE_TOUR_HAUT);
  _init_input(PIN_IN_CAPOT);
  _init_input(PIN_IN_PRESENCE_BAS);
  _init_input_pullup(PIN_IN_TEMP_BAS);
  _init_input(PIN_IN_MES_12V);

  Serial.begin(9600);
  Serial.println("Boot...");

  g_power=POWER_MAX;

  g_capot=false;
  g_veilleuse=false;
  g_presence_bas=false;
  g_presence_haut=false;
  g_presence_tour=false;
  
  g_mode=MODE_NORMAL;  
  mem.load();  
  g_aff=mem.getMode();
  Serial.print("Mode ");
  Serial.println(g_aff);
  
  dht_ext.begin();
  _init_input_pullup(PIN_IN_TEMP_EXT);
  dht_bas.begin();
  _init_input_pullup(PIN_IN_TEMP_BAS);

  Wire.begin();
  scheduler.begin();
  
  scr.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  scr.clearDisplay();
  scr.setTextSize(2);
  scr.setTextColor(WHITE);
  scr.setCursor(10,20);
  scr.write("Cab Minou");  
  scr.setTextSize(1);
  scr.setCursor(40,50);
  scr.write("Booting...");
  scr.display();

  moveBas.begin(PIN_IN_PRESENCE_BAS);
  moveHaut.begin(PIN_IN_PRESENCE_HAUT);
 
  tmrTemp.start();
  tmrRefresh.start();
  tmrOutOffBoot.start();

  ldBas.begin();
  ldHaut.begin();
  ldTour.begin();
  ldRes.begin();

  ldBas.setrg();
  ldHaut.setrg();
  ldTour.setrg();
  ldRes.setrg();

  FastLED.setBrightness(g_power);
  FastLED.show();


  //scheduler.updateFromCompilationDateTime();
  //scheduler.setDateTime(21,12,2024,0,43,00);
  
}

void _manage_mode_test(void)
{
  
  
}

void _aff_auto(void)
{
  DateTime dte=scheduler.nowLocal();
  When w(dte);

  if (w.isSoiree() && (g_capot))
  {
    ldBas.pwron();
    ldHaut.pwron();
    ldTour.pwron();
        
    ldHaut.clrall();
    ldTour.clrall();
    ldBas.setall(COL_GREEN);
  }
  else if ( w.isSoiree() || w.isMatin() )
  {
    ldBas.pwron();
    ldHaut.pwron();
    ldTour.pwron();
          
    if (w.isPile() || w.isDemi())
    {
      ldBas.setall(COL_BLACK);
      ldHaut.setall(COL_BLUE);
      ldTour.setall(COL_BLUE);
    }
    else
    {
      if (g_presence_bas)
      {
        ldBas.setall(COL_BLACK);
        ldHaut.setall(COL_BLACK);
        ldTour.setall(COL_RED);
      }
      else if (g_presence_haut)
      {
        ldBas.setall(COL_RED);
        ldHaut.setall(COL_GREEN);
        ldTour.setall(COL_RED);
      }
      else if (g_presence_tour)
      {
        ldBas.setall(COL_RED);
        ldHaut.setall(COL_RED);
        ldTour.setall(COL_GREEN);
      }
      else
      {
        ldBas.setall(COL_BLACK);
        ldHaut.setall(COL_BLACK);
        ldTour.setall(COL_GREEN);
      }
    }
  }
  else
  {
    ldBas.clrall();
    ldHaut.clrall();
    ldTour.clrall();
    ldRes.clrall();

    ldBas.pwroff();
    ldHaut.pwroff();
    ldTour.pwroff();
    ldRes.pwroff();
  }  
}

void _aff_guirlande(void)
{
  DateTime dte=scheduler.nowLocal();
  When w(dte);

  if (w.isSoiree())
  {
    ldBas.pwron();
    ldHaut.pwron();
    ldTour.pwron();

    ldBas.clrall();
    ldHaut.clrall();
    ldTour.clrall();
    ldRes.clrall();
  }
  else
  {
    ldBas.clrall();
    ldHaut.clrall();
    ldTour.clrall();
    ldRes.clrall();

    ldBas.pwroff();
    ldHaut.pwroff();
    ldTour.pwroff();
    ldRes.pwroff();
  }
}

void _manage_mode_normal(void)
{
  switch (g_aff)
  {
    case AFF_OFF:
    default:
    {
      ldBas.clrall();
      ldHaut.clrall();
      ldTour.clrall();
      break;
    }

    case AFF_AUTO:
    {
      _aff_auto();
      break;
    }

    case AFF_GUIRLANDE:
    {
      _aff_guirlande();
      break;
    }
  }
  
}

void loop()
{
  delay(10);

  g_capot=(digitalRead(PIN_IN_CAPOT)==LOW)?false:true;
  g_veilleuse=g_capot;

  tmrOutOffBoot.tick();

  if (moveHaut.tick())
  {
    g_presence_haut=true;
    g_presence_bas=false;
    moveBas.reset();
    tmrPresence.start();
  }

  if (moveBas.tick())
  {
    g_presence_bas=true;
    g_presence_haut=false;
    moveHaut.reset();
    tmrPresence.start();
  }  
  
  /// On maintient la presence tant que le capteur est true
  if (tmrPresence.isRunning())
  {
    if ( (g_presence_haut==true) && (moveHaut.isStillMoving()) )
      tmrPresence.start();

    if ( (g_presence_bas==true) && (moveBas.isStillMoving()) )
      tmrPresence.start();
  }
  
  /// On n'a plus de presence au bout du timer !
  if (tmrPresence.tick()==true)
  {
    g_presence_bas=false;
    g_presence_haut=false;
  }
  
  /// Mesure periodique de la temperature/humidite
  if (tmrTemp.tick()==true)
  {
    digitalWrite(PIN_OUT_LED1,!digitalRead(PIN_OUT_LED1));

    float val=dht_ext.readTemperature();
    g_temp_ext.set(val);
    val=dht_ext.readHumidity();
    g_hum_ext.set(val);

    val=dht_bas.readTemperature();
    g_temp_bas.set(val);
    val=dht_bas.readHumidity();
    g_hum_bas.set(val);  
  }

  if (tmrRefresh.tick()==true)
  { 
    if (tmrOutOffBoot.isRunning()==false)
    {
      if (g_mode==MODE_TEST)
      {
        _manage_mode_test();
      }
      else
      {
        _manage_mode_normal();
      }

      scr.clearDisplay();
      scr.setTextSize(2);
      scr.setTextColor(WHITE);

      char tmp[20];
      char s1[10];
      char s2[10];

      sprintf(tmp,"Mode: %d",g_aff);
      scr.setCursor(5,10);
      scr.write(tmp);

      scr.setTextSize(1);
      g_temp_ext.getStr(s1);
      g_hum_ext.getStr(s2);
      sprintf(tmp,"Ext: %s%cC / %s%%",s1,248,s2);
      scr.setCursor(5,35);
      scr.write(tmp);

      g_temp_bas.getStr(s1);
      g_hum_bas.getStr(s2);
      sprintf(tmp,"Bas: %s%cC / %s%%",s1,248,s2);
      scr.setCursor(5,50);
      scr.write(tmp);

      scr.display();
    }

    FastLED.show();
  }

  digitalWrite(PIN_OUT_LED_CAPOT_OUVERT,g_veilleuse?HIGH:LOW);
}
