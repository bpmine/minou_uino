# DATALOGGER (ex minoucabuino)

Le datalogger a pour fonction d'enregistrer les valeurs des differents capteurs sur une carte SD.

![GitHub Logo](/images/datalogger.png)

## Capteurs/Actionneurs

### Capteurs

Le datalogger enregistre les valeurs des capteurs suivants.

Type|Emplacement|Description
-|-|-
DHT11|Intérieur bas|Mesure de la température à l'intérieur
DHT11|Intérieur bas|Mesure de l'humidité à l'intérieur
PIR|Intérieur bas|Détection de mouvement en bas
SWITCH|Intérieur bas|Détection de l'ouverture du couvercle
-|-|-
PIR|Intérieur milieu|Détection de mouvement au milieu
-|-|-
DHT11|Extérieur|Mesure de la température à l'extérieur
DHT11|Extérieur|Mesure de l'humidité à l'extérieur
Photoresistance|Extérieur|Mesure de l'éclairage extérieur

### Commandes

Type|Emplacement|Description
-|-|-
DHT11|Intérieur bas|Bandeau de 26 LEDs RGB
LED Blanche|Intérieur bas|LED blanche (témoin d'ouverture du couvercle)
-|-|-
DHT11|Intérieur milieu|Bandeau de 26 LEDs RGB

## Schéma et détail technique

Un arduino Nano + datalogger shield est utilisé pour piloter tous ces éléments.

![GitHub Logo](/images/nanologger.png)

### Schéma

## Quelques mesures...

