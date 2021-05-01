#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>
#define FASTLED_ESP8266_D1_PIN_ORDER
#include "FastLED.h"

#define SIZE_LDSERVER_NAME    (10)

class LdServer
{
  private:
    ESP8266WebServer *m_pServer;
    CRGB *m_pLeds;
    int m_numLeds;
    char *m_pProg;
    char m_strName[SIZE_LDSERVER_NAME];

    void setAll(CRGB a);
    void clearAll(void);

  public:
    LdServer(ESP8266WebServer *pServer,CRGB *pLeds,int numLeds,char *pProg,char *strName);

    void handlerSetLeds(void);
    void handleClearAll(void);

    void init(void);    
};
