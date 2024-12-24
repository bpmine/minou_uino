#include "api.h"
#include "scheduler.h"
#include "pins.h"
#include "mem.h"

#include <arduino.h>
#include "ldanim.hpp"
#include "dhtval.hpp"

extern Scheduler scheduler;
extern Mem mem;
extern LdStrip<PIN_OUT_LEDS_BAS, PIN_OUT_5V_LEDS_BAS, NB_LEDS_CABANE> ldBas;
extern LdStrip<PIN_OUT_LEDS_HAUT, PIN_OUT_5V_LEDS_HAUT, NB_LEDS_CABANE> ldHaut;
extern LdStrip<PIN_OUT_LEDS_TOUR, PIN_OUT_5V_LEDS_TOUR, NB_LEDS_TOUR> ldTour;
extern LdStrip<PIN_OUT_LEDS_RES, PIN_OUT_5V_LEDS_RES, NB_LEDS_TOUR> ldRes;

extern int g_mode;
extern int g_aff;

extern DhtVal g_temp_ext;
extern DhtVal g_hum_ext;
extern DhtVal g_temp_bas;
extern DhtVal g_hum_bas;


bool api_strip_getpwr_bas(void)
{
  return digitalRead(PIN_OUT_5V_LEDS_BAS)==HIGH?true:false;
}

bool api_strip_getpwr_haut(void)
{
  return digitalRead(PIN_OUT_5V_LEDS_HAUT)==HIGH?true:false;
}

bool api_strip_getpwr_tour(void)
{
  return digitalRead(PIN_OUT_5V_LEDS_TOUR)==HIGH?true:false;
}

bool api_strip_getpwr_res(void)
{
  return digitalRead(PIN_OUT_5V_LEDS_RES)==HIGH?true:false;
}

bool api_strip_getpwr_serial(void)
{
  return digitalRead(PIN_OUT_5V_SER)==HIGH?true:false;
}


void api_strip_setpwr_bas(bool en)
{
  digitalWrite(PIN_OUT_5V_LEDS_BAS,en?HIGH:LOW);
}

void api_strip_setpwr_haut(bool en)
{
  digitalWrite(PIN_OUT_5V_LEDS_HAUT,en?HIGH:LOW);
}

void api_strip_setpwr_tour(bool en)
{
  digitalWrite(PIN_OUT_5V_LEDS_TOUR,en?HIGH:LOW);
}

void api_strip_setpwr_res(bool en)
{
  digitalWrite(PIN_OUT_5V_LEDS_RES,en?HIGH:LOW);
}

void api_strip_setpwr_serial(bool en)
{
  digitalWrite(PIN_OUT_5V_SER,en?HIGH:LOW);
}

inline CRGB _api_getCol(int col)
{
  CRGB c;

  switch (col)
  {
    case C_BLUE:
    {
      c=COL_BLUE;
      break;
    }
    case C_GREEN:
    {
      c=COL_GREEN;
      break;
    }
    case C_RED:
    {
      c=COL_RED;
      break;
    }
    case C_OFF:
    default:
    {
      c=COL_BLACK;
      break;
    }
  }

  return c;
}

void api_strip_setcol_bas(int col)
{

  ldBas.setall(_api_getCol(col));
}

void api_strip_setcol_haut(int col)
{
  ldHaut.setall(_api_getCol(col));
}

void api_strip_setcol_tour(int col)
{
  Serial.println("tour!!");
  ldTour.setall(_api_getCol(col));
}

void api_strip_setcol_res(int col)
{
  ldRes.setall(_api_getCol(col));

}


void api_set_mode(int mode)
{
  g_mode=mode;
}

void api_set_aff(int aff)
{
  g_aff=aff;
  mem.setMode(aff);
  mem.save();
}

int api_get_mode(void)
{
  return g_mode;
}

int api_get_aff(void)
{
  return g_aff;
}


void api_get_date_hour(int &day,int &month,int &year,int &hour,int &minute,int &second,int &dow)
{
  DateTime now=scheduler.nowLocal();

  day=now.day();
  month=now.month();
  year=now.year();
  hour=now.hour();
  minute=now.minute();
  second=now.second();
  dow=now.dayOfWeek();
}

void api_get_date_hour(int &day,int &month,int &year,int &hour,int &minute,int &second)
{
  int dow;
  return api_get_date_hour(day, month, year, hour, minute, second,dow);
}

void api_set_date(int day,int month,int year)
{
  scheduler.setDate(day, month, year);
}

void api_set_hour(int hour,int minute,int second)
{
  scheduler.setHour(hour, minute, second);
}


float api_get_temp_bas(void)
{
  return g_temp_bas.get();
}

float api_get_hum_bas(void)
{
  return g_hum_bas.get();
}

float api_get_temp_ext(void)
{
  return g_temp_ext.get();
}

float api_get_hum_ext(void)
{
  return g_hum_ext.get();
}
