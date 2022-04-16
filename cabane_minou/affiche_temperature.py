import requests
import json
import time
from win10toast import ToastNotifier

IP_TOUR='192.168.1.42'
IP_CABANE='192.168.1.44'

def read_sensors(ip):
    resp=requests.get('http://%s/sensors' % IP_CABANE)
    if resp.status_code==200:
        js=resp.json()
        return js

    return None

def display_sensors(sens):
    print('_'*60)
    print('HAUT:')
    print('  Temperature ext : %s°C' % (sens['temp_out']) )
    print('  Humidité ext    : %s%%' % (sens['hum_out']) )
    print('  Move haut       : %s' % (sens['move_haut']) )
    print('_'*60)
    print('BAS:')
    print('  Temperature bas : %s°C' % (sens['temp_bas']) )
    print('  Humidité bas    : %s%%' % (sens['hum_bas']) )
    print('  Move bas        : %s' % (sens['move_bas']) )
    print('  Move capot      : %s' % (sens['move_capot']) )
    print('_'*60)


class EstLa:
    def __init__(self):
        self.la=False
        self.last_ctr=None
        self.last_la=False

    def setNewCtr(self,ctr):
        if (self.last_ctr==None) or (self.last_ctr>ctr):
            self.last_ctr=ctr
            return

        if (ctr>self.last_ctr+1):
            self.la=True
        else:
            self.la=False

        self.last_ctr=ctr

        changed=False
        if self.last_la!=self.la:
            changed=True

        self.last_la=self.la

        return changed            

def showToast(toast,msg):
    if msg!=None:                
        toast.show_toast(
            "CABANE MINOU",
            msg,
            duration = 5,
            icon_path = "minou.ico"
            #threaded = True,
         )


def NotiticationSiMinouLa():
    enHaut=EstLa()
    enBas=EstLa()
    toast = ToastNotifier()

    while (True):
        sens=read_sensors(IP_CABANE)
        
        chHaut=enHaut.setNewCtr(sens['move_haut'])
        chBas=enBas.setNewCtr(sens['move_bas'])
        
        display_sensors(sens)

        if enBas.la==True:
            print('**** MINOU EST EN BAS ****')
        elif enHaut.la==True:
            print('**** MINOU EST EN HAUT ****')

        #chBas=True
        if (chBas==True) or (chHaut==True):
            print("QQche a changé...")
            msg=None
            #msg="bas"
            if (chBas==True) and (enBas.la==True):
                msg="MINOU VIENT D'APPARAITRE EN BAS"
            elif (chBas==True) and (enBas.la==False):
                msg="MINOU VIENT DE QUITTER LE BAS"
            elif (chHaut==True) and (enHaut.la==True):
                msg="MINOU VIENT D'APPARAITRE EN HAUT"
            elif (chHaut==True) and (enHaut.la==False):
                msg="MINOU VIENT DE QUITTER LE HAUT"            

            showToast(toast,msg)
            
        time.sleep(20)
    
    

if __name__=='__main__':
    NotiticationSiMinouLa()

    
