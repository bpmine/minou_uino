#include "ldserver.h"
#include <arduino.h>

#include <ArduinoJson.h>

CRGB off(0,0,0);
CRGB on(127,127,127);
CRGB red(127,0,0);
CRGB green(0,127,0);
CRGB blue(0,0,127);
CRGB yellow(127,127,0);
CRGB orange(127,82,0);
CRGB pink(127,9,73);
CRGB cyan(0,127,127);
CRGB violet(127,0,127);
CRGB custom(0,0,0);


void LdServer::setAll(CRGB a)
{
  for (int i=0;i<m_numLeds;i++)
  {
     m_pLeds[i] = a;
  }
}

void LdServer::clearAll(void)
{
  setAll(CRGB::Black);
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
 *       - 0: Eteint (zero)
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
void LdServer::handlerSetLeds(void)
{
  int start=-1,end=-1,r=-1,g=-1,b=-1;
  
  for (int i = 0; i < m_pServer->args(); i++) 
  {
    if (strcmp(m_pServer->argName(i).c_str(),"start")==0)
    {
      sscanf(m_pServer->arg(i).c_str(),"%d",&start);
    }
    else if (strcmp(m_pServer->argName(i).c_str(),"col")==0)
    {
      CRGB c=col(m_pServer->arg(i).c_str());
      r=c.r;
      g=c.g;
      b=c.b;      
    }
    else if (strcmp(m_pServer->argName(i).c_str(),"r")==0)
    {
      sscanf(m_pServer->arg(i).c_str(),"%d",&r);
    }
    else if (strcmp(m_pServer->argName(i).c_str(),"g")==0)
    {
      sscanf(m_pServer->arg(i).c_str(),"%d",&g);
    }
    else if (strcmp(m_pServer->argName(i).c_str(),"b")==0)
    {
      sscanf(m_pServer->arg(i).c_str(),"%d",&b);
    }
    else if (strcmp(m_pServer->argName(i).c_str(),"end")==0)
    {
      sscanf(m_pServer->arg(i).c_str(),"%d",&end);
    }
  }

  if ( (start!=-1) && (r!=-1) && (g!=-1) && (b!=-1) )
  {
    if (start>=m_numLeds)
      start=m_numLeds-1;
    
    if (end==-1)
      end=start;
  
    if (end>=m_numLeds)
      end=m_numLeds-1;

    if (end<start)
      end=start;

    for (int i=start;i<=end;i++)
    {
      m_pLeds[i]=CRGB(r,g,b);
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
    
    m_pServer->send(200, "text/plain", s);
    return;
  }
  
  m_pServer->send(400, "text/plain", "ERROR");
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
void LdServer::handlerAnim(void)
{
  if (m_pServer->args()==1)
  {
    if (strcmp(m_pServer->argName(0).c_str(),"prog")==0)
    {
      strncpy(m_pProg,m_pServer->arg(0).c_str(),m_maxSizeProg);
      m_pProg[m_maxSizeProg-1]=0;
      
      m_pServer->send(200, "text/plain", m_pProg);
      return;
    }
    else if (strcmp(m_pServer->argName(0).c_str(),"enable")==0)
    {
      if (strcmp(m_pServer->arg(0).c_str(),"1")==0)
      {
        flg_runProg=true;
        m_anim.init(m_pProg, m_maxSizeProg);
        
        m_pServer->send(200, "text/plain", "ANIM ENABLED");
        return;
      }
      else
      {
        flg_runProg=false;
        m_pServer->send(200, "text/plain", "ANIM DISABLED");
        return;
      }
    }
    else if (strcmp(m_pServer->argName(0).c_str(),"info")==0)
    {
      char str[100];
      m_anim.getDumpStr(str,100);
      m_pServer->send(200, "text/plain", str);   
      return;  
    }
  }
  
  m_pServer->send(400, "text/plain", "ERROR");  
}

/**
 * @brief Controlleur WS eteint toutes les LEDs /leds/clearall
*/
void LdServer::handlerClearAll()
{  
  clearAll();

  m_pServer->send(200, "text/plain", "OK");

  FastLED.show();
    
}

void LdServer::stopAnim(void)
{
  flg_runProg=false;
  clearAll();
}

CRGB LdServer::col(const char *strCol)
{
  if (strCol==NULL)
    return off;
  
  if (strcmp(strCol,"r")==0)
  {
    return red;
  }
  else if (strcmp(strCol,"g")==0)
  {
    return green;
  }
  else if (strcmp(strCol,"b")==0)
  {
    return blue;
  }  
  else if (strcmp(strCol,"w")==0)
  {
    return on;
  }  
  else if (strcmp(strCol,"y")==0)
  {
    return yellow;
  }  
  else if (strcmp(strCol,"o")==0)
  {
    return orange;
  }  
  else if (strcmp(strCol,"p")==0)
  {
    return pink;
  }  
  else if (strcmp(strCol,"c")==0)
  {
    return cyan;
  }  
  else if (strcmp(strCol,"v")==0)
  {
    return violet;
  }  
  else
  {
    return off;
  }
}

CRGB LdServer::rgb(unsigned long rgb)
{
  int r=(rgb&(0xFF0000)>>16);
  int g=(rgb&(0x00FF00)>>8);
  int b=rgb&(0x0000FF);
  
  custom.setRGB(r,g,b);
  return custom;  
}


extern StaticJsonDocument<1024> jstmp;

void LdServer::processCmd(const String &strCmd,char *strAnswer,int maxSize)
{
  auto error = deserializeJson(jstmp, strCmd);
  if (error) 
  {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    jstmp.clear();
    jstmp["msg"]="Erreur de parsing!";
    jstmp["result"]=0;
    return;
  }
  
  if (jstmp["cmd"]=="clrall")
  {    
    clearAll();
    Serial.println("EXEC clrall");
    jstmp["result"]=1;    
  }
  else if (jstmp["cmd"]=="setall")
  {
    Serial.println("EXEC setall");
    
    if (jstmp.containsKey("col"))
    {
      const char *strCol=jstmp["col"];
      setAll(col(strCol));
      jstmp["result"]=1;
    }
    else if (jstmp.containsKey("rgb"))
    {
      unsigned long ulCol=jstmp["rgb"];
      setAll(rgb(ulCol));      
      jstmp["result"]=1;
    }
    else
    {
      jstmp["result"]=0;
    }    
  }
  else if (jstmp["cmd"]=="set")
  {
    Serial.println("EXEC set");
    
    int start=0;
    int end=m_numLeds-1;
    CRGB c=off;

    if (jstmp.containsKey("start"))
      start=jstmp["start"];

    if (jstmp.containsKey("end"))
      end=jstmp["end"];
    
    if (jstmp.containsKey("col"))
    {
      const char *strCol=jstmp["col"];
      c=col(strCol);
    }
    if (jstmp.containsKey("rgb"))
    {
      unsigned long ulCol=jstmp["rgb"];
      c=rgb(ulCol);
    }

    if (start<0)
      start=0;
    if (end>=m_numLeds)
      end=m_numLeds-1;
    if (start>end)
      end=start;

    for (int i=start;i<=end;i++)
    {
      m_pLeds[i]=c;
    }

    FastLED.show();
    
    jstmp["result"]=1;
    jstmp["start"]=start;
    jstmp["end"]=end;    
  }
  else if (jstmp["cmd"]=="anim")
  {
    Serial.println("EXEC anim");
    
    if (jstmp.containsKey("enable"))
    {    
      if (jstmp.containsKey("prog"))
      {
        const char *strProg=jstmp["prog"];
        strncpy(m_pProg,strProg,m_maxSizeProg);
        m_pProg[m_maxSizeProg-1]=0;        
        m_anim.init(m_pProg, m_maxSizeProg);
      }
      
      flg_runProg=jstmp["enable"]==1?true:false;      
    }    
  }
  else
  {
    jstmp.clear();
    jstmp["msg"]="Unknown cmd";
    jstmp["result"]=0;
  }

  serializeJson(jstmp, strAnswer,maxSize);
}

LdServer::LdServer(ESP8266WebServer *pServer,CRGB *pLeds,int numLeds,char *pProg,int maxSizeProg,char *strName)
{
  m_pServer=pServer;
  m_pLeds=pLeds;
  m_pProg=pProg;
  strncpy(m_strName,strName,SIZE_LDSERVER_NAME);
  m_strName[SIZE_LDSERVER_NAME-1]=0;
  m_numLeds=numLeds;
  m_anim.setLeds(pLeds,numLeds);
  flg_runProg=false;
  m_maxSizeProg=maxSizeProg;
}

void LdServer::tick(void)
{
  if (flg_runProg==true)
  {
    m_anim.tick();
  }
}

void LdServer::init(void)
{
  clearAll();
}
