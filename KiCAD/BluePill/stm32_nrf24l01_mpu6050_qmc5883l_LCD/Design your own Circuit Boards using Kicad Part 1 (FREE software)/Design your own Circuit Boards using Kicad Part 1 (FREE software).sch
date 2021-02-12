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
L Timer:LM555xM U1
U 1 1 60263B9C
P 4600 3000
F 0 "U1" H 4600 3581 50  0000 C CNN
F 1 "LM555xM" H 4600 3490 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 5450 2600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm555.pdf" H 5450 2600 50  0001 C CNN
	1    4600 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R1
U 1 1 60265DFA
P 6000 2150
F 0 "R1" H 6068 2196 50  0000 L CNN
F 1 "R_US" H 6068 2105 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P5.08mm_Vertical" V 6040 2140 50  0001 C CNN
F 3 "~" H 6000 2150 50  0001 C CNN
	1    6000 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 60266601
P 6450 2800
F 0 "R2" V 6245 2800 50  0000 C CNN
F 1 "R_US" V 6336 2800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P5.08mm_Vertical" V 6490 2790 50  0001 C CNN
F 3 "~" H 6450 2800 50  0001 C CNN
	1    6450 2800
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R3
U 1 1 60266A75
P 6450 3350
F 0 "R3" V 6245 3350 50  0000 C CNN
F 1 "R_US" V 6336 3350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P5.08mm_Vertical" V 6490 3340 50  0001 C CNN
F 3 "~" H 6450 3350 50  0001 C CNN
	1    6450 3350
	0    1    1    0   
$EndComp
$Comp
L pspice:CAP C1
U 1 1 60267259
P 4950 4200
F 0 "C1" H 5128 4246 50  0000 L CNN
F 1 "CAP" H 5128 4155 50  0000 L CNN
F 2 "Capacitor_THT:CP_Axial_L10.0mm_D4.5mm_P15.00mm_Horizontal" H 4950 4200 50  0001 C CNN
F 3 "~" H 4950 4200 50  0001 C CNN
	1    4950 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 602684A7
P 7050 3050
F 0 "D1" V 7089 2932 50  0000 R CNN
F 1 "LED" V 6998 2932 50  0000 R CNN
F 2 "LED_THT:LED_D5.0mm" H 7050 3050 50  0001 C CNN
F 3 "~" H 7050 3050 50  0001 C CNN
	1    7050 3050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5100 2800 6300 2800
Text GLabel 5100 3000 2    50   Input ~ 0
1
Text GLabel 6000 2300 2    50   Input ~ 0
1
Text GLabel 6600 3350 2    50   Input ~ 0
1
Wire Wire Line
	6600 2800 7050 2800
Wire Wire Line
	7050 2800 7050 2900
Wire Wire Line
	4600 2600 4600 2250
Wire Wire Line
	4600 2250 3800 2250
Wire Wire Line
	3800 2250 3800 3200
Wire Wire Line
	3800 3200 4100 3200
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 6027DD86
P 2700 2100
F 0 "J1" H 2618 1867 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 2780 2001 50  0001 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 2618 1866 50  0001 C CNN
F 3 "~" H 2700 2100 50  0001 C CNN
	1    2700 2100
	-1   0    0    1   
$EndComp
Text Notes 2700 1950 0    118  ~ 24
+\n
Text Notes 2700 2250 0    118  ~ 24
-
Text Notes 2600 1750 0    118  ~ 0
9v input\n
$Comp
L power:+9V #PWR0101
U 1 1 60281AAA
P 3000 1950
F 0 "#PWR0101" H 3000 1800 50  0001 C CNN
F 1 "+9V" H 3015 2123 50  0000 C CNN
F 2 "" H 3000 1950 50  0001 C CNN
F 3 "" H 3000 1950 50  0001 C CNN
	1    3000 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1950 3000 2000
Wire Wire Line
	3000 2000 2900 2000
$Comp
L power:GND #PWR0102
U 1 1 602821F5
P 3000 2150
F 0 "#PWR0102" H 3000 1900 50  0001 C CNN
F 1 "GND" H 3005 1977 50  0000 C CNN
F 2 "" H 3000 2150 50  0001 C CNN
F 3 "" H 3000 2150 50  0001 C CNN
	1    3000 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 2100 3000 2100
Wire Wire Line
	3000 2100 3000 2150
$Comp
L power:+9V #PWR0103
U 1 1 602827BC
P 4600 2100
F 0 "#PWR0103" H 4600 1950 50  0001 C CNN
F 1 "+9V" H 4615 2273 50  0000 C CNN
F 2 "" H 4600 2100 50  0001 C CNN
F 3 "" H 4600 2100 50  0001 C CNN
	1    4600 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2100 4600 2250
Connection ~ 4600 2250
Wire Wire Line
	4100 2800 3450 2800
Wire Wire Line
	3450 2800 3450 3950
Wire Wire Line
	3450 3950 4950 3950
Wire Wire Line
	5100 3200 5100 3950
Wire Wire Line
	5100 3950 4950 3950
Connection ~ 4950 3950
Wire Wire Line
	5100 3200 5950 3200
Wire Wire Line
	5950 3200 5950 3350
Wire Wire Line
	5950 3350 6300 3350
Connection ~ 5100 3200
$Comp
L power:GND #PWR0104
U 1 1 60283F94
P 4950 4450
F 0 "#PWR0104" H 4950 4200 50  0001 C CNN
F 1 "GND" H 4955 4277 50  0000 C CNN
F 2 "" H 4950 4450 50  0001 C CNN
F 3 "" H 4950 4450 50  0001 C CNN
	1    4950 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 602843AA
P 4600 3400
F 0 "#PWR0105" H 4600 3150 50  0001 C CNN
F 1 "GND" H 4605 3227 50  0000 C CNN
F 2 "" H 4600 3400 50  0001 C CNN
F 3 "" H 4600 3400 50  0001 C CNN
	1    4600 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 60284695
P 7050 3200
F 0 "#PWR0106" H 7050 2950 50  0001 C CNN
F 1 "GND" H 7055 3027 50  0000 C CNN
F 2 "" H 7050 3200 50  0001 C CNN
F 3 "" H 7050 3200 50  0001 C CNN
	1    7050 3200
	1    0    0    -1  
$EndComp
$Comp
L power:+9V #PWR0107
U 1 1 602851CC
P 6000 2000
F 0 "#PWR0107" H 6000 1850 50  0001 C CNN
F 1 "+9V" H 6015 2173 50  0000 C CNN
F 2 "" H 6000 2000 50  0001 C CNN
F 3 "" H 6000 2000 50  0001 C CNN
	1    6000 2000
	1    0    0    -1  
$EndComp
$EndSCHEMATC
