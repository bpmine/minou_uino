/**
 * @file cabane_minou.ino
 * 
 * @brief Gestion de l'éclairage de la tour de Minou
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "DHTesp.h"
#include <uri/UriBraces.h>
#include "ldserver.h"

//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#include "timer.h"
#include "ldanim.h"

#include "wifi_code.h"

#include "move.h"


//#define PIN_BUTTON        D8
#define PIN_DARK          A0

#define PIN_MOVE_HAUT     D1
#define PIN_DATA_LED_BAS  D2 
#define PIN_LED_BAS       D4
#define PIN_CAPOT         D5
#define PIN_DATA_LED_HAUT D6
#define PIN_DHT11_BAS     D7

#define PIN_MOVE_BAS      9   /// Pullup obligatoire sur circuit
#define PIN_DHT11_OUT     10


const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

Timer wdg=Timer(3600*1000U);
Timer mvtmr=Timer(30000U);


/**
 * @brief Configuration des bandeau de LEDs
 * @{
*/
#define POWER_MAX         80
#define POWER_MIN         50
#define NUM_LEDS          26

int g_power;
CRGB leds_bas[NUM_LEDS];
CRGB leds_haut[NUM_LEDS];
/**
 * @}
*/


DHTesp dht_bas;
DHTesp dht_out;
byte g_in_humBas=0;
byte g_in_tempBas=0;
byte g_in_humOut=0;
byte g_in_tempOut=0;
bool g_flgCapot=false;

int g_darkness=0;

Move mvHaut=Move();
Move mvBas=Move();

#define SIZE_PROG (500)
char prog_haut[SIZE_PROG] = "S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*";
char prog_bas[SIZE_PROG] = "S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*";

LdServer ldsrv_bas=LdServer(&server,leds_bas,NUM_LEDS,prog_bas,"bas");
LdServer ldsrv_haut=LdServer(&server,leds_haut,NUM_LEDS,prog_haut,"haut");

/**
 * @brief Allume toutes les LEDs avec la couleur fournie
 * @param pLeds Buffer des LEDs
 * @param a Couleur
*/
void setAll(CRGB *pLeds,CRGB a)
{
  for (int i=0;i<NUM_LEDS;i++)
  {
     pLeds[i] = a;
  }
}

/**
 * @brief Eteint toutes les LEDs
 * @param pLeds Buffer des LEDs 
*/
void clearAll(CRGB *pLeds)
{
  setAll(pLeds,CRGB::Black);
}

/**
 * @brief Controleur WS /leds/wdg/info
 * 
 * Retourne les informations du watchdog
 * 
 * {
 *    "enabled":true,
 *    "delay":456,
 *    "elapsed":15
 * }
 * 
*/
void handleWdgInfo()
{ 
  String s="{\"enabled\"=\"";
  s+=wdg.isRunning();
  s+="\", \"duration\"=\"";
  s+=wdg.getDuration_ms()/1000U;
  s+="\", \"remaining\"=\"";
  s+=wdg.getRemaining_ms()/1000U;  
  s+="\"}";
  
  server.send(200, "text/plain", s);
}

/**
 * @brief Controleur WS /leds/wdg
 * 
 * Pilotage du watchdog
 * 
 *   - /leds/wdg => Rearme le watchdog. Retourne WDG OK.
 *   - /leds/wdg?enable=1 => Active le watchdog. Retourne WDG ENABLED ou WDG DISABLED
 *   - /leds/wdg?delay=456 => Configure le delai du watchdog (en s). Retourne WDG DELAY 456.
*/
void handleWdg()
{
  if (server.args()==0)
  {
    if (wdg.isRunning()==true)
      wdg.start();
      
    server.send(200, "text/plain", "WDG OK");
    
    return;
  }
  else if (server.args()==1)
  {
    if (strcmp(server.argName(0).c_str(),"enable")==0)
    {
      if (strcmp(server.arg(0).c_str(),"1")==0)
      {
        wdg.start();
        server.send(200, "text/plain", "WDG ENABLED");
        return;
      }
      else
      {
        wdg.stop();
        server.send(200, "text/plain", "WDG DISABLED");
        return;
      }
    }
    else if (strcmp(server.argName(0).c_str(),"delay")==0)
    {
      long delay_s=0;
      if (sscanf(server.arg(0).c_str(),"%ld",&delay_s)==1)
      {
        unsigned long delay_ms=delay_s*1000L;

        wdg.setDuration(delay_ms);
        wdg.start();
        
        String s="WDG DELAY ";
        s+=delay_s;
        s+=" seconds";
        server.send(200, "text/plain", s);
        return;
      }
    }
  }

  server.send(400, "text/plain", "ERROR");
}

/**
 * @brief Controlleur WS eteint toutes les LEDs /leds/clearall
*/
void handleClearAll()
{  
  String lds=server.pathArg(0);
  if (lds=="haut")
  {
    ldsrv_haut.handleClearAll();
    server.send(200, "text/plain", "OK");
  }
  if (lds=="bas")
  {
    ldsrv_bas.handleClearAll();
    server.send(200, "text/plain", "OK");
  }
  else
  {
    server.send(400, "text/plain", "ERROR BAD PARAMETER");    
  }
  FastLED.show();
}

/**
 * @brief Controleur /leds/set
*/
void handleSetLeds()
{
  //if (wdg.isRunning()==true)
  wdg.start();

  String lds=server.pathArg(0);
  if (lds=="haut")
  {
    ldsrv_haut.handlerSetLeds();
  }  
  if (lds=="bas")
  {
    ldsrv_bas.handlerSetLeds();
  }  
  else
  {
    server.send(400, "text/plain", "ERROR UNKNOWN LED SET");
    return;
  }
}


/**
 * @brief Controlleur WS informations
 * 
 * Retourne les informations sur le noeud
*/
void handleInfo()
{      
  String s="[{";
  s+="'name':'haut',";
  s+="'description':'Gestion des LEDs du haut de la cabane du minou'";
  s+="'num_leds':'";
  s+=NUM_LEDS;
  s+="'";
  s+="}";
  s+="',{name':'bas',";
  s+="'description':'Gestion des LEDs du bas de la cabane du minou'";
  s+="'num_leds':'";
  s+=NUM_LEDS;
  s+="'";
  s+="}]";
  
  server.send(200, "text/plain", s);
}

void handleSensors(void)
{
  String s="{";
  s+="'temp_out':";
  s+=g_in_tempOut;
  s+=",'temp_bas':";
  s+=g_in_tempBas;
  s+=",'hum_out':";
  s+=g_in_humOut;
  s+=",'hum_bas':";
  s+=g_in_humBas;
  s+=",'capot':";
  s+=g_flgCapot;
  s+=",'darkness':";
  s+=g_darkness;
  s+=",'move_haut':";
  s+=mvHaut.getCount();
  s+=",'move_bas':";
  s+=mvBas.getCount();
  s+="}";  
  
  server.send(200, "text/plain", s);  
}

/**
 * @brief Controlleur WS pour gerer les programmes MACRO d'animation des LEDs
 * 
 * On envoi une chaine contenant le programme MACRO
 * 
 * - /leds/anim?prog="S26E33LrOS1E23..." => Modifie le programme en cours. Retourne un echo du programme si reussite.
 * - /leds/anim?enable=1 => Active l'animation. Retourne ANIM ENABLED ou ANIM DISABLED
 * - /leds/anim?info => Retourne la configuration de l'animation
*/
/*void handleAnim()
{
  if (server.args()==1)
  {
    if (strcmp(server.argName(0).c_str(),"prog")==0)
    {
      strncpy(prog,server.arg(0).c_str(),SIZE_PROG);
      prog[SIZE_PROG-1]=0;
      
      server.send(200, "text/plain", prog);
      return;
    }
    else if (strcmp(server.argName(0).c_str(),"enable")==0)
    {
      if (strcmp(server.arg(0).c_str(),"1")==0)
      {
        flg_runProg=true;
        anim.init(prog, strlen(prog));
        
        server.send(200, "text/plain", "ANIM ENABLED");
        return;
      }
      else
      {
        flg_runProg=false;
        server.send(200, "text/plain", "ANIM DISABLED");
        return;
      }
    }
    else if (strcmp(server.argName(0).c_str(),"info")==0)
    {
      char str[100];
      anim.getDumpStr(str,100);
      server.send(200, "text/plain", str);   
      return;  
    }
  }
  server.send(400, "text/plain", "ERROR");     
}*/

/**
 * @brief Controleur WS en cas d'erreur
*/
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void latch_value(byte *i_pOutVal,byte i_iNewValue)
{
  byte old=(*i_pOutVal);
  if ( (old!=i_iNewValue) && ( ((i_iNewValue!=0) && (i_iNewValue!=255) ) || (abs(i_iNewValue-old)<2) ) )
  {
    (*i_pOutVal)=i_iNewValue;
  }
}



/**
 * @brief Setup d'initialisation de l'arduino
*/
void setup(void)
{
  g_power=POWER_MAX;
  
  Serial.begin(115200);
  Serial.println("BOOT");

  pinMode(PIN_LED_BAS, OUTPUT);
  digitalWrite(PIN_LED_BAS, LOW);

  pinMode(PIN_DARK, INPUT);

  pinMode(PIN_MOVE_BAS, INPUT);
  pinMode(PIN_MOVE_HAUT, INPUT);
  mvHaut.begin(PIN_MOVE_HAUT);
  mvBas.begin(PIN_MOVE_BAS);

  pinMode(PIN_DHT11_BAS, INPUT);
  pinMode(PIN_DHT11_OUT, INPUT);
  
  pinMode(PIN_CAPOT, INPUT);

  dht_bas.setup(PIN_DHT11_BAS, DHTesp::DHT11);
  dht_out.setup(PIN_DHT11_OUT, DHTesp::DHT11);

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_BAS>(leds_bas, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_HAUT>(leds_haut, NUM_LEDS); 
  FastLED.setBrightness(g_power);

  setAll(leds_bas,CRGB::Red);
  setAll(leds_haut,CRGB::Blue);
  FastLED.show();
  delay(2000);
  
  clearAll(leds_bas);
  clearAll(leds_haut);
  FastLED.show();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) 
  {
    Serial.println("MDNS responder started");
  }

  ldsrv_bas.init();

  server.on(UriBraces("/leds/{}/clearall"), handleClearAll);
  //server.on(UriBraces("/leds/{}/anim"), handleAnim); 
  server.on(UriBraces("/leds/{}/set"), handleSetLeds);
  
  server.on("/leds/info", handleInfo);
  server.on("/leds/wdg", handleWdg);
  server.on("/leds/wdg/info", handleWdgInfo);  
  
  server.on("/sensors", handleSensors);  

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  wdg.stop();

  //ArduinoOTA.setHostname("CabaneMinou");
  ArduinoOTA.setHostname(OTA_NAME);

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

/**
 * @loop Boucle principale de l'arduino
*/
void test_loop(void) 
{
  static int cnt=200;
  static int tst_etat=0;

  TempAndHumidity newValues = dht_bas.getTempAndHumidity();
  latch_value(&g_in_humBas,(byte)newValues.humidity);
  latch_value(&g_in_tempBas,(byte)newValues.temperature);
  
  newValues = dht_out.getTempAndHumidity();
  latch_value(&g_in_humOut,(byte)newValues.humidity);
  latch_value(&g_in_tempOut,(byte)newValues.temperature);

  g_darkness=analogRead(PIN_DARK);
  Serial.println(g_darkness);

  delay(10);
  
  if ( (digitalRead(PIN_CAPOT)==HIGH) )
  {
    digitalWrite(PIN_LED_BAS,HIGH);
  }
  else
  {
    digitalWrite(PIN_LED_BAS,LOW);
  }

  if (mvHaut.tick()==true)
  {
    Serial.println("Move Haut");
  }
  
  if (mvBas.tick()==true)
  {
    Serial.println("Move Bas");
  }

  FastLED.show();

  cnt--;
  if (cnt<=0)
  {
    cnt=400;
    if (tst_etat==0)
    {
      setAll(leds_bas,CRGB::Red);
      setAll(leds_haut,CRGB::Blue);
      tst_etat=1;

      Serial.print("Hum bas:");
      Serial.println(g_in_humBas);
      Serial.print("Tmp bas:");
      Serial.println(g_in_tempBas);

      Serial.print("Hum out:");
      Serial.println(g_in_humOut);
      Serial.print("Tmp out:");
      Serial.println(g_in_tempOut);

      Serial.print("Dark:");
      Serial.println(g_darkness);
      
      Serial.print("Move Bas:");
      Serial.println(mvBas.getCount());
      Serial.print("Move Haut:");
      Serial.println(mvHaut.getCount());

      Serial.println("");
    }
    else    
    {
      clearAll(leds_bas);
      clearAll(leds_haut);
      tst_etat=0;
    }    
  }
}

void wdgSwitchAllOff()
{  
  clearAll(leds_bas);
  clearAll(leds_haut);
}

void loop_app()
{
  TempAndHumidity newValues = dht_bas.getTempAndHumidity();
  latch_value(&g_in_humBas,(byte)newValues.humidity);
  latch_value(&g_in_tempBas,(byte)newValues.temperature);
  
  newValues = dht_out.getTempAndHumidity();
  latch_value(&g_in_humOut,(byte)newValues.humidity);
  latch_value(&g_in_tempOut,(byte)newValues.temperature);

  g_flgCapot=(digitalRead(PIN_CAPOT)==HIGH)?true:false;

  g_darkness=analogRead(PIN_DARK);

  if (wdg.isRunning()==false)
  {
    if (mvHaut.tick()==true)
    {
      mvtmr.start();
      setAll(leds_haut,CRGB::Green);
      Serial.println("Move Haut");
    }
  
    if (mvBas.tick()==true)
    {
      mvtmr.start();
      setAll(leds_haut,CRGB::Red);
      Serial.println("Move Bas");
    }
  }    

  if (mvtmr.tick()==true)
  { 
      Serial.println("Timer move off");     
      clearAll(leds_haut);
      clearAll(leds_bas);
  }
  
  server.handleClient();
  MDNS.update();

  /*if (flg_runProg==true)
  {
    anim.tick();
    delay(20);
  }*/

  if (wdg.tick()==true)
  {
    wdgSwitchAllOff();    
  }

  FastLED.show();

  ArduinoOTA.handle();

  if ( g_flgCapot==true )
  {
    digitalWrite(PIN_LED_BAS,HIGH);
  }
  else
  {
    digitalWrite(PIN_LED_BAS,LOW);
  }  
}

void loop()
{
  loop_app();
  //test_loop();
}
