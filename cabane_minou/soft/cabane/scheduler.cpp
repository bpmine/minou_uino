#include "scheduler.h"

DS3231 rtc;

Scheduler::Scheduler()
{  
  ok=false;
}

bool Scheduler::begin()
{
  ok=true; /// @todo a retirer qd ok
  
  if (rtc.begin()==true)
  {
    ok=true;
    return true;
  }

  return false;
}

void Scheduler::setDateTime(DateTime &dte)
{
  if (ok==false)
    return;
    
  Serial.print("Update ");
  Serial.println(toDateTimeStr(dte));

  int off=getOffset(isEte(dte));
  Serial.print("Offset: -");
  Serial.println(off);

  DateTime dateHeureUTC (dte - TimeDelta(0,off,0,0));
  Serial.print("UTC: ");
  Serial.println(toDateTimeStr(dateHeureUTC));
    
  rtc.adjust(dateHeureUTC);  
}

void Scheduler::updateFromCompilationDateTime(void)
{
  if (ok==false)
    return;
  
  DateTime dateHeurePC (F(__DATE__), F(__TIME__));
  setDateTime(dateHeurePC);
}

void Scheduler::setDate(int day,int month,int year)
{
  if (ok==false)
    return;
  
  DateTime dteSet=nowLocal();
  dteSet.setday(day);
  dteSet.setmonth(month);
  dteSet.setyear(year);

  setDateTime(dteSet);  
}

void Scheduler::setHour(int hour,int minute,int second)
{
  if (ok==false)
    return;
  
  DateTime dteSet=nowLocal();
  dteSet.sethour(hour);
  dteSet.setminute(minute);
  dteSet.setsecond(second);

  setDateTime(dteSet);
}

void Scheduler::setDateTime(int day,int month,int year,int hour,int minute,int second)
{
  if (ok==false)
    return;
  
  DateTime dteSet=DateTime(year,month,day,hour,minute,second);
  dteSet.setday(day);
  dteSet.setmonth(month);
  dteSet.setyear(year);
  dteSet.sethour(hour);
  dteSet.setminute(minute);
  dteSet.setsecond(second);

  setDateTime(dteSet);  
}
  
bool Scheduler::isEte(DateTime &dte)
{
 /*
 * En France, le passage à l'heure d'hiver s'effectue le dernier dimanche d'octobre,
 * et le passage à l'heure d'été, le dernier dimanche de mars.
 * 
 * À la fin mars, on ajoute 1 heure, dans la nuit de samedi à dimanche.
 * Le changement se fait à 2 heures du matin, où il sera alors 3h du matin.
 * 
 * À la fin d'octobre, on retire 1 heure, dans la nuit de samedi à dimanche. 
 * Le changement se fait à 3 heures du matin, où il sera alors 2h du matin.
 * 
 * À noter que ce changement d'heure va possiblement disparaître, dans les années à venir.
 * Mais là pour l'instant, en 2023, il est toujours "en cours".
 * 
 */

  // Variables temporaires de cette fonction
  boolean estAvantLeDernierDimancheDeMars;
  boolean estApresLeDernierDimancheDeOctobre;
  boolean conditionValidite1;
  boolean conditionValidite2;
  boolean conditionValidite3;

  // Récupération temporaire des données dont nous aurons besoin
  int anneeDeLaDateHeureAanalyser = dte.year();
  int moisDeLaDateHeureAanalyser = dte.month();
  int jourDeLaDateHeureAanalyser = dte.day();
  int heuresDeLaDateHeureAanalyser = dte.hour();
  int minutesDeLaDateHeureAanalyser = dte.minute();

  // Recherche du "numéro" de jour correspondant au dernier dimanche du mois de mars (de l'année de la date à analyser)
  int dernierDimancheDuMoisDeMars;
  for(int i=31; i >= 1; i--) 
  {
    DateTime jourDeMars (anneeDeLaDateHeureAanalyser, 3, i, 0, 0, 0);       // Paramètres = année, mois, jour, heures, minutes, secondes
    if(jourDeMars.dayOfWeek() == 0)
    {
      dernierDimancheDuMoisDeMars = i;  // "dayOfTheWeek == 0" signifiant que nous sommes un "dimanche", avec la librairie RTClib,
      break;                            // et le "break" permet alors de quitter cette boucle for, comme nous avons trouvé le dernier dimanche du mois
    }
  }

  // Recherche du "numéro" de jour correspondant au dernier dimanche du mois d'octobre (de l'année de la date à analyser)
  int dernierDimancherDuMoisDeOctobre;
  for(int i=31; i >= 1; i--) 
  {
    DateTime jourDeOctobre (anneeDeLaDateHeureAanalyser, 10, i, 0, 0, 0);   // Paramètres = année, mois, jour, heures, minutes, secondes
    if(jourDeOctobre.dayOfWeek() == 0)
    {
      dernierDimancherDuMoisDeOctobre = i;  // "dayOfTheWeek == 0" signifiant que nous sommes un "dimanche", avec la librairie RTClib,
      break;                                // et le "break" permet alors de quitter cette boucle for, comme nous avons trouvé le dernier dimanche du mois
    }
  }

  // On teste pour savoir si on est avant le dernier dimanche de mars, et avant 3h du matin
  conditionValidite1 = moisDeLaDateHeureAanalyser < 3;
  conditionValidite2 = (moisDeLaDateHeureAanalyser == 3) && (jourDeLaDateHeureAanalyser < dernierDimancheDuMoisDeMars);
  conditionValidite3 = (moisDeLaDateHeureAanalyser == 3) && (jourDeLaDateHeureAanalyser == dernierDimancheDuMoisDeMars) && (heuresDeLaDateHeureAanalyser < 3);  
  if(conditionValidite1 || conditionValidite2 || conditionValidite3)
    estAvantLeDernierDimancheDeMars = true;
  else
    estAvantLeDernierDimancheDeMars = false;

  // On teste pour savoir si on est après le dernier dimanche d'octobre, et après 3h du matin
  // (remarque : surtout ne pas faire l'upload entre 2 et 3h du mat, ce jour là en particulier, sinon ça va dysfonctionner !)
  conditionValidite1 = moisDeLaDateHeureAanalyser > 10;
  conditionValidite2 = (moisDeLaDateHeureAanalyser == 10) && (jourDeLaDateHeureAanalyser > dernierDimancherDuMoisDeOctobre);
  conditionValidite3 = (moisDeLaDateHeureAanalyser == 10) && (jourDeLaDateHeureAanalyser == dernierDimancheDuMoisDeMars) && (heuresDeLaDateHeureAanalyser >= 3);  
  if(conditionValidite1 || conditionValidite2 || conditionValidite3)
    estApresLeDernierDimancheDeOctobre = true;
  else
    estApresLeDernierDimancheDeOctobre = false;

  // Et on retourne le résultat
  if(estAvantLeDernierDimancheDeMars || estApresLeDernierDimancheDeOctobre)
    return false;       // Car là, on serait en "heure d'hiver"
  else
    return true;        // Car là, on serait en "heure d'été"} 
}

int Scheduler::getOffset(bool ete)
{
  return ete==true?2:1; 
}

DateTime Scheduler::nowLocal(void)
{
  if (ok==false)
    return DateTime(0,0,0,0,0,0);
  
  DateTime utc=rtc.now();
  
  int nbreHeuresAjouter=getOffset(isEte(utc));
  
  DateTime local(utc + TimeDelta(0,nbreHeuresAjouter,0,0));
  
  return local;
}

DateTime Scheduler::nowUtc(void)
{
  if (ok==false)
    return DateTime(0,0,0,0,0,0);

  DateTime utc=rtc.now();
  return utc;
}

String Scheduler::toDateTimeStr(DateTime &dte)
{ 
  if (ok==false)
    return "";
  
  return toDateStr(dte) + " " + toHourStr(dte);
}

String Scheduler::toDateStr(DateTime &dte)
{
  if (ok==false)
    return "";

  char tmp[15];
  
  sprintf(
    tmp,
    "%02d/%02d/%04d",
    dte.day(),
    dte.month(),
    dte.year()
    );
    
  return String(tmp);
}
  
String Scheduler::toHourStr(DateTime &dte)
{
  if (ok==false)
    return "";
  
  char tmp[15];
  
  sprintf(
    tmp,
    "%02d:%02d:%02d",
    dte.hour(),
    dte.minute(),
    dte.second()
    );
    
  return String(tmp);
}

bool Scheduler::nowEte(void)
{
  if (ok==false)
    return false;
  
  DateTime utc=rtc.now();
  return isEte(utc);
}
