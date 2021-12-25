import requests;
import time;

IP="192.168.1.44"

def test1():
    resp=requests.get("http://%s/leds/wdg?enable=1" % (IP))
    print(resp.text)
    resp=requests.get("http://%s/leds/wdg/info" % (IP))
    print(resp.text)

    time.sleep(2)

    resp=requests.get("http://%s/leds/wdg/info" % (IP))
    print(resp.text)

    time.sleep(9)
    resp=requests.get("http://%s/leds/wdg/info" % (IP))
    print(resp.text)


def test2():
    resp=requests.get("http://%s/leds/haut/set?start=1&end=12&col=r" % (IP))
    print(resp.text)
    resp=requests.get("http://%s/leds/haut/set?start=13&end=26&col=b" % (IP))
    print(resp.text)
    resp=requests.get("http://%s/leds/bas/set?start=1&end=26&col=g" % (IP))
    print(resp.text)


test1()    
