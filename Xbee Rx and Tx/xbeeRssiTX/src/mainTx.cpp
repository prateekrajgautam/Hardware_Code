#include <Arduino.h>
//#include <SoftwareSerial.h>
#include <XBee.h>

// Create an XBee object at the top of your sketch
XBee xbee = XBee();


// Create an array for holding the data you want to send.
  uint8_t payload[] = {'H', 'i'};

  // Specify the address of the remote XBee (this is the SH + SL)
  //XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x403e0f30);
  XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x41C1D2D2);
  //address = 0xFFFF; // Boadcasting
void setup()
{

  // Start the serial port
  Serial.begin(9600);
  // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
  xbee.setSerial(Serial);
pinMode(LED_BUILTIN,OUTPUT);
  
}

void loop()
{
  // Create a TX Request
  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));

  // Send your request
  xbee.send(zbTx);
  digitalWrite(LED_BUILTIN,LOW);
  delay(5);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(45);
  //delay(50);
//Serial.print("\nTxd");
}