# Program ESP-01 using arduino nano

## connections



**NANO RST --- NANO GND** *to bypass nano and act as usb to serial*

| NANO | ESP-01 |
| ---- | ------ |
| 3.3V | VCC    |
| GND  | GND    |
| TX   | TX     |
| RX   | RX     |
| GND  | GPIO0  |
| 3.3V | CH_PD  |
|      |        |

- before uploading programme disconnect connection from GPIO0 and and do same with CH_PD
- Wait for the code to be uploaded to ESP8266.
- When you see. ' leaving..  hard resting via RST pin', disconnect the serial port (GPIO 0).
   then connect the RESET (EXT_RSTB) of ESP8266 to ground for 1 second and then release it.

### After programming 

| ESP-01        | Connection |
| ------------- | ---------- |
| VCC           | 3.3V       |
| GND           | GND        |
| CH_PD (CH_EN) | 3.3V       |
|               |            |
|               |            |
|               |            |