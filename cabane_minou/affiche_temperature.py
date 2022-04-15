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
print('Temperature ext: %s°C' % (sens['temp_out']) )
print('Temperature bas: %s°C' % (sens['temp_bas']) )


    
