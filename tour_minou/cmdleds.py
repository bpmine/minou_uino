import requests;
from time import sleep
from random import randint

lstCmds=[
    [(0,255,0,0,0)],
    [(0,12,127,0,0),(13,25,0,127,0),(26,34,0,0,127)],
    ]

class BdLeds:
    ip=None
    nb_leds=0
    leds=[];

    def apply(self):
        dta="AA%02x" % (self.nb_leds);
        for (r,g,b) in self.leds:
            dta+="%02x%02x%02x" % (r,g,b)
            
        print(dta);
        r = requests.post('http://%s/leds/write' % (self.ip),data=dta)
        print(r.text);

    def get_info(self):
        r = requests.get('http://%s/leds/info' % (self.ip))
        return r.text;
        

    def setAllLeds(self,col):
        for i in range(self.nb_leds):
            self.leds[i]=col;

    def clearAllLeds(self):
        self.setAllLeds((0,0,0))
    
    def __init__(self,nb_leds,ip):
        if (nb_leds>255):
            raise Exception("Trop grand!!")

        self.ip=ip;
        self.nb_leds=nb_leds
        self.leds=[(0,0,0) for i in range(nb_leds)]

        

class GrpLd:
    lds=None
    list_leds=[]
    def __init__(self,lds):
        self.lds=lds
        self.list_leds=[]
    def addLed(self,n):
        if n<self.lds.nb_leds:
            self.list_leds.append(n)
    def addLeds(self,nStart, nEnd):
        for i in range(nStart,nEnd):
            if i<self.lds.nb_leds:
                self.list_leds.append(i)
    def setColor(self,col):
        for i in self.list_leds:
            lds.leds[i]=col
    def clear(self):
        for i in self.list_leds:
            lds.leds[i]=(0,0,0)
        
NUM_LEDS=34

lds=BdLeds(NUM_LEDS,"192.168.3.10");
print(lds.get_info());

lds.setAllLeds((0,0,0));
lds.apply()

grpAr=GrpLd(lds)
grpAr.addLeds(0, 13)
grpAv=GrpLd(lds)
grpAv.addLeds(13, 26)
grpInt=GrpLd(lds)
grpInt.addLeds(26, 34)

grpAr1=GrpLd(lds)
grpAr1.addLeds(9,12)
grpAr2=GrpLd(lds)
grpAr2.addLeds(4, 6)
grpAr3=GrpLd(lds)
grpAr3.addLeds(0, 2)

grpAv1=GrpLd(lds)
grpAv1.addLeds(14,17)
grpAv2=GrpLd(lds)
grpAv2.addLeds(19, 22)
grpAv3=GrpLd(lds)
grpAv3.addLeds(23,26)

grpTour=GrpLd(lds)
grpTour.addLeds(0,26)




def bbr_alt():
    grpInt.setColor((0,0,127))
    lds.apply();

    while True:
        grpAv1.setColor((0,0,127))
        grpAv2.setColor((127,127,127))
        grpAv3.setColor((127,0,0))
        grpAr1.clear();
        grpAr2.clear();
        grpAr3.clear();
        lds.apply();

        sleep(1)

        grpAr1.setColor((0,0,127))
        grpAr2.setColor((127,127,127))
        grpAr3.setColor((127,0,0))
        grpAv1.clear();
        grpAv2.clear();
        grpAv3.clear();
        lds.apply();

        sleep(1)

def dep_3ld():
    grpInt.setColor((0,127,0))
    lds.apply();
    
    while True:
        col=(127,127,0)
        
        grpTour.clear();
        grpAv1.setColor(col)
        lds.apply();

        grpTour.clear();
        grpAv2.setColor(col)
        lds.apply();

        grpTour.clear();
        grpAv3.setColor(col)
        lds.apply();

def dep_3ld2():
    grpInt.setColor((0,127,0))
    lds.apply();
    
    while True:
        cols=[
            (127,0,0),
            (0,127,0),
            (0,0,127),
            (127,127,0),
            (0,127,127),
            (127,0,127),
            (127,127,127)
            ];

        for col in cols:
            for i in range(13-3):
                grpTour.clear();
                lds.leds[i]=col
                lds.leds[i+1]=col
                lds.leds[i+2]=col
                lds.leds[i+13]=col
                lds.leds[i+13+1]=col
                lds.leds[i+13+2]=col
                lds.apply();

        for col in cols:
            (r,g,b)=col;
            if (r==127):
                r=1;
            if (g==127):
                g=1;
            if (b==127):
                b=1;

            for i in range (0,80,5):
                if (r!=0):
                    r+=5;
                if (g!=0):
                    g+=5;
                if (b!=0):
                    b+=5;

                if (r>127):
                    r=127;
                if (g>127):
                    g=127;
                if (b>127):
                    b=127;

                grpTour.setColor((r,g,b))
                lds.apply();
                if r==127 and g==127 and b==127:
                    break;

                sleep(0.05)

            for i in range (0,80,5):
                r-=5;
                g-=5;
                b-=5;
                if (r<0):
                    r=0;
                if (g<0):
                    g=0;
                if (b<0):
                    b=0;

                grpTour.setColor((r,g,b))
                lds.apply();
                if r==0 and g==0 and b==0:
                    break;
                
                
            

#bbr_alt();
#dep_3ld2()

#grpTour.setColor((127,0,0))
#grpInt.setColor((0,127,0))
lds.clearAllLeds()
lds.apply();

