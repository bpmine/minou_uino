/**
 * @file terminal.cpp
 * @brief Gestion du terminal
 *
 **/
#ifndef TERMINAL_HEADER_INCLUDED
#define TERMINAL_HEADER_INCLUDED

#include "api.h"
#include <arduino.h>
#include "scheduler.h"
#include "when.hpp"


extern Scheduler scheduler;
extern bool g_capot;
extern bool g_veilleuse;

extern bool g_presence_bas;
extern bool g_presence_haut;
extern bool g_presence_tour;


static void _help(void)
{
  Serial.println("____________________");
  Serial.println("Aide:");
  Serial.println("  - test");
  Serial.println("  - state");
  Serial.println("  - pwr <bas, haut, tour, res, ser> <on/off>");
  Serial.println("  - strip <bas, haut, tour, res, ser> <0-4>");
  Serial.println("  - setdate dd/mm/yyyy");
  Serial.println("  - settime hh:mm:ss");
  Serial.println("  - time");
  Serial.println("  - aff <mode>");
  Serial.println("  - temp");

}

static bool _parseOnOff(const char *params,bool def)
{
  if (strcmp(params,"on")==0)
    return true;
  else if (strcmp(params,"off")==0)
    return false;
  else
    return def;
}

static void _OnOffAns(const char *strTitle,bool on)
{
  Serial.print(strTitle);
  Serial.print(": ");
  if (on==true)
    Serial.println("ON");
  else
    Serial.println("OFF");
}

static void _term_exec_state()
{
  /*bool bas=api_strip_getpwr_bas();
  bool haut=api_strip_getpwr_haut();
  bool tour=api_strip_getpwr_tour();
  bool res=api_strip_getpwr_res();
  bool serial=api_strip_getpwr_serial();*/

  Serial.println("Etat cabane:");
  /*Serial.print("  - Strip bas    : ");Serial.println(bas);
  Serial.print("  - Strip haut   : ");Serial.println(haut);
  Serial.print("  - Strip tour   : ");Serial.println(tour);
  Serial.print("  - Strip res    : ");Serial.println(res);
  Serial.print("  - Strip serial : ");Serial.println(serial);*/

  DateTime dte=scheduler.nowLocal();
  When w(dte);
  Serial.print("  - Soiree       : ");Serial.println(w.isSoiree());
  Serial.print("  - Matin        : ");Serial.println(w.isMatin());
  Serial.print("  - Pres. Haut   : ");Serial.println(g_presence_haut);
  Serial.print("  - Pres. Bas    : ");Serial.println(g_presence_bas);
  Serial.print("  - Pres. tour   : ");Serial.println(g_presence_tour);
  Serial.print("  - Capot        : ");Serial.println(g_capot);
  Serial.print("  - Veilleuse    : ");Serial.println(g_veilleuse);

  Serial.print("  - Mode         : ");Serial.println(api_get_mode());
  Serial.print("  - Affichage    : ");Serial.println(api_get_aff());
}

static void _term_exec_setpwr(const char *strParams)
{
  char name[20];
  char onoff[20];
  if ( (strlen(strParams)>19) || (sscanf(strParams,"%s %s",name,onoff)!=2) )
  {
    Serial.println("Format incorrect!");
    return;      
  }
  
  bool val=_parseOnOff(onoff,false);
  if (strcmp(name,"bas")==0)
  {
    api_strip_setpwr_bas(val);
    if (val==true)
      api_strip_setcol_bas(C_RED);
    _OnOffAns(name,val);
  }
  else if (strcmp(name,"haut")==0)
  {
    api_strip_setpwr_haut(val);
    if (val==true)
      api_strip_setcol_haut(C_RED);
    _OnOffAns(name,val);
  }
  else if (strcmp(name,"tour")==0)
  {
    api_strip_setpwr_tour(val);
    if (val==true)
      api_strip_setcol_tour(C_RED);
    _OnOffAns(name,val);
  }
  else if (strcmp(name,"res")==0)
  {
    api_strip_setpwr_res(val);
    if (val==true)
      api_strip_setcol_res(C_RED);
    _OnOffAns(name,val);
  }
  else if (strcmp(name,"ser")==0)
  {
    api_strip_setpwr_serial(val);
    _OnOffAns(name,val);
  }
  else
  {
    Serial.println("Strip LED non trouvé!");
  }
}

static void _term_exec_strip(const char *strParams)
{
  char name[20];
  int col;
  if ( (strlen(strParams)>19) || (sscanf(strParams,"%s %d",name,&col)!=2) )
  {
    Serial.println("Format incorrect!");
    return;
  }

  Serial.print("Strip col: ");
  if (strcmp(name,"bas")==0)
  {
    api_strip_setcol_bas(col);
    Serial.println(col);
  }
  else if (strcmp(name,"haut")==0)
  {
    api_strip_setcol_haut(col);
    Serial.println(col);
  }
  else if (strcmp(name,"tour")==0)
  {
    api_strip_setcol_tour(col);
    Serial.println(col);
  }
  else if (strcmp(name,"res")==0)
  {
    api_strip_setcol_res(col);
    Serial.println(col);
  }
  else
  {
    Serial.println("Strip LED non trouvé!");
  }
}

static void _term_exec_time(void)
{
  int year,month,day,hour,minute,second,dow;

  const char *dys[]={
      "Dimanche",
      "Lundi",
      "Mardi",
      "Mercredi",
      "Jeudi",
      "Vendredi",
      "Samedi"
  };

  api_get_date_hour(day, month, year, hour, minute, second,dow);

  if ( (dow<0) || (dow>6) )
    dow=0;

  char tmp[50];
  sprintf(tmp,"%s %02d/%02d/%02d %02d:%02d:%02d",dys[dow],day,month,year,hour,minute,second);
  Serial.println(tmp);
}

static void _term_exec_settime(const char *strParams)
{
  int hour,minute,second;
  if ( sscanf(strParams,"%02d:%02d:%02d",&hour,&minute,&second)==3 )
  {
    api_set_hour(hour, minute, second);
    _term_exec_time();
  }
  else
  {
    Serial.println("Paramètre incorrect!");
  }
}

static void _term_exec_aff(const char *strParams)
{
  int mode;
  if ( sscanf(strParams,"%d",&mode)==1 )
  {
    api_set_aff(mode);
    Serial.print("Mode affichage: ");
    Serial.println(mode);
  }
  else
  {
    Serial.println("Paramètre incorrect!");
  }
}

static void _term_exec_temp(void)
{
  Serial.println("Temperatures et humidites:");

  char tmp[30];
  char s1[10],s2[10];

  dtostrf (api_get_temp_bas(),4,1,s1);
  dtostrf (api_get_hum_bas(),4,1,s2);
  sprintf(tmp,"  - Bas: %s°C / %s%%",s1,s2);
  Serial.println(tmp);

  dtostrf (api_get_temp_ext(),4,1,s1);
  dtostrf (api_get_hum_ext(),4,1,s2);
  sprintf(tmp,"  - Ext: %s°C / %s%%",s1,s2);
  Serial.println(tmp);
}

static void _term_exec_setdate(const char *strParams)
{
  int day,month,year;
  if ( sscanf(strParams,"%02d/%02d/%04d",&day,&month,&year)==3 )
  {
    api_set_date(day,month,year);
    _term_exec_time();
  }
  else
  {
    Serial.println("Paramètre incorrect!");
  }
}

static void _execCmd(const char *strCmd,const char *strParams)
{
  if (strcmp(strCmd,"test")==0)
  {
    Serial.println("TEST OK");
  }
  else if (strcmp(strCmd,"help")==0)
  {
    _help();
  }
  else if (strcmp(strCmd,"state")==0)
  {
    _term_exec_state();
  }
  else if (strcmp(strCmd,"pwr")==0)
  {
    _term_exec_setpwr(strParams);
  }
  else if (strcmp(strCmd,"strip")==0)
  {
    _term_exec_strip(strParams);
  }
  else if (strcmp(strCmd,"time")==0)
  {
    _term_exec_time();
  }
  else if (strcmp(strCmd,"setdate")==0)
  {
    _term_exec_setdate(strParams);
  }
  else if (strcmp(strCmd,"settime")==0)
  {
    _term_exec_settime(strParams);
  }
  else if (strcmp(strCmd,"aff")==0)
  {
    _term_exec_aff(strParams);
  }
  else if (strcmp(strCmd,"temp")==0)
  {
    _term_exec_temp();
  }
}

void serialEvent(void)
{
  if (Serial.available()>0)
  {
    String st=Serial.readString();
    if (st.length()>30)
      return;


    st.trim();

    int inx=st.indexOf(" ");
    if (inx==-1)
    {
      _execCmd(st.c_str(),NULL);
    }
    else
    {
      _execCmd(st.substring(0,inx).c_str(),st.substring(inx+1).c_str());
    }
  }
}

#endif
