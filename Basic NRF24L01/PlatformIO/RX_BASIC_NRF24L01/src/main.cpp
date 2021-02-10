//Basic RX using nrf24l01

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
int ListeningADDRESS = 0x99;//Rx
int DestinationADDRESS = 0xFF;//Tx or BS
//int CODE = 0x7F0F;

//const byte address[6] = "00001";
const int address = 0x99;

//int CODE_KEY=11001100;
const uint16_t CODE_KEY = 0x7F0F;
//int B[10] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};//Beacon
uint16_t  B[10];
/*
 //BEACON ENCODING
  B[0] = 0x70FF;
  B[3] = DestinationADDRESS;
  B[4] = CODE;
  B[5] = THETA;
  B[6] = PHI;
  B[9] = FLAG;
  //TRANSMIT BEACON B
  radio.write(&B, sizeof(B));
*/


//bool button_state = 0;
bool button_state = 0;
//int led_pin = 6;//UNO

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
  B[4] = '\0';
  uint16_t CODE = 0x00;
  if (radio.available()) //Looking for the data.
  {
    digitalWrite(BoardLED, LOW);
    delay(5);
    digitalWrite(BoardLED, HIGH);
    delay(45);
    //char text[32] = "00000000"; //Saving the incoming data
    
    //radio.read(&CODE, sizeof(CODE));
    radio.read(&B, sizeof(B));
    CODE = B[4];
    //radio.read(&text, sizeof(text)); //Reading the data
    //Serial.print(text);
    //radio.read(&button_state, sizeof(button_state)); //Reading the data

    Serial.print("Beacon Received\n");
    if (CODE == CODE_KEY)
    {
      digitalWrite(led_pin, HIGH);
      delay(10);
      digitalWrite(led_pin, LOW);
      delay(40);
      Serial.print(" CODE Received: ");
      Serial.print(CODE);
      Serial.print("        CODE KEY: ");
      Serial.println(CODE_KEY);
    }
    else
    {
      Serial.println(" CODE missMatch");
      Serial.print(" CODE Received: ");
      Serial.print(CODE);
      Serial.print("        CODE KEY: ");
      Serial.println(CODE_KEY);
    }
  }
  else
  {
    Serial.println("No Signal");
  }
  delay(50);

  Serial.println("\nBeacon Recieived\n");
  Serial.println(B[0]);
  Serial.println(B[2]);
  Serial.println(B[3]);
  Serial.println(B[4]);
  Serial.println(B[5]);
  Serial.println(B[6]);
  Serial.println(B[7]);
  Serial.println(B[8]);
  Serial.println(B[9]);
}
