//Basic TX using nrf24l01

/*
 * NRF24L01       Arduino_ Uno    Arduino_Mega    Blue_Pill(stm32f01C)
 * __________________________________________________________________________
 * VCC        |   3.3v          | 3.3v          |     3.3v
 * GND        |   GND           | GND           |     GND
 * CSN        |   Pin10 SPI/SS  | Pin10 SPI/SS  |     B0 NSS1 (PB0) 3.3v
 * CE         |   Pin9          | Pin9          |     B1 digital (PB1) 3.3v
 * SCK        |   Pin13         | Pin52         |     A5 SCK1   (PA5) 3.3v
 * MOSI       |   Pin11         | Pin51         |     A7 MOSI1  (PA7) 3.3v
 * MISO       |   Pin12         | Pin50         |     A6 MISO1  (PA6) 3.3v
 * 
 * 
 */

//Pin Configuration
#ifdef BOARD_UNO
#define CE 9
#define CSN 10
#define button_pin 5
#define led_pin 6
#define BoardLED 13
#endif

#ifdef BOARD_STM32
#define CE PB1
#define CSN PB0
#define button_pin PB10
#define led_pin PB11
#define BoardLED PC13
#endif



#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



RF24 radio(CE, CSN);
//const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
uint16_t address = 0x99;
uint16_t CODE = 0xF0F0;
bool button_state = 0;
void setup()
{
  pinMode(BoardLED, OUTPUT);
  pinMode(button_pin, INPUT);
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
  Serial.begin(9600);
  //const char text[] = "11001100";
}
void loop()
{
  radio.write(&CODE, sizeof(CODE));
  digitalWrite(BoardLED, LOW);
  delay(5);
  digitalWrite(BoardLED, HIGH);
  delay(45);

  button_state = digitalRead(button_pin);
  Serial.print(button_state);
  if (button_state == HIGH)
  {
    Serial.println(" HIGH");
    //const char text[] = "Your Button State is HIGH";
    const char text[] = "11001100";

    radio.write(&text, sizeof(text)); //Sending the message to receiver
  }
  else
  {

    Serial.println(" LOW");
    //const char text[] = "Your Button State is LOW";
    //const char text[] = "11001100";
    //radio.write(&text, sizeof(text)); //Sending the message to receiver
  }
  //radio.write(&button_state, sizeof(button_state)); //Sending the message to receiver
  delay(100);
}
