EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L bpc:NodeMCUs U1
U 1 1 6046A751
P 2950 1850
F 0 "U1" H 4500 2115 50  0000 C CNN
F 1 "NodeMCUs" H 4500 2024 50  0000 C CNN
F 2 "bpc:BPC_NodeMCU_small" H 3150 200 50  0001 L CNN
F 3 "https://github.com/nodemcu/nodemcu-devkit-v1.0/blob/master/NODEMCU_DEVKIT_V1.0.PDF" H 3150 50  50  0001 L CNN
F 4 "NodeMCU DEVKIT V1.0" H 3150 -100 50  0001 L CNN "Description"
F 5 "7.35" H 3150 -700 50  0001 L CNN "Height"
F 6 "Seeed Studio" H 3150 -800 50  0001 L CNN "Manufacturer_Name"
F 7 "113990105" H 3150 -1050 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "713-113990105" H 3150 -250 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/Seeed-Studio/113990105/?qs=SElPoaY2y5KMx8k1roAIKw%3D%3D" H 3150 -400 50  0001 L CNN "Mouser Price/Stock"
F 10 "113990105" H 3150 -950 50  0001 L CNN "Arrow Part Number"
F 11 "https://www.arrow.com/en/products/113990105/seeed-technology-limited" H 3150 -550 50  0001 L CNN "Arrow Price/Stock"
	1    2950 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 6046DFE9
P 3950 750
F 0 "R2" H 4020 796 50  0000 L CNN
F 1 "R" H 4020 705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 3880 750 50  0001 C CNN
F 3 "~" H 3950 750 50  0001 C CNN
	1    3950 750 
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0101
U 1 1 6046FAE7
P 4250 750
F 0 "#PWR0101" H 4250 600 50  0001 C CNN
F 1 "+3.3V" H 4265 923 50  0000 C CNN
F 2 "" H 4250 750 50  0001 C CNN
F 3 "" H 4250 750 50  0001 C CNN
	1    4250 750 
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 60470189
P 3100 750
F 0 "#PWR0102" H 3100 500 50  0001 C CNN
F 1 "GND" H 3105 577 50  0000 C CNN
F 2 "" H 3100 750 50  0001 C CNN
F 3 "" H 3100 750 50  0001 C CNN
	1    3100 750 
	0    1    1    0   
$EndComp
$Comp
L Device:D_Zener D1
U 1 1 60470D30
P 3450 750
F 0 "D1" V 3404 830 50  0000 L CNN
F 1 "1V" V 3495 830 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P2.54mm_Vertical_AnodeUp" H 3450 750 50  0001 C CNN
F 3 "~" H 3450 750 50  0001 C CNN
	1    3450 750 
	-1   0    0    1   
$EndComp
Wire Wire Line
	3800 750  3750 750 
Wire Wire Line
	3300 750  3100 750 
Wire Wire Line
	4250 750  4100 750 
$Comp
L Device:R R1
U 1 1 604719A0
P 3450 1350
F 0 "R1" V 3243 1350 50  0000 C CNN
F 1 "R" V 3334 1350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 3380 1350 50  0001 C CNN
F 3 "~" H 3450 1350 50  0001 C CNN
	1    3450 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 1350 3750 750 
Connection ~ 3750 750 
Wire Wire Line
	3750 750  3600 750 
Text Notes 2250 1100 0    50   ~ 0
Vers capteur lum.
$Comp
L power:+5V #PWR0103
U 1 1 60476C35
P 2950 3500
F 0 "#PWR0103" H 2950 3350 50  0001 C CNN
F 1 "+5V" H 2965 3673 50  0000 C CNN
F 2 "" H 2950 3500 50  0001 C CNN
F 3 "" H 2950 3500 50  0001 C CNN
	1    2950 3500
	-1   0    0    1   
$EndComp
Wire Wire Line
	2950 3250 2950 3450
$Comp
L power:GND #PWR0104
U 1 1 60477C1D
P 2700 3500
F 0 "#PWR0104" H 2700 3250 50  0001 C CNN
F 1 "GND" H 2705 3327 50  0000 C CNN
F 2 "" H 2700 3500 50  0001 C CNN
F 3 "" H 2700 3500 50  0001 C CNN
	1    2700 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3150 2700 3150
Wire Wire Line
	2700 3150 2700 3500
$Comp
L power:+3.3V #PWR0105
U 1 1 604783B1
P 2800 2950
F 0 "#PWR0105" H 2800 2800 50  0001 C CNN
F 1 "+3.3V" V 2815 3078 50  0000 L CNN
F 2 "" H 2800 2950 50  0001 C CNN
F 3 "" H 2800 2950 50  0001 C CNN
	1    2800 2950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2950 2850 2800 2850
Wire Wire Line
	2800 2850 2800 2950
$Comp
L power:GND #PWR0106
U 1 1 60479EEB
P 2100 3000
F 0 "#PWR0106" H 2100 2750 50  0001 C CNN
F 1 "GND" H 2105 2827 50  0000 C CNN
F 2 "" H 2100 3000 50  0001 C CNN
F 3 "" H 2100 3000 50  0001 C CNN
	1    2100 3000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J4
U 1 1 6047AAE7
P 9850 2350
F 0 "J4" H 9950 3150 50  0000 C CNN
F 1 "Bornier" H 9950 3000 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-10-5.08_1x10_P5.08mm_Horizontal" H 9850 2350 50  0001 C CNN
F 3 "~" H 9850 2350 50  0001 C CNN
	1    9850 2350
	1    0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR0107
U 1 1 6047ED42
P 6150 3250
F 0 "#PWR0107" H 6150 3100 50  0001 C CNN
F 1 "+3.3V" V 6165 3378 50  0000 L CNN
F 2 "" H 6150 3250 50  0001 C CNN
F 3 "" H 6150 3250 50  0001 C CNN
	1    6150 3250
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 6047FB19
P 6600 3400
F 0 "#PWR0108" H 6600 3150 50  0001 C CNN
F 1 "GND" H 6605 3227 50  0000 C CNN
F 2 "" H 6600 3400 50  0001 C CNN
F 3 "" H 6600 3400 50  0001 C CNN
	1    6600 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3250 6050 3250
Wire Wire Line
	6600 3150 6600 3400
Wire Wire Line
	6050 3150 6600 3150
$Comp
L Device:R R3
U 1 1 60481340
P 6450 1350
F 0 "R3" H 6520 1396 50  0000 L CNN
F 1 "100k" H 6520 1305 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 6380 1350 50  0001 C CNN
F 3 "~" H 6450 1350 50  0001 C CNN
	1    6450 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D2
U 1 1 6048239C
P 6750 1350
F 0 "D2" V 6704 1430 50  0000 L CNN
F 1 "D_Schottky" V 6795 1430 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P2.54mm_Vertical_AnodeUp" H 6750 1350 50  0001 C CNN
F 3 "~" H 6750 1350 50  0001 C CNN
	1    6750 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	6600 2750 6600 1500
Wire Wire Line
	6600 1500 6450 1500
Wire Wire Line
	6600 1500 6750 1500
Connection ~ 6600 1500
$Comp
L power:+3.3V #PWR0109
U 1 1 60483FEA
P 6600 1000
F 0 "#PWR0109" H 6600 850 50  0001 C CNN
F 1 "+3.3V" H 6615 1173 50  0000 C CNN
F 2 "" H 6600 1000 50  0001 C CNN
F 3 "" H 6600 1000 50  0001 C CNN
	1    6600 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2750 6600 2800
Wire Wire Line
	6600 3100 6600 3150
Connection ~ 6600 3150
$Comp
L Device:R R4
U 1 1 60485635
P 6600 2950
F 0 "R4" H 6670 2996 50  0000 L CNN
F 1 "100k" H 6670 2905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 6530 2950 50  0001 C CNN
F 3 "~" H 6600 2950 50  0001 C CNN
	1    6600 2950
	1    0    0    -1  
$EndComp
Connection ~ 6600 2750
Wire Wire Line
	6050 2750 6600 2750
$Comp
L Device:R R9
U 1 1 6048803F
P 7100 3100
F 0 "R9" V 6893 3100 50  0000 C CNN
F 1 "10" V 6984 3100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7030 3100 50  0001 C CNN
F 3 "~" H 7100 3100 50  0001 C CNN
	1    7100 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	6950 3100 6850 3100
Wire Wire Line
	6850 3100 6850 2750
Wire Wire Line
	6850 2750 6600 2750
$Comp
L Device:R R10
U 1 1 60489ED5
P 7350 2800
F 0 "R10" V 7143 2800 50  0000 C CNN
F 1 "220" V 7234 2800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7280 2800 50  0001 C CNN
F 3 "~" H 7350 2800 50  0001 C CNN
	1    7350 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	7200 2800 7000 2800
Wire Wire Line
	7000 2800 7000 2650
Wire Wire Line
	7000 2650 6050 2650
$Comp
L Device:R R11
U 1 1 6048B2DB
P 7400 1450
F 0 "R11" H 7470 1496 50  0000 L CNN
F 1 "100k" H 7470 1405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7330 1450 50  0001 C CNN
F 3 "~" H 7400 1450 50  0001 C CNN
	1    7400 1450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0110
U 1 1 6048B6F3
P 7400 1000
F 0 "#PWR0110" H 7400 850 50  0001 C CNN
F 1 "+3.3V" H 7415 1173 50  0000 C CNN
F 2 "" H 7400 1000 50  0001 C CNN
F 3 "" H 7400 1000 50  0001 C CNN
	1    7400 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 1000 7400 1300
Wire Wire Line
	6050 2550 7400 2550
Wire Wire Line
	7400 2550 7400 1600
$Comp
L Device:R R7
U 1 1 6048CB73
P 7000 2450
F 0 "R7" V 6793 2450 50  0000 C CNN
F 1 "220" V 6884 2450 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 6930 2450 50  0001 C CNN
F 3 "~" H 7000 2450 50  0001 C CNN
	1    7000 2450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 6048D668
P 6200 2450
F 0 "#PWR0111" H 6200 2200 50  0001 C CNN
F 1 "GND" V 6205 2322 50  0000 R CNN
F 2 "" H 6200 2450 50  0001 C CNN
F 3 "" H 6200 2450 50  0001 C CNN
	1    6200 2450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6200 2450 6050 2450
$Comp
L power:+3.3V #PWR0112
U 1 1 6048E4FD
P 6200 2350
F 0 "#PWR0112" H 6200 2200 50  0001 C CNN
F 1 "+3.3V" V 6215 2478 50  0000 L CNN
F 2 "" H 6200 2350 50  0001 C CNN
F 3 "" H 6200 2350 50  0001 C CNN
	1    6200 2350
	0    1    1    0   
$EndComp
Wire Wire Line
	6200 2350 6050 2350
Wire Wire Line
	6850 2450 6750 2450
Wire Wire Line
	6750 2450 6750 2250
Wire Wire Line
	6750 2250 6050 2250
$Comp
L Device:R R12
U 1 1 6048FE71
P 7700 2250
F 0 "R12" V 7493 2250 50  0000 C CNN
F 1 "220" V 7584 2250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7630 2250 50  0001 C CNN
F 3 "~" H 7700 2250 50  0001 C CNN
	1    7700 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	7200 2250 7200 2050
Wire Wire Line
	7200 2050 6050 2050
Wire Wire Line
	7200 2250 7550 2250
Wire Wire Line
	7850 2250 8000 2250
$Comp
L Device:R R13
U 1 1 60496EE9
P 7800 1450
F 0 "R13" H 7870 1496 50  0000 L CNN
F 1 "100k" H 7870 1405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7730 1450 50  0001 C CNN
F 3 "~" H 7800 1450 50  0001 C CNN
	1    7800 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D4
U 1 1 60497113
P 8100 1450
F 0 "D4" V 8054 1530 50  0000 L CNN
F 1 "D_Schottky" V 8145 1530 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P2.54mm_Vertical_AnodeUp" H 8100 1450 50  0001 C CNN
F 3 "~" H 8100 1450 50  0001 C CNN
	1    8100 1450
	0    1    1    0   
$EndComp
Wire Wire Line
	8100 1300 8100 1250
Wire Wire Line
	8100 1250 7950 1250
Wire Wire Line
	7800 1250 7800 1300
$Comp
L power:+3.3V #PWR0113
U 1 1 60497124
P 7950 1000
F 0 "#PWR0113" H 7950 850 50  0001 C CNN
F 1 "+3.3V" H 7965 1173 50  0000 C CNN
F 2 "" H 7950 1000 50  0001 C CNN
F 3 "" H 7950 1000 50  0001 C CNN
	1    7950 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 1000 7950 1250
Connection ~ 7950 1250
Wire Wire Line
	7950 1250 7800 1250
Wire Wire Line
	7800 1600 7950 1600
Wire Wire Line
	6050 1950 6750 1950
Wire Wire Line
	7950 1950 7950 1600
Connection ~ 7950 1600
Wire Wire Line
	7950 1600 8100 1600
Connection ~ 7950 1950
$Comp
L Device:R R16
U 1 1 6049DEB3
P 8350 1950
F 0 "R16" V 8143 1950 50  0000 C CNN
F 1 "10" V 8234 1950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 8280 1950 50  0001 C CNN
F 3 "~" H 8350 1950 50  0001 C CNN
	1    8350 1950
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 6049EA45
P 6900 1800
F 0 "R5" V 6693 1800 50  0000 C CNN
F 1 "100k" V 6784 1800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 6830 1800 50  0001 C CNN
F 3 "~" H 6900 1800 50  0001 C CNN
	1    6900 1800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0114
U 1 1 6049EEED
P 7100 1800
F 0 "#PWR0114" H 7100 1550 50  0001 C CNN
F 1 "GND" V 7105 1672 50  0000 R CNN
F 2 "" H 7100 1800 50  0001 C CNN
F 3 "" H 7100 1800 50  0001 C CNN
	1    7100 1800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7100 1800 7050 1800
Wire Wire Line
	6750 1800 6750 1950
Connection ~ 6750 1950
Wire Wire Line
	6750 1950 7950 1950
Wire Wire Line
	7950 1950 8200 1950
$Comp
L power:+5V #PWR0115
U 1 1 604A3DC7
P 8700 1450
F 0 "#PWR0115" H 8700 1300 50  0001 C CNN
F 1 "+5V" H 8715 1623 50  0000 C CNN
F 2 "" H 8700 1450 50  0001 C CNN
F 3 "" H 8700 1450 50  0001 C CNN
	1    8700 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 1850 9650 1850
Wire Wire Line
	9650 1950 8500 1950
Wire Wire Line
	8000 2250 8000 2050
Wire Wire Line
	8100 2450 8100 2150
Wire Wire Line
	8100 2150 9650 2150
Wire Wire Line
	7150 2450 8100 2450
Wire Wire Line
	8700 1450 8700 1550
Wire Wire Line
	8200 2550 8200 2250
Wire Wire Line
	8200 2250 9650 2250
Wire Wire Line
	7400 2550 8200 2550
Connection ~ 7400 2550
Wire Wire Line
	8300 2800 8300 2350
Wire Wire Line
	8300 2350 9650 2350
Wire Wire Line
	7500 2800 8300 2800
Wire Wire Line
	8400 3100 8400 2450
Wire Wire Line
	8400 2450 9650 2450
Wire Wire Line
	7250 3100 8400 3100
Wire Wire Line
	6450 1200 6450 1150
Wire Wire Line
	6450 1150 6600 1150
Wire Wire Line
	6750 1150 6750 1200
Wire Wire Line
	6600 1000 6600 1150
Connection ~ 6600 1150
Wire Wire Line
	6600 1150 6750 1150
$Comp
L Device:R R6
U 1 1 604CDE74
P 6900 3800
F 0 "R6" H 6970 3846 50  0000 L CNN
F 1 "100k" H 6970 3755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 6830 3800 50  0001 C CNN
F 3 "~" H 6900 3800 50  0001 C CNN
	1    6900 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D3
U 1 1 604CE110
P 7200 3800
F 0 "D3" V 7154 3880 50  0000 L CNN
F 1 "D_Schottky" V 7245 3880 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P2.54mm_Vertical_AnodeUp" H 7200 3800 50  0001 C CNN
F 3 "~" H 7200 3800 50  0001 C CNN
	1    7200 3800
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0116
U 1 1 604CE11E
P 7050 3450
F 0 "#PWR0116" H 7050 3300 50  0001 C CNN
F 1 "+3.3V" H 7065 3623 50  0000 C CNN
F 2 "" H 7050 3450 50  0001 C CNN
F 3 "" H 7050 3450 50  0001 C CNN
	1    7050 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 3650 6900 3600
Wire Wire Line
	6900 3600 7050 3600
Wire Wire Line
	7200 3600 7200 3650
Wire Wire Line
	7050 3450 7050 3600
Connection ~ 7050 3600
Wire Wire Line
	7050 3600 7200 3600
$Comp
L Device:R R14
U 1 1 604D28C6
P 7850 3800
F 0 "R14" H 7920 3846 50  0000 L CNN
F 1 "100k" H 7920 3755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7780 3800 50  0001 C CNN
F 3 "~" H 7850 3800 50  0001 C CNN
	1    7850 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D5
U 1 1 604D2B98
P 8150 3800
F 0 "D5" V 8104 3880 50  0000 L CNN
F 1 "D_Schottky" V 8195 3880 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P2.54mm_Vertical_AnodeUp" H 8150 3800 50  0001 C CNN
F 3 "~" H 8150 3800 50  0001 C CNN
	1    8150 3800
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR0117
U 1 1 604D2BA6
P 8000 3450
F 0 "#PWR0117" H 8000 3300 50  0001 C CNN
F 1 "+3.3V" H 8015 3623 50  0000 C CNN
F 2 "" H 8000 3450 50  0001 C CNN
F 3 "" H 8000 3450 50  0001 C CNN
	1    8000 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3650 7850 3600
Wire Wire Line
	7850 3600 8000 3600
Wire Wire Line
	8150 3600 8150 3650
Wire Wire Line
	8000 3450 8000 3600
Connection ~ 8000 3600
Wire Wire Line
	8000 3600 8150 3600
Wire Wire Line
	7850 3950 8000 3950
Wire Wire Line
	6900 3950 7050 3950
$Comp
L Device:R R8
U 1 1 604D84BB
P 7050 4600
F 0 "R8" H 7120 4646 50  0000 L CNN
F 1 "100k" H 7120 4555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 6980 4600 50  0001 C CNN
F 3 "~" H 7050 4600 50  0001 C CNN
	1    7050 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 604D8B0D
P 8000 4600
F 0 "R15" H 8070 4646 50  0000 L CNN
F 1 "100k" H 8070 4555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7930 4600 50  0001 C CNN
F 3 "~" H 8000 4600 50  0001 C CNN
	1    8000 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 2550 8800 4050
Wire Wire Line
	8800 4050 8000 4050
Wire Wire Line
	8000 4050 8000 3950
Connection ~ 8000 3950
Wire Wire Line
	8000 3950 8150 3950
Connection ~ 8000 4050
Wire Wire Line
	8000 4050 8000 4450
$Comp
L power:GND #PWR0118
U 1 1 604DFF43
P 8000 4900
F 0 "#PWR0118" H 8000 4650 50  0001 C CNN
F 1 "GND" H 8005 4727 50  0000 C CNN
F 2 "" H 8000 4900 50  0001 C CNN
F 3 "" H 8000 4900 50  0001 C CNN
	1    8000 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4750 8000 4900
$Comp
L power:GND #PWR0119
U 1 1 604E292F
P 7050 4950
F 0 "#PWR0119" H 7050 4700 50  0001 C CNN
F 1 "GND" H 7055 4777 50  0000 C CNN
F 2 "" H 7050 4950 50  0001 C CNN
F 3 "" H 7050 4950 50  0001 C CNN
	1    7050 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4750 7050 4950
Wire Wire Line
	7050 4450 7050 4250
Connection ~ 7050 3950
Wire Wire Line
	7050 3950 7200 3950
Wire Wire Line
	7050 4250 8950 4250
Wire Wire Line
	8950 4250 8950 2650
Wire Wire Line
	8950 2650 9650 2650
Connection ~ 7050 4250
Wire Wire Line
	7050 4250 7050 3950
Wire Wire Line
	8800 2550 9650 2550
$Comp
L power:GND #PWR0120
U 1 1 604F4246
P 9500 2900
F 0 "#PWR0120" H 9500 2650 50  0001 C CNN
F 1 "GND" H 9505 2727 50  0000 C CNN
F 2 "" H 9500 2900 50  0001 C CNN
F 3 "" H 9500 2900 50  0001 C CNN
	1    9500 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 2750 9500 2750
Wire Wire Line
	9500 2750 9500 2900
$Comp
L Device:CP C2
U 1 1 604F76A8
P 9250 1550
F 0 "C2" V 9505 1550 50  0000 C CNN
F 1 "100u" V 9414 1550 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 9288 1400 50  0001 C CNN
F 3 "~" H 9250 1550 50  0001 C CNN
	1    9250 1550
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP C1
U 1 1 604F8992
P 9250 1000
F 0 "C1" V 9505 1000 50  0000 C CNN
F 1 "100u" V 9414 1000 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 9288 850 50  0001 C CNN
F 3 "~" H 9250 1000 50  0001 C CNN
	1    9250 1000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9100 1550 8700 1550
Connection ~ 8700 1550
Wire Wire Line
	8700 1550 8700 1850
Wire Wire Line
	9400 1550 9400 1300
Wire Wire Line
	9100 1000 9100 1550
Connection ~ 9100 1550
$Comp
L power:GND #PWR0121
U 1 1 60502853
P 9500 1300
F 0 "#PWR0121" H 9500 1050 50  0001 C CNN
F 1 "GND" V 9505 1172 50  0000 R CNN
F 2 "" H 9500 1300 50  0001 C CNN
F 3 "" H 9500 1300 50  0001 C CNN
	1    9500 1300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9500 1300 9400 1300
Connection ~ 9400 1300
Wire Wire Line
	9400 1300 9400 1000
NoConn ~ 6150 1850
NoConn ~ 6150 2150
NoConn ~ 6150 2850
NoConn ~ 6150 2950
NoConn ~ 6150 3050
NoConn ~ 2850 2950
NoConn ~ 2850 3050
Wire Wire Line
	2950 2950 2850 2950
Wire Wire Line
	2950 3050 2850 3050
Wire Wire Line
	6050 3050 6150 3050
Wire Wire Line
	6050 2950 6150 2950
Wire Wire Line
	6050 2850 6150 2850
Wire Wire Line
	6050 1850 6150 1850
Wire Wire Line
	6050 2150 6150 2150
$Comp
L Connector_Generic:Conn_01x06 J1
U 1 1 60535C70
P 1900 2550
F 0 "J1" H 1818 2025 50  0000 C CNN
F 1 "Ext SPI" H 1818 2116 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 1900 2550 50  0001 C CNN
F 3 "~" H 1900 2550 50  0001 C CNN
	1    1900 2550
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J2
U 1 1 60548E6C
P 2800 1350
F 0 "J2" H 2950 1250 50  0000 R CNN
F 1 "Lum" H 2950 1500 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 2800 1350 50  0001 C CNN
F 3 "~" H 2800 1350 50  0001 C CNN
	1    2800 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 1350 3600 1350
Wire Wire Line
	3300 1350 3200 1350
Wire Wire Line
	2950 1850 2950 1600
Wire Wire Line
	2950 1600 3200 1600
Wire Wire Line
	3200 1600 3200 1350
Connection ~ 3200 1350
Wire Wire Line
	3200 1350 3000 1350
Wire Wire Line
	2950 2350 2100 2350
Wire Wire Line
	2950 2450 2100 2450
Wire Wire Line
	2100 2550 2950 2550
Wire Wire Line
	2950 2650 2100 2650
Wire Wire Line
	2100 2750 2950 2750
Wire Wire Line
	2100 3000 2100 2750
Connection ~ 2100 2750
$Comp
L power:+3.3V #PWR0122
U 1 1 6058811F
P 2250 2250
F 0 "#PWR0122" H 2250 2100 50  0001 C CNN
F 1 "+3.3V" V 2265 2378 50  0000 L CNN
F 2 "" H 2250 2250 50  0001 C CNN
F 3 "" H 2250 2250 50  0001 C CNN
	1    2250 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 2250 2100 2250
Wire Wire Line
	2950 2250 2650 2250
Wire Wire Line
	2650 2250 2650 1900
Wire Wire Line
	2650 1900 1450 1900
Wire Wire Line
	1450 1900 1450 4050
Wire Wire Line
	1450 4050 8000 4050
Wire Wire Line
	7050 4250 3450 4250
Wire Wire Line
	1300 4250 1300 1750
Wire Wire Line
	1300 1750 2700 1750
Wire Wire Line
	2700 1750 2700 2150
Wire Wire Line
	2700 2150 2950 2150
NoConn ~ 2800 1950
NoConn ~ 2800 2050
Wire Wire Line
	2950 1950 2800 1950
Wire Wire Line
	2800 2050 2950 2050
$Comp
L Connector:Conn_01x01_Male J3
U 1 1 605BA4BE
P 3450 4700
F 0 "J3" V 3450 4950 50  0000 R CNN
F 1 "Temp" V 3300 4750 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 3450 4700 50  0001 C CNN
F 3 "~" H 3450 4700 50  0001 C CNN
	1    3450 4700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3450 4500 3450 4250
Connection ~ 3450 4250
Wire Wire Line
	3450 4250 1300 4250
Text Notes 3400 5000 0    50   ~ 0
Vers DHT Ext.
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 605C3F86
P 1350 5550
F 0 "#FLG0101" H 1350 5625 50  0001 C CNN
F 1 "PWR_FLAG" H 1350 5723 50  0000 C CNN
F 2 "" H 1350 5550 50  0001 C CNN
F 3 "~" H 1350 5550 50  0001 C CNN
	1    1350 5550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0123
U 1 1 605C457D
P 1350 5700
F 0 "#PWR0123" H 1350 5550 50  0001 C CNN
F 1 "+5V" H 1365 5873 50  0000 C CNN
F 2 "" H 1350 5700 50  0001 C CNN
F 3 "" H 1350 5700 50  0001 C CNN
	1    1350 5700
	-1   0    0    1   
$EndComp
Wire Wire Line
	1350 5550 1350 5700
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 605CABDA
P 1900 5550
F 0 "#FLG0102" H 1900 5625 50  0001 C CNN
F 1 "PWR_FLAG" H 1900 5723 50  0000 C CNN
F 2 "" H 1900 5550 50  0001 C CNN
F 3 "~" H 1900 5550 50  0001 C CNN
	1    1900 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0124
U 1 1 605CAF48
P 1900 5750
F 0 "#PWR0124" H 1900 5500 50  0001 C CNN
F 1 "GND" H 1905 5577 50  0000 C CNN
F 2 "" H 1900 5750 50  0001 C CNN
F 3 "" H 1900 5750 50  0001 C CNN
	1    1900 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 5550 1900 5750
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 605D1864
P 2400 5550
F 0 "#FLG0103" H 2400 5625 50  0001 C CNN
F 1 "PWR_FLAG" H 2400 5723 50  0000 C CNN
F 2 "" H 2400 5550 50  0001 C CNN
F 3 "~" H 2400 5550 50  0001 C CNN
	1    2400 5550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0125
U 1 1 605D1B14
P 2400 5750
F 0 "#PWR0125" H 2400 5600 50  0001 C CNN
F 1 "+3.3V" H 2415 5923 50  0000 C CNN
F 2 "" H 2400 5750 50  0001 C CNN
F 3 "" H 2400 5750 50  0001 C CNN
	1    2400 5750
	-1   0    0    1   
$EndComp
Wire Wire Line
	2400 5550 2400 5750
Wire Wire Line
	8000 2050 9650 2050
$Comp
L Device:CP C3
U 1 1 60521752
P 3250 3600
F 0 "C3" V 3505 3600 50  0000 C CNN
F 1 "100u" V 3414 3600 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P5.00mm" H 3288 3450 50  0001 C CNN
F 3 "~" H 3250 3600 50  0001 C CNN
	1    3250 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 60523025
P 3250 3800
F 0 "#PWR01" H 3250 3550 50  0001 C CNN
F 1 "GND" H 3255 3627 50  0000 C CNN
F 2 "" H 3250 3800 50  0001 C CNN
F 3 "" H 3250 3800 50  0001 C CNN
	1    3250 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3750 3250 3800
Wire Wire Line
	3250 3450 2950 3450
Connection ~ 2950 3450
Wire Wire Line
	2950 3450 2950 3500
Text Notes 7300 7250 0    50   ~ 0
1
Text Notes 7400 7250 0    50   ~ 0
1
Text Notes 7400 7500 0    50   ~ 0
CABANE DE MINOU (Bas/Tour/...)
Text Notes 8150 7650 0    50   ~ 0
28/03/2021
Text Notes 10600 7650 0    50   ~ 0
A0
$EndSCHEMATC
