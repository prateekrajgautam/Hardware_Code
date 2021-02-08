//BasicRX using nrf24l01
#include <Arduino.h>
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    RF24 radio(9, 10); // CE, CSN
    //const byte address[6] = "00001";
    const uint16_t address = 0x99;
    int button_pin = 6;
    //int CODE_KEY=11001100;
    const uint16_t CODE_KEY=0xF0F0;
    boolean button_state = 0;
    int led_pin = 6;
    
    void setup() {
    pinMode(led_pin, OUTPUT);
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
    radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
    radio.startListening();              //This sets the module as receiver
    }
    void loop()
    {
    if (radio.available())              //Looking for the data.
      {
      char text[32] = "00000000";                 //Saving the incoming data
      uint16_t CODE=0x0000;
      radio.read(&CODE, sizeof(CODE));
      radio.read(&text, sizeof(text));    //Reading the data
      radio.read(&button_state, sizeof(button_state));    //Reading the data
 
      Serial.print("Beacon Received");
      if(CODE==CODE_KEY) {
        Serial.println(" CODE Match");
        Serial.print(" CODE Received: ");
        Serial.print(CODE);
        Serial.print(" CODE KEY: ");
        Serial.println(CODE_KEY);
        digitalWrite(led_pin,HIGH);
        delay(5);
        digitalWrite(led_pin,LOW);
        }
        else
        {      
          Serial.println(" CODE missMatch");
          }
      }
      else
      {
        Serial.println("No Signal");
        }
    delay(100);
    }