/**
 * @file cabane_minou.ino
 * 
 * Cible: NodeMCU V1.0 (ESP 12E)
 * 
 * @brief Gestion de l'éclairage de la tour de Minou
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#define ENABLE_MQTT
//#define ENABLE_OTA

#ifdef ENABLE_OTA
  #include <ArduinoOTA.h>
#endif

#include <DHTesp.h>
#include <uri/UriBraces.h>
#include "ldserver.h"

//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#include "timer.h"
#include "ldanim.h"

#include "credentials.h"

#include "move.h"

#include <ArduinoJson.h>

#ifdef ENABLE_MQTT
  #include <EspMQTTClient.h>  

  #define MQTT_TRACE_ON
  
  #define TOPIC_PREFIX  "/minou"
  #define TOPIC_CMD     "cmd"
  #define TOPIC_DATA    "data"
  #define TOPIC_LOG     "log"
  
  #define DELAY_TASK_REPORT_COMM  (3600*1000UL)
  #define DELAY_TASK_REPORT_DATA  (5*60*1000UL)
  #define DELAY_TASK_MQTT_OK      (800UL)
  
  EspMQTTClient mqttClient(
    STASSID,
    STAPSK,
    MQTT_IP,
    MQTT_LOGIN,
    MQTT_PASS,
    "minou_xxx"
  );
  
  int g_id=0;
  
#endif


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

Timer tmrTempOut=Timer(40*1000U);
Timer tmrTempBas=Timer(40*1000U);

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
int g_in_humBas=0;
int g_in_tempBas=0;
int g_in_humOut=0;
int g_in_tempOut=0;
bool g_flgCapot=false;


Move mvHaut=Move();
Move mvBas=Move();
Move mvCapot=Move();

#define SIZE_PROG (500)
char prog_haut[SIZE_PROG] = "S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*";
char prog_bas[SIZE_PROG] = "S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*";

LdServer ldsrv_bas=LdServer(&server,leds_bas,NUM_LEDS,prog_bas,SIZE_PROG,"bas");
LdServer ldsrv_haut=LdServer(&server,leds_haut,NUM_LEDS,prog_haut,SIZE_PROG,"haut");

StaticJsonDocument<1024> jstmp;
char tmpBuf[1024];

void makeJsonSensors(char *strJson,int maxSize)
{
  jstmp.clear();
  jstmp["temp_out"]=g_in_tempOut;
  jstmp["temp_bas"]=g_in_tempBas;
  jstmp["hum_out"]=g_in_humOut;
  jstmp["hum_bas"]=g_in_humBas;
  jstmp["capot"]=g_flgCapot;
  jstmp["move_haut"]=mvHaut.getCount();
  jstmp["move_bas"]=mvBas.getCount();
  jstmp["move_capot"]=mvCapot.getCount();

  serializeJson(jstmp, strJson,maxSize);
}


#ifdef ENABLE_MQTT
void sendLog(char *strMsg)
{
  char strTopicLog[50];
  sprintf(strTopicLog,"%s/%s/%03d",TOPIC_PREFIX,TOPIC_LOG,g_id);
  mqttClient.publish(strTopicLog, strMsg);
}

void sendData(char *strMsg)
{
  char strTopicData[50];
  sprintf(strTopicData,"%s/%s/%03d",TOPIC_PREFIX,TOPIC_DATA,g_id);
  mqttClient.publish(strTopicData, strMsg);
}

void taskReportComm(void)
{
  char strReport[50];
  sprintf(strReport,"comm %d",mqttClient.getConnectionEstablishedCount());
  sendLog(strReport);
  
  mqttClient.executeDelayed(DELAY_TASK_REPORT_COMM, taskReportComm);
}

void taskReportData(void)
{ 
  makeJsonSensors(tmpBuf,sizeof(tmpBuf));
  sendData(tmpBuf);
  
  mqttClient.executeDelayed(DELAY_TASK_REPORT_DATA, taskReportData);
}

void setup_mqtt(void)
{
  g_id=MQTT_ID;
  static char strName[15];
  sprintf(strName,"minou_%03d",g_id);
  mqttClient.setMqttClientName(strName);
  Serial.print("Start Mqtt ");
  Serial.print(strName);
  Serial.println("...");  

  mqttClient.enableMQTTPersistence();

  #ifdef MQTT_TRACE_ON
    mqttClient.enableDebuggingMessages();
  #endif

  mqttClient.executeDelayed(DELAY_TASK_REPORT_COMM, taskReportComm);
  mqttClient.executeDelayed(DELAY_TASK_REPORT_DATA, taskReportData);
  //client.executeDelayed(DELAY_TASK_MQTT_OK, taskBlinkLed);  
}

void onReceiveCmd(const String &payload)
{
  Serial.print("Cmd Recue: ");
  Serial.println(payload);  
}

void onReceiveCmdHaut(const String &payload)
{
  Serial.print("Cmd LEDs Haut Recue: ");
  Serial.println(payload);

  ldsrv_haut.processCmd(payload,tmpBuf,sizeof(tmpBuf));
}

void onReceiveCmdBas(const String &payload)
{  
  Serial.print("Cmd LEDs Bas Recue: ");
  Serial.println(payload);

  ldsrv_bas.processCmd(payload,tmpBuf,sizeof(tmpBuf));  
}

void onConnectionEstablished()
{
  Serial.println("Connection Mqtt.");

  char strTopicCmd[50];
  sprintf(strTopicCmd,"%s/%s/%03d/leds/haut",TOPIC_PREFIX,TOPIC_CMD,g_id);  
  mqttClient.subscribe(strTopicCmd,onReceiveCmdHaut);
  sprintf(strTopicCmd,"%s/%s/%03d/leds/bas",TOPIC_PREFIX,TOPIC_CMD,g_id);  
  mqttClient.subscribe(strTopicCmd,onReceiveCmdBas);
  sprintf(strTopicCmd,"%s/%s/%03d/leds",TOPIC_PREFIX,TOPIC_CMD,g_id);  
  mqttClient.subscribe(strTopicCmd,onReceiveCmd);

  sendLog("MQTT Connected");
}

#endif

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
  jstmp.clear();
  jstmp["enabled"]=wdg.isRunning();
  jstmp["duration"]=wdg.getDuration_ms();
  jstmp["remaining"]=wdg.getRemaining_ms();
  serializeJson(jstmp, tmpBuf,sizeof(tmpBuf));  
  
  server.send(200, "text/plain", tmpBuf);
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
    ldsrv_haut.handlerClearAll();
    server.send(200, "text/plain", "OK");
  }
  if (lds=="bas")
  {
    ldsrv_bas.handlerClearAll();
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
  jstmp.clear();
  jstmp[0]["name"]="haut";
  jstmp[0]["description"]="Gestion des LEDs du haut de la cabane du minou";
  jstmp[0]["num_leds"]=NUM_LEDS;
  jstmp[1]["name"]="bas";
  jstmp[1]["description"]="Gestion des LEDs du bas de la cabane du minou";
  jstmp[1]["num_leds"]=NUM_LEDS;
  serializeJson(jstmp, tmpBuf,sizeof(tmpBuf));
    
  server.send(200, "text/plain", tmpBuf);
}

void handleSensors(void)
{  
  char strJson[500]="";

  makeJsonSensors(strJson,500);
  
  server.send(200, "text/plain", strJson);  
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
void handleAnim()
{
  String lds=server.pathArg(0);
  if (lds=="haut")
  {
    ldsrv_haut.handlerAnim();
  }  
  if (lds=="bas")
  {
    ldsrv_bas.handlerAnim();
  }  
  else
  {
    server.send(400, "text/plain", "ERROR UNKNOWN LED SET");
    return;
  }    
}

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

void measureTempHum(DHTesp &dht,int *temp_dg,int *hum_pc,Timer *tmr)
{
  bool flgFound=false;
  for (int i=4;i>0;i--)
  {
    TempAndHumidity newValues = dht.getTempAndHumidity();
  
    if (dht.getStatus()==DHTesp::ERROR_NONE)
    {
      *temp_dg=newValues.temperature;
      *hum_pc=newValues.humidity;  
      tmr->start();
    }

    delay(20);
  }

  if (tmr->tick()==true)
  {
    *temp_dg=-255;
    *hum_pc=-255;
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
  mvCapot.begin(PIN_CAPOT);

  dht_bas.setup(PIN_DHT11_BAS, DHTesp::DHT11);
  dht_out.setup(PIN_DHT11_OUT, DHTesp::DHT11);

  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_BAS>(leds_bas, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, PIN_DATA_LED_HAUT>(leds_haut, NUM_LEDS); 
  FastLED.setBrightness(g_power);

  setAll(leds_bas,CRGB::Red);
  setAll(leds_haut,CRGB::Blue);
  FastLED.show();
  Serial.println("Leds ok");
  delay(500);
  
  #ifndef ENABLE_MQTT
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
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
  
    if (MDNS.begin("esp8266")) 
    {
      Serial.println("MDNS responder started");
    }
  #else
    setup_mqtt();  
  #endif

  clearAll(leds_bas);
  clearAll(leds_haut);
  FastLED.show();

  ldsrv_haut.init();
  ldsrv_bas.init();

  server.on(UriBraces("/leds/{}/clearall"), handleClearAll);
  server.on(UriBraces("/leds/{}/anim"), handleAnim); 
  server.on(UriBraces("/leds/{}/set"), handleSetLeds);
  
  server.on("/leds/info", handleInfo);
  server.on("/leds/wdg", handleWdg);
  server.on("/leds/wdg/info", handleWdgInfo);  
  
  server.on("/sensors", handleSensors);  

  server.onNotFound(handleNotFound);
  server.enableCORS(true);

  server.begin();
  Serial.println("HTTP server started");

  wdg.stop();

  #ifdef ENABLE_OTA
    ArduinoOTA.setHostname(OTA_NAME);
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
  #endif
}

/**
 * @loop Boucle principale de l'arduino
*/
void test_loop(void) 
{
  static int cnt=200;
  static int tst_etat=0;

  measureTempHum(dht_bas,&g_in_tempBas,&g_in_humBas,&tmrTempBas);
  measureTempHum(dht_out,&g_in_tempOut,&g_in_humOut,&tmrTempOut);

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

  if (mvCapot.tick()==true)
  {
    Serial.println("Manipulation capot");
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

  #ifdef ENABLE_OTA
    ArduinoOTA.handle();
  #endif
}

void wdgSwitchAllOff()
{  
  clearAll(leds_bas);
  clearAll(leds_haut);
}

void loop_app()
{
  #ifdef ENABLE_MQTT
    mqttClient.loop();
  #endif
  
  measureTempHum(dht_bas,&g_in_tempBas,&g_in_humBas,&tmrTempBas);
  measureTempHum(dht_out,&g_in_tempOut,&g_in_humOut,&tmrTempOut);

  g_flgCapot=(digitalRead(PIN_CAPOT)==HIGH)?true:false;

  //if (wdg.isRunning()==false)
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

    if (mvCapot.tick()==true)
    {
      Serial.println("Manipulation capot");
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

  ldsrv_bas.tick();
  ldsrv_haut.tick();
  
  if (wdg.tick()==true)
  {
    wdgSwitchAllOff();    
  }

  FastLED.show();

  #ifdef ENABLE_OTA
    ArduinoOTA.handle();
  #endif  

  /// Temoin LED blanche du bas allumée quand capot ouvert
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
