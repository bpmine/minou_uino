#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>
#define FASTLED_ESP8266_D1_PIN_ORDER
#include "FastLED.h"
#include "ldanim.h"

#define SIZE_LDSERVER_NAME    (10)

class LdServer
{
  private:
    ESP8266WebServer *m_pServer;
    CRGB *m_pLeds;
    int m_numLeds;
    char *m_pProg;
    int m_maxSizeProg;
    char m_strName[SIZE_LDSERVER_NAME];

    void setAll(CRGB a);
    void clearAll(void);
    LdAnim m_anim;
    bool flg_runProg;

  public:
    LdServer(ESP8266WebServer *pServer,CRGB *pLeds,int numLeds,char *pProg,int maxSizeProg,char *strName);

    void handlerSetLeds(void);
    void handlerClearAll(void);
    void handlerAnim(void);

    void tick(void);
    void stopAnim(void);

    void init(void);    
};
