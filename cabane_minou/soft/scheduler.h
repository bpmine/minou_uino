#ifndef SCHEDULER_HEADER_INCLUDED
#define SCHEDULER_HEADER_INCLUDED

#include <RTClib.h>

class Scheduler
{
  private:
    bool ok;

    bool isEte(DateTime &utc);
    int getOffset(bool ete);   
    void setDateTime(DateTime &dte); 
    
  public:
    Scheduler();

    bool begin();
    
    DateTime nowLocal(void);
    DateTime nowUtc(void);
    bool nowEte(void);
    
    void updateFromCompilationDateTime(void);
    void setDate(int day,int month,int year);
    void setHour(int hour,int minute,int second);
    void setDateTime(int day,int month,int year,int hour,int minute,int second);

    String toDateTimeStr(DateTime &dte);
    String toDateStr(DateTime &dte);
    String toHourStr(DateTime &dte);
};

#endif
