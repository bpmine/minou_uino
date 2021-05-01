# CABANE MINOU 2 - Pilotage par Wifi

C'est la deuxième version de la carte de contrôle de la cabane de minou.
Cette fois-ci, elle est accessible par Wifi.

## Interface E/S

### Bornier

Num | Type | Port|Description
--|--|--|--
1|Alim||Masse
2|Entrée|D12|Température extérieure DH11 (Voir aussi J3)
3|Entrée|D11|Mouvement en bas 
4|Entrée|D7|Température en bas DH11
5|Sortie|D6|Bandeau de LEDs du Haut
6|Entrée|D5|Capot
7|Sortie|D4|LED blanche du bas
8|Sortie|D2|Bandeau de LEDs du bas
9|Entrée|D1|Mouvement en haut
10|Alim||5V

*NB:* L'entrée 2 n'est pas connectée car elle est dispo sur le picot J3 de la carte.

```c
#define PIN_DARK          A0

#define PIN_MOVE_HAUT     D1
#define PIN_DATA_LED_BAS  D2 
#define PIN_LED_BAS       D4
#define PIN_CAPOT         D5
#define PIN_DATA_LED_HAUT D6
#define PIN_DHT11_BAS     D7

#define PIN_MOVE_BAS      9
#define PIN_DHT11_OUT     10
```

## Fonctionnalités

- Bandeau