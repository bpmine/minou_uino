#!/usr/bin/python3
import os
import datetime
import time
import requests

IP_CABANE='192.168.1.44'
IP_TOUR='192.168.1.42'

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
        resp=requests.get("http://%s/leds/wdg" % (self.ip))
        return resp.json

    def sendCmd(self,start,end,col):
        assert start>0 and start<=self.nbre_leds
        assert end>=start and end<=self.nbre_leds
        
        resp=requests.get("http://%s/%s/set?start=%d&end=%d&col=%s" % (self.ip,self.base_url,start,end,col))
        print(resp.text)

    def setAnim(self,prog):
        resp=requests.get('http://%s/%s/anim?enable=0' % (self.ip,self.base_url))
        print(resp.text)
        resp=requests.get('http://%s/%s/anim?prog=%s' % (self.ip,self.base_url,prog))
        print(resp.text)
        resp=requests.get('http://%s/%s/anim?enable=1' % (self.ip,self.base_url))
        print(resp.text)

    def enableAnim(self):
        resp=requests.get('http://%s/%s/anim?enable=1' % (self.ip,self.base_url))
        print(resp.text)
        
    def disableAnim(self):
        resp=requests.get('http://%s/%s/anim?enable=0' % (self.ip,self.base_url))
        print(resp.text)
        
    def clrAll(self):
        self.sendCmd(1,self.nbre_leds,ArdLeds.OFF)
        
    def setAll(self,col):
        self.sendCmd(1,self.nbre_leds,col)

    def set(self,start,end,col):
        self.sendCmd(start,end,col)
        

ledsCabaneHaut=ArdLeds('cabane_haut',IP_CABANE,36,'leds/haut')
ledsCabaneBas=ArdLeds('cabane_bas',IP_CABANE,36,'leds/bas')
ledsTour=ArdLeds('tour',IP_TOUR,33)


#cmd_prog='S26E33LrOS1E23X800LgOWLrOWLbOWLgOWLrOWLbOWX800CS26E33LgOS1E23WX100LgOWLrOWLbOWLgOWLrOWLbOWLgOWLrOWLbOWLgOWLrOWLbOWX1000ZW*'
PROG_INITIAL_TOUR='S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*'

PROG_1_CABANE='D2LrAWLgAWLbAW*'

def clearAll():
    ledsCabaneBas.disableAnim()
    ledsCabaneHaut.disableAnim()
    ledsTour.disableAnim()


    ledsCabaneBas.clrAll()
    ledsCabaneHaut.clrAll()
    ledsTour.clrAll()



def tick_minute(hour,minute):
    print('Tick %d:%d' % (hour,minute) )
    
    if (hour==6) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.setAll('b')
        ledsTour.setAnim(PROG_INITIAL_TOUR)

    if (hour==7) and (minute==0):
        ledsCabaneBas.disableAnim()
        
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.setAnim(PROG_1_CABANE)
        ledsTour.setAnim(PROG_INITIAL_TOUR)

    if (hour==9) and (minute==0):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()
        
        ledsCabaneBas.clrAll()
        ledsCabaneHaut.clrAll()
        ledsTour.clrAll()

    if (hour==17) and (minute==0):
        ledsCabaneHaut.setWdg(3600*12)
        ledsTour.setWdg(3600*12)
        
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()

        ledsCabaneBas.setAll('g')
        ledsCabaneHaut.setAll('g')
        ledsTour.setAll('g')
        
    if (hour==17) and (minute==30):
        ledsTour.disableAnim()
        ledsCabaneBas.setAll('g')
        ledsCabaneHaut.setAll('r')
        ledsTour.setAnim(PROG_INITIAL_TOUR)
        
    if (hour==18) and (minute==00):
        ledsCabaneBas.disableAnim()
        ledsCabaneBas.setAll('b')
        ledsCabaneHaut.setAnim(PROG_1_CABANE)
        ledsTour.setAnim(PROG_INITIAL_TOUR)

    if (hour==22) and (minute==00):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()

        ledsCabaneBas.clrAll()        
        ledsCabaneHaut.setAll('r')
        ledsTour.setAnim(PROG_INITIAL_TOUR)

    if (hour==23) and (minute==00):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()

        ledsCabaneBas.clrAll()        
        ledsCabaneHaut.setAll('b')
        ledsTour.setAnim(PROG_INITIAL_TOUR)

    if (hour==00) and (minute==00):
        ledsCabaneBas.disableAnim()
        ledsCabaneHaut.disableAnim()
        ledsTour.disableAnim()

        ledsCabaneBas.clrAll()
        ledsCabaneHaut.clrAll()
        ledsTour.setAll('r')


def run():
    lh=None
    lm=None
    boot=True

    while True:
        now=datetime.datetime.now()
        if boot==True:
            print('BOOT')
            lh=now.hour
            lm=now.minute
            boot=False
            continue

        h=now.hour
        m=now.minute
        if (h!=lh) or (m!=lm):
            tick_minute(h,m)
            
        lh=now.hour
        lm=now.minute

        time.sleep(0.5)

#run()
        
now=datetime.datetime.now()
tick_minute(now.hour,now.minute)


    
