# Hardware_Code



## Use arduino to program FTDI/Serial devices

### First config Arduino as and of the three

1. Connect RESET pin to GND. This is easiest.

2. Remove the Atmel Chip

3. upload this sketch

   ```cpp
   void setup(){
     pinMode(0,INPUT);
     pinMode(1,INPUT);
   }
   void loop(){
   }
   ```

   

### Second connect with external board

1. connect power pins

2. then connect RX (arduino) pin to RX (external board)

3. and TX (arduino)  pin to TX(external board)

   