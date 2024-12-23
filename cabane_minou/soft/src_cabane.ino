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
#include "ldanim.h"
#include "when.hpp"
#include "mem.h"

#include <arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>


/**
 * @brief Configuration des bandeau de LEDs
 * @{
*/
#define POWER_MAX         80
#define POWER_MIN         50
#define NB_LEDS_CABANE    26
#define NB_LEDS_TOUR      34

int g_power=POWER_MIN;
LdAnim<PIN_OUT_LEDS_BAS, PIN_OUT_5V_LEDS_BAS, NB_LEDS_CABANE> ldBas;
LdAnim<PIN_OUT_LEDS_HAUT, PIN_OUT_5V_LEDS_HAUT, NB_LEDS_CABANE> ldHaut;
LdAnim<PIN_OUT_LEDS_TOUR, PIN_OUT_5V_LEDS_TOUR, NB_LEDS_TOUR> ldTour;

/*LdAnim animBas;
LdAnim animHaut;
LdAnim animTour;*/

/**
 * @}
*/
Timer tmrTemp(1000,false);
Timer tmrRefresh(2000,false);
Timer tmrPresence(10000,true);
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

#define NB_LEDS_HAUT  (10)
#define NB_LEDS_BAS   (10)
#define NB_LEDS_TOUR  (10)

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

#define AFF_OFF       0
#define AFF_BOOT      1
#define AFF_DEMO      2
#define AFF_AUTO      3
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

/**
 * @brief Allume toutes les LEDs avec la couleur fournie
 * @param pLeds Buffer des LEDs
 * @param a Couleur
*/
/*void setAll(CRGB *pLeds,CRGB a)
{
  for (int i=0;i<NUM_LEDS;i++)
  {
     pLeds[i] = a;
  }
}*/

/**
 * @brief Eteint toutes les LEDs
 * @param pLeds Buffer des LEDs 
*/
/*void clearAll(CRGB *pLeds)
{
  setAll(pLeds,CRGB::Black);
}*/


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
  
  dht_ext.begin();
  _init_input_pullup(PIN_IN_TEMP_EXT);
  dht_bas.begin();
  _init_input_pullup(PIN_IN_TEMP_BAS);

  Wire.begin();
  scheduler.begin();
  
  scr.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //scr.clearDisplay();
  //scr.display();

  //delay(500);  
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

  ldBas.setrg();
  ldHaut.setrg();
  ldTour.setrg();
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
  static bool pile=false;
  static bool demi=false;
  
  DateTime dte=scheduler.nowLocal();
  When w(dte);
        
  if (w.isSoiree())
  {
    Serial.println("Soiree");
    ldBas.pwron();
          
    if (w.isPile())
    {
      if (pile==false)
      {
        pile=true;
        //animHaut.init(progPileHaut,strlen(progPileHaut));
        //animTour.init(progPileTour,strlen(progPileTour));        
      }

      //animHaut.tick();
      //animTour.tick();
    }
    else
    {
      pile=false;
    }
          
    if (w.isDemi())
    {
      /// animer demi
    }
    
    if (g_presence_bas)
    {
      ldBas.setall(COL_BLACK);
      ldHaut.setall(COL_BLACK);
      ldTour.setall(COL_RED);  
    }
    else if (g_presence_haut)
    {
      ldBas.setall(COL_BLACK);
      ldHaut.setall(COL_GREEN);
      ldTour.setall(COL_RED);
    }
    else
    {
      Serial.println("normal");
      ldBas.setall(COL_RED);
      ldHaut.setall(COL_BLUE);
      ldTour.setall(COL_GREEN);
    }    
  }
  else
  {
    Serial.println("Journee");
    ldBas.pwroff();
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
  }
  
}

void loop() 
{
  delay(100);

  g_capot=(digitalRead(PIN_IN_CAPOT)==LOW)?true:false;
  g_veilleuse=!g_capot;

  if (moveBas.tick())
  {
    g_presence_bas=true;
    g_presence_haut=false;
    tmrPresence.start();
  }  
  
  if (moveHaut.tick())
  {
    g_presence_haut=true;
    g_presence_bas=false;
    tmrPresence.start();
  }

  tmrOutOffBoot.tick();
  
  if (tmrPresence.tick()==true)
  {
    g_presence_bas=false;
    g_presence_haut=false;
  }
  
  if (tmrTemp.tick()==true)
  {
    digitalWrite(PIN_OUT_LED1,!digitalRead(PIN_OUT_LED1));
    //digitalWrite(PIN_OUT_5V_LEDS_TOUR,!digitalRead(PIN_OUT_5V_LEDS_TOUR));

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
    }

    FastLED.show();
  }

  digitalWrite(PIN_OUT_LED_CAPOT_OUVERT,g_veilleuse?HIGH:LOW);
}
