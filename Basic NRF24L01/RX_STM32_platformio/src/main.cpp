//Basic RX using nrf24l01

/*
 * NRF24L01       Arduino_ Uno    Arduino_Mega    Blue_Pill(stm32f01C)
 * __________________________________________________________________________
 * VCC        |   3.3v          | 3.3v          |     3.3v
 * GND        |   GND           | GND           |     GND
 * CSN        |   Pin10 SPI/SS  | Pin10 SPI/SS  |     A4 NSS1 (PA4) 3.3v
 * CE         |   Pin9          | Pin9          |     B0 digital (PB0) 3.3v
 * SCK        |   Pin13         | Pin52         |     A5 SCK1   (PA5) 3.3v
 * MOSI       |   Pin11         | Pin51         |     A7 MOSI1  (PA7) 3.3v
 * MISO       |   Pin12         | Pin50         |     A6 MISO1  (PA6) 3.3v
 * 
 * 
 */
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//RF24 radio(9, 10); // CE, CSN for UNO
RF24 radio(PB12, PB13); // CE, CSN for STM32
//const byte address[6] = "00001";
const uint16_t address = 0x99;
int button_pin = PB10;
//int CODE_KEY=11001100;
const uint16_t CODE_KEY = 0xF0F0;
//bool button_state = 0;
int button_state = 0;
//int led_pin = 6;//UNO
int led_pin = PB11; //STM
int BoardLED = PC13;
void setup()
{
  pinMode(led_pin, OUTPUT);
  pinMode(BoardLED, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address); //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);     //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();            //This sets the module as receiver
}
void loop()
{
  if (radio.available()) //Looking for the data.
  {
    digitalWrite(BoardLED, LOW);
    delay(5);
    digitalWrite(BoardLED, HIGH);
    delay(45);
    char text[32] = "00000000"; //Saving the incoming data
    uint16_t CODE = 0x0000;
    radio.read(&CODE, sizeof(CODE));
    radio.read(&text, sizeof(text));                 //Reading the data
    Serial.print(text);
    radio.read(&button_state, sizeof(button_state)); //Reading the data

    Serial.print("Beacon Received");
    if (CODE == CODE_KEY)
    {
      digitalWrite(led_pin, HIGH);
      delay(10);
      digitalWrite(led_pin, LOW);
      delay(40);
    }
    else
    {
      Serial.println(" CODE missMatch");
      Serial.print(" CODE Received: ");
      Serial.print(CODE);
      Serial.print(" CODE KEY: ");
      Serial.println(CODE_KEY);
    }
  }
  else
  {
    Serial.println("No Signal");
  }
  delay(100);
}