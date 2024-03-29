#!/usr/bin/python3
import os
from datetime import datetime,timezone
from dateutil import tz
import time
from sys import argv
import requests
import re

IP_CABANE='192.168.2.21'
IP_TOUR='192.168.2.22'

LOG_PATH='/var/log/domgw'
LOG_FILENAME='automatisme.log'
LOG_ENABLED=True

prev_missing=[]

class ArdLeds:
    GREEN='g'
    BLUE='b'
    WHITE='w'
    RED='r'
    OFF='0'
    YELLOW='y'
    ORANGE='o'
    PINK='p'
    CYAN='c'
    VIOLET='v'
 
    def __init__(self,name,ip,nbre_leds,base_url='leds'):
        self.name=name
        self.ip=ip
        self.nbre_leds=nbre_leds
        self.base_url=base_url

    def disableWdg(self):
        resp=requests.get("http://%s/leds/wdg?enable=0" % (self.ip))
        print(resp.text)
        
    def enableWdg(self):
        resp=requests.get("http://%s/leds/wdg?enable=1" % (self.ip))
        print(resp.text)

    def setWdg(self,delay_s):
        resp=requests.get("http://%s/leds/wdg?delay=%d" % (self.ip,delay_s))
        print(resp.text)

    def getWdg(self):
        try:
            resp=requests.get("http://%s/leds/wdg" % (self.ip))
            if resp!=None and resp.status_code==200 and resp.text=='WDG OK':
                return True                
        except:
            pass
        
        return False

    def sendCmd(self,start,end,col):
        try:
            assert start>0 and start<=self.nbre_leds
            assert end>=start and end<=self.nbre_leds
            
            resp=requests.get("http://%s/%s/set?start=%d&end=%d&col=%s" % (self.ip,self.base_url,start,end,col))
            if resp!=None and resp.status_code==200:
                print(resp.text)
                return True            
        except:
            pass
        
        return False

    def setAnim(self,prog):
        try:
            resp=requests.get('http://%s/%s/anim?enable=0' % (self.ip,self.base_url))
            print(resp.text)
            resp=requests.get('http://%s/%s/anim?prog=%s' % (self.ip,self.base_url,prog))
            print(resp.text)
            resp=requests.get('http://%s/%s/anim?enable=1' % (self.ip,self.base_url))
            print(resp.text)
        except:
            pass

    def enableAnim(self):
        try:
            resp=requests.get('http://%s/%s/anim?enable=1' % (self.ip,self.base_url))
            print(resp.text)
        except:
            pass
        
    def disableAnim(self):
        try:
            resp=requests.get('http://%s/%s/anim?enable=0' % (self.ip,self.base_url))
            print(resp.text)
        except:
            pass
        
    def clrAll(self):
        self.sendCmd(1,self.nbre_leds,ArdLeds.OFF)
        
    def setAll(self,col):
        self.sendCmd(1,self.nbre_leds,col)

    def set(self,start,end,col):
        self.sendCmd(start,end,col)
        

ledsCabaneHaut=ArdLeds('cabane_haut',IP_CABANE,36,'leds/haut')
ledsCabaneBas=ArdLeds('cabane_bas',IP_CABANE,36,'leds/bas')
ledsTour=ArdLeds('tour',IP_TOUR,33)


#PROG_INITIAL_TOUR='S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*'
PROG_1_CABANE='D2LrAWLgAWLbAW*'
PROG_INITIAL_TOUR='D1S26E33LrOS0E25LgOWS26E33LbOS0E25LrOWS26E33LgOS0E25LbOW*'

def clearAll():
    ledsCabaneBas.disableAnim()
    ledsCabaneHaut.disableAnim()
    ledsTour.disableAnim()


    ledsCabaneBas.clrAll()
    ledsCabaneHaut.clrAll()
    ledsTour.clrAll()

def log(msg):
    dte=datetime.now()
    sDte=dte.strftime('%d/%m/%Y %H:%M:%S')

    print('%s> %s\n' % (sDte,msg))
    
    if LOG_ENABLED==True:        
        with open(os.path.sep.join([LOG_PATH,LOG_FILENAME]),'a+') as fp:                        
            fp.write('%s> %s\n' % (sDte,msg))
            fp.close()  


def tick_minute(hour,minute):
    global prev_missing
    #log('Tick %d:%d' % (hour,minute) )

    isBasOk=ledsCabaneBas.getWdg()
    isHautOk=ledsCabaneHaut.getWdg()
    isTourOk=ledsTour.getWdg()

    missing=[]
    if isBasOk==False:
        missing.append('Bas')
    if isHautOk==False:
        missing.append('Haut')
    if isTourOk==False:
        missing.append('Tour')

    if len(missing)>0 and prev_missing!=missing:
        log('[ERREUR] - Au moins un équipement ne répond pas: %s' % (missing))
    elif prev_missing!=missing:
        log('Tous les équipements répondent de nouveau')

    prev_missing=missing
        
   
    if (hour==6) and (minute==0):
        log('Debut animation du matin')
        
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()
        
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.clrAll()
        ledsTour.setAll('b')
    elif (hour==6) and (minute==30):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()        
        ledsTour.disableAnim()
        
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.setAll('b')
        ledsTour.setAll('b')
    elif (hour==7) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()
        
        ledsCabaneBas.setAll('r')
        ledsCabaneHaut.setAll('g')
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==7) and (minute>0) and (minute<15):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        
        ledsCabaneBas.setAll('r')
        if (minute % 2) == 0:            
            ledsCabaneHaut.setAll('b')
        else:
            ledsCabaneHaut.setAll('g')
    elif (hour==7) and (minute>15) and (minute<30):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        
        ledsCabaneBas.setAll('b')        
        if (minute % 2) == 0:
            ledsCabaneHaut.setAll('g')
        else:
            ledsCabaneHaut.setAll('r')
    elif (hour==7) and (minute>30) and (minute<45):
        ledsCabaneBas.disableAnim()
        ledsCabaneBas.clrAll()        
        ledsCabaneHaut.disableAnim()
        ledsCabaneBas.setAll('g')
        if (minute % 2) == 0:
            ledsCabaneHaut.setAll('b')
        else:
            ledsCabaneHaut.setAll('r')
    elif (hour==7) and (minute==45):
        ledsCabaneBas.disableAnim()
        ledsCabaneBas.setAll('g')
        ledsCabaneHaut.setAnim(PROG_1_CABANE)
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==8) and (minute==0):
        log('Fin animation du matin')
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()        
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.clrAll()
        ledsTour.clrAll()
    elif (hour>8) and (hour<17) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()        
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.clrAll()
        ledsTour.clrAll()
    elif (hour==17) and (minute==0):
        log('Debut animation du soir')
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()
        ledsCabaneBas.setAll('g')
        ledsCabaneHaut.setAll('g')
        ledsTour.setAll('g')        
    elif (hour==17) and (minute==30):
        ledsTour.disableAnim()
        ledsCabaneBas.setAll('g')
        ledsCabaneHaut.setAll('r')
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==18) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneBas.setAll('b')
        ledsCabaneHaut.setAnim(PROG_1_CABANE)
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==19) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneBas.setAll('b')
        ledsCabaneHaut.setAnim(PROG_1_CABANE)
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==20) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneBas.setAll('r')
        ledsCabaneHaut.setAnim(PROG_1_CABANE)
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==21) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneBas.setAll('r')
        ledsCabaneHaut.setAnim(PROG_1_CABANE)
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==22) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsCabaneBas.clrAll()        
        ledsCabaneHaut.setAll('r')
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==23) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsCabaneBas.clrAll()        
        ledsCabaneHaut.setAll('b')
        ledsTour.setAnim(PROG_INITIAL_TOUR)
    elif (hour==0) and (minute==0):
        log('Fin anim du soir / Tour en veilleuse rouge puis bleu puis vert...')
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.clrAll()
        ledsTour.setAll('r')
    elif (hour>=0) and (hour<6):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.clrAll()
        res=minute%3
        if res==0:
            ledsTour.setAll('r')
        elif res==1:
            ledsTour.setAll('g')
        else:
            ledsTour.setAll('b')
        

if LOG_ENABLED==True:
    if not os.path.exists(LOG_PATH):
        os.mkdir(LOG_PATH)

    
now=datetime.now(timezone.utc)
FRA=tz.gettz('Europe/Paris')
now=now.astimezone(tz=FRA)

hour=now.hour
minute=now.minute
if len(argv)==2:
    p=re.compile(r'^([0-9]{2}):([0-9]{2})$')
    m=p.match(argv[1])
    if m!=None:
        hour=int(m.group(1))
        minute=int(m.group(2))
        log('APPEL MANUEL: %02d:%02d' % (hour,minute))

tick_minute(hour,minute)


    
        



    
