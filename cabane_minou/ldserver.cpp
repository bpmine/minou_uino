#include "ldserver.h"
#include <arduino.h>



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
      if (strcmp(m_pServer->arg(i).c_str(),"r")==0)
      {
        r=127;
        g=0;
        b=0;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"g")==0)
      {
        r=0;
        g=127;
        b=0;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"b")==0)
      {
        r=0;
        g=0;
        b=127;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"w")==0)
      {
        r=127;
        g=127;
        b=127;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"0")==0)
      {
        r=0;
        g=0;
        b=0;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"y")==0)
      {
        r=127;
        g=127;
        b=0;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"o")==0)
      {
        r=127;
        g=82;
        b=0;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"p")==0)
      {
        r=127;
        g=9;
        b=73;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"c")==0)
      {
        r=0;
        g=127;
        b=127;    
      }
      else if (strcmp(m_pServer->arg(i).c_str(),"v")==0)
      {
        r=127;
        g=0;
        b=127;    
      }
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
 * @brief Controlleur WS eteint toutes les LEDs /leds/clearall
*/
void LdServer::handleClearAll()
{  
  clearAll();

  m_pServer->send(200, "text/plain", "OK");

  FastLED.show();
    
}

LdServer::LdServer(ESP8266WebServer *pServer,CRGB *pLeds,int numLeds,char *pProg,char *strName)
{
  m_pServer=pServer;
  m_pLeds=pLeds;
  m_pProg=pProg;
  strncpy(m_strName,strName,SIZE_LDSERVER_NAME);
  m_strName[SIZE_LDSERVER_NAME-1]=0;
  m_numLeds=numLeds;
}

void LdServer::init(void)
{
  clearAll();
}
