#include <Arduino.h>
#include <SoftwareSerial.h>
#include <XBee.h>

// Create an XBee object at the top of your sketch
XBee xbee = XBee();
Rx16Response rx16 = Rx16Response();

void setup()
{
  /*
  // Start the serial port
  Serial.begin(9600);
  // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
  xbee.setSerial(Serial);
  */
  pinMode(LED_BUILTIN, OUTPUT);

  //xbee.begin(9600);
  xbee.setSerial(Serial);
  Serial.begin(9600);
 
}
 int loopStart=0;
void loop()
{
  if(loopStart==0){
    loopStart=loopStart+1;
    Serial.print("\n\nLoop Started\n");
  }
  xbee.readPacket(100);
  if (xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE)
    {
      xbee.getResponse().getRx16Response(rx16);
      Serial.print(rx16.getRssi());
    }
  }
  digitalWrite(LED_BUILTIN,HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN,LOW);
  delay(10);
  
}
