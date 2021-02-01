#include "DHTesp.h"
#include "FastLED.h"
#include <SD.h>
#include  <SPI.h>
#include <Wire.h>
#include <RTClib.h>

//#define FORCE_NUIT

#define POWER_MAX         80
#define POWER_MIN         50
#define NUM_LEDS          26

#define PIN_SDCARD_CS     10

#define PIN_BOUTON        6

#define MOVE_BAS          7
#define LED_BAS_PIN       8

#define CAPOT_PIN         9

#define MOVE_HAUT         2


#define DHT11_BAS_PIN     5
#define PIN_DATA_LED_BAS  4
#define PIN_DATA_LED_HAUT 3

#define DHT11_OUT_PIN     A0
#define LUM_PIN           A3

#define SB_FIRST_CYCLE    (0x01)
#define SB_EVENT          (0x02)
#define SB_NOSD           (0x04)
byte g_bitsSystem=0;

#define IB_CAPOT_BAS      (0x01)
#define IB_MOVE_BAS       (0x02)
#define IB_BOUTON         (0x04)
#define IB_MOVE_HAUT      (0x08)
byte g_bitsInputs=0;
byte g_bitsInputsFront=0;

#define OB_LED_BAS        (0x01)
byte g_bitsOutputs=0;
byte g_bitsOutputsPrev=0;

byte g_in_tempBas=0;
byte g_in_humBas=0;
byte g_in_tempOut=0;
byte g_in_humOut=0;

#define LIGHT_SEUIL_NIGHT (110)
#define LIGHT_SEUIL_DAY (90)
int g_in_lumiere=0;
bool g_night=false;

byte g_power=POWER_MIN;

long t0=0;
long t00=0;

byte g_mode=0;

#define TM_DISPLAY_MODE_S     (5)
#define TM_MOVE_BAS_FILTER_S  (50)
#define TM_MOVE_BAS_LIGHT_M   (1)
#define TM_MOVE_HAUT_FILTER_S (50)
#define TM_LOG_M              (2)

byte g_tmLog_m=0;
byte g_tmMoveFilterBas_s=0;
byte g_tmMoveLightBas_m=0;
byte g_tmMoveFilterHaut_s=0;
byte g_tmShow_s=0;

#define TICK_TMR(tmr) {if ( (tmr>0) && (tmr!=255))  {tmr--;if (tmr==0) tmr=255;}} 

int g_nbreMoveBas=0;
int g_nbreMoveHaut=0;

DHTesp dht_bas;
DHTesp dht_out;
DS1307 RTC;
//File dataFile;

CRGB leds_bas[NUM_LEDS];
CRGB leds_haut[NUM_LEDS];

/*bool createNewFileIfNeeded_ld()
{
  char fn[10];
  for (int i=0;i<10000;i++)
  {
    sprintf(fn,"%04d.log",i);
    if (!SD.exists(fn))
      break;
  }

  Serial.print("O/C ");
  Serial.print(fn);
  Serial.println("...");
  
  dataFile = SD.open(fn, FILE_WRITE);
  if (dataFile)
  {    
      Serial.println("Write Header");
      dataFile.print("DATE;");
      dataFile.print("HEURE;");
      dataFile.print("SYST;");
      dataFile.print("IN;OUT;");
      dataFile.print("T_BAS;");
      dataFile.print("T_OUT;");
      dataFile.print("H_BAS;");
      dataFile.print("H_OUT;");
      dataFile.print("LUM;");
      dataFile.println("MOVES;");
      dataFile.flush();
      Serial.println("Done");

      return true;
  } 
  else
  {
    Serial.println("Error opening file...");
    return false;
  }
}*/

File dataFile;
char fn[10];

void initFileName()
{
  DateTime dte = RTC.now();
  int year=(dte.year()-2000);
  sprintf(fn, "%02d%02d%02d.g",year,dte.month(),dte.day());

  Serial.print("Fichier:");
  Serial.println(fn);
}

void logBoot()
{
      File file=SD.open("bt.log", FILE_WRITE);
      if (file) 
      {
        char tmp[15];
        DateTime now = RTC.now();
        sprintf(tmp, "%02d/%02d/%04d",now.day() , now.month(), now.year());
        file.print(tmp);
        file.print(" ");

        sprintf(tmp, "%02d:%02d:%02d",now.hour() , now.minute(), now.second()); 
        file.print(tmp);
        file.println("> Boot");          

        file.close();
      }  
}


void printHeader()
{
    bool flg=SD.exists(fn);
    if (flg==false)
    {
      dataFile=SD.open(fn, FILE_WRITE);
      if (dataFile) 
      {
        dataFile.print("DATE;");
        dataFile.print("HEURE;");
        dataFile.print("SYST;");
        dataFile.print("IN;OUT;");
        dataFile.print("T_BAS;");
        dataFile.print("T_OUT;");
        dataFile.print("H_BAS;");
        dataFile.print("H_OUT;");
        dataFile.print("LUM;");
        dataFile.println("MOVES;"); 
        
        dataFile.close(); 
      }
    }
}

void logState()
{
  if ((g_bitsSystem&SB_NOSD)==SB_NOSD)
    return;

    //printHeader();

  dataFile=SD.open(fn, FILE_WRITE);  
  if (dataFile) 
  {    
    char tmp[15];
    DateTime now = RTC.now();
    sprintf(tmp, "%02d/%02d/%04d",now.day() , now.month(), now.year());
    dataFile.print(tmp);
    dataFile.print(";");
    
    sprintf(tmp, "%02d:%02d:%02d",now.hour() , now.minute(), now.second()); 
    dataFile.print(tmp);
    dataFile.print(";");          

    dataFile.print(g_bitsSystem);
    dataFile.print(";");          
    dataFile.print(g_bitsInputs);
    dataFile.print(";");          
    dataFile.print(g_bitsOutputs);
    dataFile.print(";");          
    dataFile.print(g_in_tempBas);
    dataFile.print(";");    
    dataFile.print(g_in_tempOut);
    dataFile.write(";");          
    dataFile.print(g_in_humBas);
    dataFile.print(";");          
    dataFile.print(g_in_humOut);
    dataFile.print(";");          
    dataFile.print(g_in_lumiere);
    dataFile.print(";");
    dataFile.print(g_nbreMoveBas);
    dataFile.print(";");
    dataFile.print(g_nbreMoveHaut);
    dataFile.println(";");
    
    dataFile.close();
  }
}

void setAll(CRGB *pLeds,CRGB a)
{
  for (int i=0;i<NUM_LEDS;i++)
  {
     pLeds[i] = a;
  }
}

void setBBR(CRGB *pLeds)
{
  clearAll(pLeds);

  pLeds[0]=CRGB::Red;
  pLeds[1]=CRGB::Red;
  pLeds[2]=CRGB::Red;
  pLeds[3]=CRGB::Red;

  //pLeds[NUM_LEDS/2]=CRGB::White;

  pLeds[NUM_LEDS-1]=CRGB::Blue;
  pLeds[NUM_LEDS-2]=CRGB::Blue;
  pLeds[NUM_LEDS-3]=CRGB::Blue;
  pLeds[NUM_LEDS-4]=CRGB::Blue;
  pLeds[NUM_LEDS-5]=CRGB::Blue;
  pLeds[NUM_LEDS-6]=CRGB::Blue;
  pLeds[NUM_LEDS-7]=CRGB::Blue;
  pLeds[NUM_LEDS-8]=CRGB::Blue;
  pLeds[NUM_LEDS-9]=CRGB::Blue;
  pLeds[NUM_LEDS-10]=CRGB::Blue;
}

void set2Cols(CRGB *pLeds,CRGB c1, CRGB c2,int nb1)
{
    for (int i=0;i<NUM_LEDS;i++)
    {
      if (i<nb1)
      {      
       pLeds[i] = c1;
      }
      else
      {
        pLeds[i] = c2;
      }
    }
}

void setHalfAll(CRGB *pLeds,CRGB a)
{
  for (int i=0;i<NUM_LEDS;i++)
  {
    if ( (i%2) == 0 )
    {
     pLeds[i] = a;
    }
    else
    {
      pLeds[i] = CRGB::Black;
    }
  }
}

void clearAll(CRGB *pLeds)
{
  setAll(pLeds,CRGB::Black);
}

void printTime()
{
  char tmp[35];
  DateTime now = RTC.now();
  sprintf(tmp, "Time: %02d/%02d/%04d %02d:%02d:%02d",now.day() , now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.println(tmp);    
}

void init_strip_leds()
{
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_BAS>(leds_bas, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_HAUT>(leds_haut, NUM_LEDS); 
  FastLED.setBrightness(g_power);

  clearAll(leds_bas);
  clearAll(leds_haut);
  
  FastLED.show();  
}

void setup()
{
  g_power=POWER_MAX;
  bool flgNOSD=false;
  
  Serial.begin(9600);
  Serial.println("Boot");

  dht_bas.setup(DHT11_BAS_PIN, DHTesp::DHT11);
  dht_out.setup(DHT11_OUT_PIN, DHTesp::DHT11);
  pinMode(CAPOT_PIN,INPUT);
  pinMode(LED_BAS_PIN,OUTPUT);
  pinMode(MOVE_BAS,INPUT);
  pinMode(MOVE_HAUT,INPUT_PULLUP);
  pinMode(PIN_BOUTON,INPUT_PULLUP);
  pinMode(LUM_PIN,INPUT);

  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime((__DATE__), (__TIME__))); 
  printTime();

  init_strip_leds();

  delay(200);
  Serial.print("Find SD card: ");
  if (!SD.begin(PIN_SDCARD_CS))
  {
    Serial.println("[NOK]");
    flgNOSD=true;
  }  
  else
  {
    Serial.println("[OK]");
  }
  
  g_bitsSystem=SB_FIRST_CYCLE|SB_EVENT;
  if (flgNOSD)
    g_bitsSystem|=SB_NOSD;

  g_bitsInputs=0;
  g_bitsInputsFront=0;

  g_bitsOutputs=0;
  g_bitsOutputsPrev=0;

  g_in_tempBas=0;
  g_in_humBas=0;
  g_in_tempOut=0;
  g_in_humOut=0;
  g_in_lumiere=0;

  g_mode=0;

  g_tmLog_m=1;
  g_tmMoveFilterBas_s=0;
  g_tmMoveFilterHaut_s=0;
  g_tmMoveLightBas_m=0;
  g_tmShow_s=0;

  g_nbreMoveBas=0;
  g_nbreMoveHaut=0;

  t0=millis();
  t00=millis();

  initFileName();
  //logBoot();
}

void latch_value(byte *i_pOutVal,byte i_iNewValue)
{
  byte old=(*i_pOutVal);
  if ( (old!=i_iNewValue) && ( (i_iNewValue!=0) || (abs(i_iNewValue-old)<2) ) )
  {
    (*i_pOutVal)=i_iNewValue;

    if (abs(i_iNewValue-old)>2)
      g_bitsSystem|=SB_EVENT;
  }
}

void cycle_inputs()
{
  TempAndHumidity newValues = dht_bas.getTempAndHumidity();
  latch_value(&g_in_humBas,(byte)newValues.humidity);
  latch_value(&g_in_tempBas,(byte)newValues.temperature);
      
  newValues = dht_out.getTempAndHumidity();
  latch_value(&g_in_humOut,(byte)newValues.humidity);
  latch_value(&g_in_tempOut,(byte)newValues.temperature);

  byte newBitsInputs=0;
  newBitsInputs=digitalRead(CAPOT_PIN)==1?newBitsInputs|IB_CAPOT_BAS:newBitsInputs&(~IB_CAPOT_BAS);
  newBitsInputs=digitalRead(MOVE_BAS)==1?newBitsInputs|IB_MOVE_BAS:newBitsInputs&(~IB_MOVE_BAS);
  newBitsInputs=digitalRead(MOVE_HAUT)==1?newBitsInputs|IB_MOVE_HAUT:newBitsInputs&(~IB_MOVE_HAUT);
  newBitsInputs=digitalRead(PIN_BOUTON)==0?newBitsInputs|IB_BOUTON:newBitsInputs&(~IB_BOUTON);

  if (newBitsInputs!=g_bitsInputs)
    g_bitsSystem|=SB_EVENT;
    
  g_bitsInputsFront=(g_bitsInputs ^ newBitsInputs)&newBitsInputs; 
  g_bitsInputs=newBitsInputs;

  int lum=analogRead(LUM_PIN);
  g_in_lumiere=lum;

  if (g_in_lumiere<LIGHT_SEUIL_DAY)
    g_night=false;
  else if (g_in_lumiere>LIGHT_SEUIL_NIGHT)
    g_night=true;

  #ifdef FORCE_NUIT
    g_night=true;
  #endif
}

void displayMode()
{
  clearAll(leds_bas);
  clearAll(leds_haut);
  switch (g_mode)
  {
    case 0:setAll(leds_haut,CRGB::Black);break;
    case 1:setAll(leds_haut,CRGB::Blue);break;
    case 2:setAll(leds_haut,CRGB::Green);break;
    case 3:setAll(leds_haut,CRGB::Orange);break;    
    case 4:setAll(leds_haut,CRGB::Red);break;
    case 5:setAll(leds_haut,CRGB::White);break;    
  }

  g_power=POWER_MAX;
}

int getRebours(int m)
{
  int rem=60-m;
  int cur=rem*NUM_LEDS/60;
  return cur;
}

void doLEDsGivenHours()
{
      g_power=POWER_MAX;
      
      clearAll(leds_bas);
      
      DateTime now = RTC.now();
      int h=now.hour();
      int m=now.minute();
      
      if (g_night==false)
      {
        setAll(leds_haut,CRGB::Black);        
      }
      else
      {        
        if (h>=22)
        {
          set2Cols(leds_haut,CRGB::Blue,CRGB::Red,getRebours(m));
        }
        else if (h>=21)
        {
            set2Cols(leds_haut,CRGB::Violet,CRGB::Green,getRebours(m));  
        }
        else if (h>=20)
        {
            set2Cols(leds_haut,CRGB::Orange,CRGB::Violet,getRebours(m));    
            setHalfAll(leds_bas,CRGB::Green);      
        }
        else if (h>=19)
        {
            set2Cols(leds_haut,CRGB::Red,CRGB::Green,getRebours(m));       
            setHalfAll(leds_bas,CRGB::Yellow);   
        }
        else if (h>=18)
        {
            set2Cols(leds_haut,CRGB::Brown,CRGB::Red,getRebours(m));     
            setHalfAll(leds_bas,CRGB::Green);       
        }
        else if (h>=17)
        {
            set2Cols(leds_haut,CRGB::White,CRGB::Green,getRebours(m));   
            setHalfAll(leds_bas,CRGB::Pink);       
        }
        else if (h<6)
        {
          g_power=POWER_MIN;
          set2Cols(leds_haut,CRGB::Blue,CRGB::Green,getRebours(m));    
        }
        else if (h<7)
        {
            set2Cols(leds_haut,CRGB::Red,CRGB::Blue,getRebours(m));          
        }
        else if (h<8)
        {
            set2Cols(leds_haut,CRGB::Blue,CRGB::Green,getRebours(m));          
            setAll(leds_bas,CRGB::Green);
        }
        else
        {
          clearAll(leds_haut);
        }
      }  
}

void cycle_outputs()
{
  digitalWrite(LED_BAS_PIN,g_bitsOutputs&OB_LED_BAS==OB_LED_BAS?HIGH:LOW);
  g_bitsInputsFront=0;

  FastLED.setBrightness(g_power);
  FastLED.show();

  if ((g_bitsSystem&SB_EVENT)==SB_EVENT)
    display_temp_hum();
  
  g_bitsSystem=g_bitsSystem&(~SB_EVENT);

  // Affiche le mode pendant qqs secondes...
  if (g_tmShow_s>0)
  {
    if (g_tmShow_s==255)
      g_tmShow_s=0;
      
    Serial.print(".");
    displayMode();
    return;
  }

  // 0: Rien (coffre et LED blanche seulement)
  // 1: En fonction de l'heuure
  // 2: Statique 1
  // 3: Statique 2
  // 4: Auto (detection minou = bleu bas 50pc pdt 30s + rouge haut pdt 1min)
  // 5: BBR
  // 5: Tous les quarts d'heures pdt 1 min (rouge haut)
  // Gestion de l'affichage selon le mode
  switch (g_mode)
  {
    case 0:
    default:
    {
      clearAll(leds_bas);
      clearAll(leds_haut);
      break;
    }
    case 1:  // Eteint le jour, puis en fonction des heures
    {
      doLEDsGivenHours();      
      break;
    }
    
    case 2:  // Statique Rouge haut / Vert Bas
    {
      g_power=POWER_MAX;
      setAll(leds_bas,CRGB::Green);
      setAll(leds_haut,CRGB::Red);
      
      break;
    }
    
    case 3:  // Rouge haut ttes les 15 mins
    {
      g_power=POWER_MAX;
      DateTime now = RTC.now();
      if ( (now.minute()%15)==0)
      {
        setAll(leds_haut,CRGB::Red);
      }
      else
      {
        clearAll(leds_haut);
      }
      
      clearAll(leds_bas);

      break;
    }
    case 4: // Detecteur mouvement
    {
      g_power=POWER_MAX;

      // Si capot ouvert, on allume en haut en bleu
      if ( ((g_bitsInputs&IB_CAPOT_BAS)==IB_CAPOT_BAS)  )
      {          
        g_power=POWER_MIN;
        clearAll(leds_bas);
        setAll(leds_haut,CRGB::Blue);
          
        break;
      }      

      // Si ca vient de bouger, on arme les timers et on éclaire vert en bas et orange en haut
      if ( ((g_bitsInputs&IB_MOVE_BAS)==IB_MOVE_BAS) || ( g_tmMoveFilterBas_s!=0 ) )
      {
        g_power=POWER_MAX;
        setHalfAll(leds_bas,CRGB::Green);
        setAll(leds_haut,CRGB::Orange);

        g_tmMoveFilterBas_s=TM_MOVE_BAS_FILTER_S;
        g_tmMoveLightBas_m=TM_MOVE_BAS_LIGHT_M;
      }
      else
      {        
        // Ensuite on eclaire plus leger jusqu'a la fin des timers
        g_power=POWER_MIN;
        
        if (g_tmMoveLightBas_m>0)
        {
          if (g_tmMoveLightBas_m==255)
            g_tmMoveLightBas_m=0;
            
          setHalfAll(leds_bas,CRGB::Green);
          setAll(leds_haut,CRGB::Orange);
        }
        else
        {
          clearAll(leds_bas);
          clearAll(leds_haut);        
        }        
      }
      
      break;
    }
    case 5:  // Bleu / Rouge en haut et bleu en bas
    {
      g_power=POWER_MAX;
      setBBR(leds_haut);
      setAll(leds_bas,CRGB::Blue);      
    }
  }
}

void dispVal(byte val)
{
  String str=String(val);
  Serial.println(str);
}

void display_temp_hum()
{    
  Serial.print("Hum bas: ");
  Serial.print(g_in_humBas);
  Serial.println("%");
  
  Serial.print("Temp bas: ");
  Serial.print(g_in_tempBas);
  Serial.println("°");

  Serial.print("Hum ext: ");
  Serial.print(g_in_humOut);
  Serial.println("%");
  
  Serial.print("Temp ext: ");
  Serial.print(g_in_tempOut);
  Serial.println("°");  
  
  Serial.print("Lumiere:");
  Serial.println(g_in_lumiere);
  Serial.print("Nuit:");
  Serial.println(g_night);

  Serial.print("mode:");
  Serial.println(g_mode);

  Serial.print("move bas:");
  Serial.println(g_nbreMoveBas);

  Serial.print("move haut:");
  Serial.println(g_nbreMoveHaut);
}

void comptage_moves()
{
  // Comptage du nombre de moves en bas
  if ( ((g_bitsInputsFront&IB_MOVE_BAS)==IB_MOVE_BAS) && (g_tmMoveFilterBas_s==0) )
  {
    g_nbreMoveBas++;
    g_tmMoveFilterBas_s=TM_MOVE_BAS_FILTER_S;
  }

  // Comptage du nombre de moves en haut
  if ( ((g_bitsInputsFront&IB_MOVE_HAUT)==IB_MOVE_HAUT) && (g_tmMoveFilterHaut_s==0) )
  {
    g_nbreMoveHaut++;
    g_tmMoveFilterHaut_s=TM_MOVE_HAUT_FILTER_S;
  }    
}
  
void loop() 
{
  cycle_inputs();

  bool flgCapotBas=(g_bitsInputs&IB_CAPOT_BAS)==IB_CAPOT_BAS?true:false;
  bool flgLEDBas=flgCapotBas;
  g_bitsOutputs=flgLEDBas==true?g_bitsOutputs|OB_LED_BAS:g_bitsOutputs&(~OB_LED_BAS);

  if (flgCapotBas)
    g_tmMoveFilterBas_s=TM_MOVE_BAS_FILTER_S;
    
  // Sur front montant du bouton poussoir, on switche les LEDs
  if ((g_bitsInputsFront&IB_BOUTON)==IB_BOUTON)
  {
    g_mode++;
    if (g_mode>5)
      g_mode=0;

    Serial.print("\nMode:");
    Serial.println(g_mode);

    g_tmShow_s=TM_DISPLAY_MODE_S;
  }  

  comptage_moves();

  cycle_outputs();

  delay(100);  

  if ( (g_tmLog_m==255) || ((g_bitsSystem&SB_FIRST_CYCLE)==SB_FIRST_CYCLE) )
  {  
    g_tmLog_m=TM_LOG_M;
    logState();    
  }

  g_bitsSystem=g_bitsSystem&(~SB_FIRST_CYCLE);  

  long l=millis();

  // Execution toutes les secondes
  if (l-t0>1000)
  {
    t0=millis();        
    TICK_TMR(g_tmShow_s);
    TICK_TMR(g_tmMoveFilterBas_s);
    TICK_TMR(g_tmMoveFilterHaut_s);

    if (g_tmMoveFilterHaut_s==255)
      g_tmMoveFilterHaut_s=0;
    if (g_tmMoveFilterBas_s==255)
      g_tmMoveFilterBas_s=0;    
  }

  // Execution toutes les minutes
  if (l-t00>60000)
  {
    t00=millis();    
    TICK_TMR(g_tmLog_m);
    TICK_TMR(g_tmMoveLightBas_m);
  }    
}
