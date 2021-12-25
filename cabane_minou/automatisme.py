import os
import datetime
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
        resp=requests.get("http://%s/wdg?enable=0" % (self.ip))
        print(resp.text)
        
    def enableWdg(self):
        resp=requests.get("http://%s/wdg?enable=1" % (self.ip))
        print(resp.text)

    def setWdg(self,delay_s):
        resp=requests.get("http://%s/wdg?delay=%d" % (self.ip,delay_s))
        print(resp.text)

    def getWdg(self):
        resp=requests.get("http://%s/wdg" % (self.ip))
        print(resp.json)

    def sendCmd(self,start,end,col):
        assert start>0 and start<=self.nbre_leds
        assert end>=start and end<=self.nbre_leds
        
        resp=requests.get("http://%s/%s/set?start=%d&end=%d&col=%s" % (self.ip,self.base_url,start,end,col))
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

ledsCabaneHaut.setAll('r')
ledsCabaneBas.setAll('r')
ledsTour.setAll('r')

ledsCabaneHaut.getWdg()

def run17H():
    pass

tab=[
    {
        'time':'17:00',
        'func':run17H
     }
    ]



    
