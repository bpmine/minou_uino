import requests

IP_TOUR='192.168.1.42'
IP_CABANE='192.168.1.44'


def test_tour():

    #cmd_prog='S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*'
    #cmd_prog='X50ZS1LgOWCS2OWCS3OWCS4OWCS5OWCS6OWCS7OWCS8OWCS9OWCS10OWCS11OWCS12OWCS13OWCS14OWCS15OWCS16OWCS17OWCS18OWCS19OWCS20OWCS21OWC*'
    cmd_prog='S26E33LrOS1E23X800LgOWLrOWLbOWLgOWLrOWLbOWX800CS26E33LgOS1E23WX100LgOWLrOWLbOWLgOWLrOWLbOWLgOWLrOWLbOWLgOWLrOWLbOWX1000ZW*'

    IP='192.168.1.42'

    resp=requests.get('http://%s/leds/wdg' % IP)
    print(resp.text)

    resp=requests.get('http://%s/leds/wdg?delay=36000' % IP)
    print(resp.text)

    resp=requests.get('http://%s/leds/anim?enable=0' % IP)
    print(resp.text)

    resp=requests.get('http://%s/leds/anim?prog=%s' % (IP,cmd_prog))
    print(resp.text)

    resp=requests.get('http://%s/leds/anim?enable=1' % IP)
    print(resp.text)

    resp=requests.get('http://%s/leds/anim?info' % IP)
    print(resp.text)


def init_tour():

    cmd_prog='S26E33LrOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LgOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOWS26E33LbOS1E23X200LgOWLrOWLbOWLgOWLrOWLbOW*'
    #cmd_prog='X50ZS1LgOWCS2OWCS3OWCS4OWCS5OWCS6OWCS7OWCS8OWCS9OWCS10OWCS11OWCS12OWCS13OWCS14OWCS15OWCS16OWCS17OWCS18OWCS19OWCS20OWCS21OWC*'
    #cmd_prog='S26E33LrOS1E23X800LgOWLrOWLbOWLgOWLrOWLbOWX800CS26E33LgOS1E23WX100LgOWLrOWLbOWLgOWLrOWLbOWLgOWLrOWLbOWLgOWLrOWLbOWX1000ZW*'

    resp=requests.get('http://%s/leds/wdg' % IP_TOUR)
    print(resp.text)

    resp=requests.get('http://%s/leds/wdg?delay=18000' % IP_TOUR)
    print(resp.text)

    resp=requests.get('http://%s/leds/anim?enable=0' % IP_TOUR)
    print(resp.text)

    resp=requests.get('http://%s/leds/anim?prog=%s' % (IP_TOUR,cmd_prog))
    print(resp.text)

    resp=requests.get('http://%s/leds/anim?enable=1' % IP_TOUR)
    print(resp.text)

    resp=requests.get('http://%s/leds/anim?info' % IP_TOUR)
    print(resp.text)

def init_cabane():

    cmd_prog='D2LrAWLgAWLbAW*'

    resp=requests.get('http://%s/leds/wdg' % IP_CABANE)
    print(resp.text)
    resp=requests.get('http://%s/leds/wdg?delay=18000' % IP_CABANE)
    print(resp.text)
    resp=requests.get('http://%s/leds/haut/anim?enable=0' % IP_CABANE)
    print(resp.text)

    resp=requests.get('http://%s/leds/haut/anim?prog=%s' % (IP_CABANE,cmd_prog))
    print(resp.text)

    resp=requests.get('http://%s/leds/haut/anim?enable=1' % IP_CABANE)
    print(resp.text)

    resp=requests.get('http://%s/leds/haut/anim?info' % IP_CABANE)
    print(resp.text)

def disable_wdg():
    resp=requests.get('http://%s/leds/wdg?enable=0' % IP_TOUR)
    print(resp.text)
    resp=requests.get('http://%s/leds/wdg?enable=0' % IP_CABANE)
    print(resp.text)

def clear_all():
    resp=requests.get('http://%s/leds/clearall' % IP_TOUR)
    print(resp.text)


clear_all() 

#init_tour()  
#init_cabane()

#disable_wdg()
