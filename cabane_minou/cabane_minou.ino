/**
 * @file cabane_minou.ino
 * 
 * @brief Gestion de l'éclairage de la tour de Minou
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//#include <Wire.h>

//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"

#include "ldanim.h"

#include "wifi_code.h"

//#define PIN_BUTTON        D8
#define PIN_LUM           A0

#define PIN_MOVE_HAUT     D1
#define PIN_DATA_LED_BAS  D2 
#define PIN_LED_BAS       D4
#define PIN_CAPOT         D5
#define PIN_DATA_LED_HAUT D6
#define PIN_DHT11_BAS     D7

#define PIN_MOVE_BAS      9  //D8  /// Pullup obligatoire sur circuit
#define PIN_DHT11_OUT     10




const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);


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

/**
 * @brief Gestion du watchdog
 * @{
*/
bool g_flgWdg=true;
long g_t0_ms=0;
long g_delay_ms=3600L*1000L;
/**
 * @}
*/

#define SIZE_PROG (500)
char prog[SIZE_PROG] = "S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*";
bool flg_runProg=false;
LdAnim anim = LdAnim(leds_haut, NUM_LEDS);

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
 * @brief Controleur WS /leds/write
 * 
 * Ecrit toutes les LEDS
 * La data dans le post est une suite d'octets codes en hexadecimal
 * Trame: AA[Taille][R0][G0][B0]...[Rn][Gn][Bn]
 * 
 * Retourne "OK" si pris en compte
 * 
 * Exemple: AA06000000FF0000 => LED 0 eteinte et LED 1 en rouge
*/
void handleWrite()
{  
  if (g_flgWdg==true)
    g_t0_ms=millis();
  
  if (server.hasArg("plain")== false)
  {  
    server.send(200, "text/plain", "Body not received");
    return;
  }

  const char *pCmd=server.arg("plain").c_str();

  int tmp=0;  
  sscanf(pCmd,"%02x",&tmp);
  if (tmp!=0xAA)
  {  
    server.send(200, "text/plain", "Bad magic");
    return;
  }

  int len=0;
  sscanf(pCmd+2,"%02x",&len);
  Serial.print("Len:");
  Serial.println(len);
  
  for (int i=0;i<len;i++)
  {
    if (i>NUM_LEDS)
      break;
      
    int r,g,b;
    char strVals[10];
    memcpy(strVals,pCmd+4+(i*6),6);
    strVals[6]=0;
    Serial.println(strVals);

    if (sscanf(strVals,"%02x%02x%02x",&r,&g,&b)==3)
    {
      Serial.print(i);
      Serial.print(" ");
      Serial.print("RGB: ");
      Serial.print(r);
      Serial.print(" ");
      Serial.print(g);
      Serial.print(" ");
      Serial.println(b);

      leds_haut[i]=CRGB(r,g,b);
    }
  }
  
  FastLED.show();
  
  server.send(200, "text/plain", "OK");
}

/**
 * @brief Controleur /leds/set
 * 
 * Parametres de la requete (dans l'URL):
 *   - start: première LED de la selection
 *   - end: derniere LED de la selection
 *   - col: Couleur a appliquer 
 *       - g: Vert
 *       - b: Bleu
 *       - w: Blanc
 *       - r: Rouge
 *       - 0: Etaint (zero)
 *       - y: Jaune
 *       - o: Orange
 *       - p: Rose
 *       - c: Cyan
 *       - v: Violet
 *   - r: Composante rouge
 *   - g: composante verte
 *   - b: composante bleue
 *   
 *   RETOURNE un JSON avec les valeurs appliquees en cas de reussite sinon ERROR
 *   
 *   Exemples: 
 *     /leds/set?start=0&end=33&col=r  => les LEDs 0 a 33 en rouge
 *     /leds/set?start=0&end=15&r=0&g=127&b=0  => les LEDs 0 a 15 en vert
*/
void handleSetLeds()
{
  if (g_flgWdg==true)
    g_t0_ms=millis();

  int start=-1,end=-1,r=-1,g=-1,b=-1;
  
  for (int i = 0; i < server.args(); i++) 
  {
    if (strcmp(server.argName(i).c_str(),"start")==0)
    {
      sscanf(server.arg(i).c_str(),"%d",&start);
    }
    else if (strcmp(server.argName(i).c_str(),"col")==0)
    {
      if (strcmp(server.arg(i).c_str(),"r")==0)
      {
        r=127;
        g=0;
        b=0;    
      }
      else if (strcmp(server.arg(i).c_str(),"g")==0)
      {
        r=0;
        g=127;
        b=0;    
      }
      else if (strcmp(server.arg(i).c_str(),"b")==0)
      {
        r=0;
        g=0;
        b=127;    
      }
      else if (strcmp(server.arg(i).c_str(),"w")==0)
      {
        r=127;
        g=127;
        b=127;    
      }
      else if (strcmp(server.arg(i).c_str(),"0")==0)
      {
        r=0;
        g=0;
        b=0;    
      }
      else if (strcmp(server.arg(i).c_str(),"y")==0)
      {
        r=127;
        g=127;
        b=0;    
      }
      else if (strcmp(server.arg(i).c_str(),"o")==0)
      {
        r=127;
        g=82;
        b=0;    
      }
      else if (strcmp(server.arg(i).c_str(),"p")==0)
      {
        r=127;
        g=9;
        b=73;    
      }
      else if (strcmp(server.arg(i).c_str(),"c")==0)
      {
        r=0;
        g=127;
        b=127;    
      }
      else if (strcmp(server.arg(i).c_str(),"v")==0)
      {
        r=127;
        g=0;
        b=127;    
      }
    }
    else if (strcmp(server.argName(i).c_str(),"r")==0)
    {
      sscanf(server.arg(i).c_str(),"%d",&r);
    }
    else if (strcmp(server.argName(i).c_str(),"g")==0)
    {
      sscanf(server.arg(i).c_str(),"%d",&g);
    }
    else if (strcmp(server.argName(i).c_str(),"b")==0)
    {
      sscanf(server.arg(i).c_str(),"%d",&b);
    }
    else if (strcmp(server.argName(i).c_str(),"end")==0)
    {
      sscanf(server.arg(i).c_str(),"%d",&end);
    }
  }

  if ( (start!=-1) && (r!=-1) && (g!=-1) && (b!=-1) )
  {
    if (start>=NUM_LEDS)
      start=NUM_LEDS-1;
    
    if (end==-1)
      end=start;
  
    if (end>=NUM_LEDS)
      end=NUM_LEDS-1;

    if (end<start)
      end=start;

    for (int i=start;i<=end;i++)
    {
      leds_haut[i]=CRGB(r,g,b);
    }

    FastLED.show();

    String s="{";
    s+="start=";
    s+=start;
    s+=", end=";
    s+=end;
    s+=", rgb=\"(";
    s+=r;
    s+=",";
    s+=g;
    s+=",";
    s+=b;
    s+=")\"";
    s+="}";
    
    server.send(200, "text/plain", s);
    return;
  }
  
  server.send(400, "text/plain", "ERROR");
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
  long delta=(millis()-g_t0_ms)/1000;
  
  String s="{\"enabled\"=\"";
  s+=g_flgWdg;
  s+="\", \"delay\"=\"";
  s+=(g_delay_ms/1000);
  s+="\", \"elapsed\"=\"";
  s+=delta;  
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
    server.send(200, "text/plain", "WDG OK");
    return;
  }
  else if (server.args()==1)
  {
    if (strcmp(server.argName(0).c_str(),"enable")==0)
    {
      if (strcmp(server.arg(0).c_str(),"1")==0)
      {
        g_flgWdg=true;
        g_t0_ms=millis();
        server.send(200, "text/plain", "WDG ENABLED");
        return;
      }
      else
      {
        g_flgWdg=false;
        server.send(200, "text/plain", "WDG DISABLED");
        return;
      }
    }
    else if (strcmp(server.argName(0).c_str(),"delay")==0)
    {
      long delay_s=0;
      if (sscanf(server.arg(0).c_str(),"%ld",&delay_s)==1)
      {
        g_flgWdg=true;
        g_delay_ms=delay_s*1000L;
        g_t0_ms=millis();
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
  clearAll(leds_haut);
  clearAll(leds_bas);
  FastLED.show();
    
  server.send(200, "text/plain", "OK");
}

/**
 * @brief Controlleur WS informations
 * 
 * Retourne les informations sur le noeud
*/
void handleInfo()
{      
  String s="{";
  s+="'name':'LedsTourMinou',";
  s+="'description':'Gestion des LEDs de la tour du minou'";
  s+="'num_leds':'";
  s+=NUM_LEDS;
  s+="'";
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
void handleAnim()
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

  pinMode(PIN_LUM, INPUT);
  //pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_DHT11_OUT, INPUT_PULLUP);

  pinMode(PIN_MOVE_BAS, INPUT_PULLUP);
  pinMode(PIN_MOVE_HAUT, INPUT_PULLUP);
  pinMode(PIN_DHT11_BAS, INPUT_PULLUP);
  pinMode(PIN_CAPOT, INPUT_PULLUP);

  //Wire.begin();

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

/*  server.on("/leds/write", handleWrite);
  server.on("/leds/clearall", handleClearAll);
  server.on("/leds/info", handleInfo);
  server.on("/leds/set", handleSetLeds);
  server.on("/leds/wdg", handleWdg);
  server.on("/leds/wdg/info", handleWdgInfo);  
  server.on("/leds/anim", handleAnim); 
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");  */

  g_t0_ms=millis();
}

/**
 * @loop Boucle principale de l'arduino
*/
void loop(void) 
{
  delay(10);
  if ( (digitalRead(PIN_CAPOT)==0) )
  {
    digitalWrite(PIN_LED_BAS,HIGH);
  }
  else
  {
    digitalWrite(PIN_LED_BAS,LOW);
  }

  if (digitalRead(PIN_DHT11_OUT)==LOW)
  {
    Serial.println("DHT11 OUT");
    delay(1000);
  }
  
  if (digitalRead(PIN_DHT11_BAS)==LOW)
  {
    Serial.println("DHT11 BAS");
    delay(1000);
  }

  if (digitalRead(PIN_MOVE_BAS)==LOW)
  {
    Serial.println("MOVE BAS");
    delay(1000);
  }

  if (digitalRead(PIN_MOVE_HAUT)==LOW)
  {
    Serial.println("MOVE HAUT");
    delay(1000);
  }

  if (digitalRead(PIN_CAPOT)==LOW)
  {
    Serial.println("CAPOT");
    delay(100);
  }
  if (digitalRead(PIN_BUTTON)==LOW)
  {
    Serial.println("BUTTON");
    delay(000);
  }

  /*server.handleClient();
  MDNS.update();

  if (flg_runProg==true)
  {
    anim.tick();
    delay(20);
  }

  if (g_flgWdg==true)
  {
    long t=millis();
    long delta=0;
    if (t>g_t0_ms)
      delta=t-g_t0_ms;
    else
      delta=0xFFFFFFFF-g_t0_ms+t;
    
    if (delta>g_delay_ms)
    {
      g_t0_ms=millis();
      clearAll(leds);
      flg_runProg=false;
      FastLED.show();
    }    
  }*/
}
