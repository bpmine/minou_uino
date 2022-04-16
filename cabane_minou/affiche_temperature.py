import requests
import json
import time

IP_TOUR='192.168.1.42'
IP_CABANE='192.168.1.44'

def read_sensors(ip):
    resp=requests.get('http://%s/sensors' % IP_CABANE)
    if resp.status_code==200:
        js=resp.json()
        return js

    return None

sens=read_sensors(IP_CABANE)
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


    
