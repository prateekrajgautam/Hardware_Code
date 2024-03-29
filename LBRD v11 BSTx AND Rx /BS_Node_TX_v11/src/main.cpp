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

//NRF24L01
#define CE 9
#define CSN 10

//QMC I2C and MPU6050
//#define SCL A5
//#define SDA A4
#define INTERRUPT_PIN 2 // use pin 2 on Arduino Uno & most boards

//general
#define details 3
#define led_pin 4
#define button_pin 7
#define modePin 8
#define BoardLED 13

//Servo pwm driving pin
#define servoThetaPin 5
#define servoPhiPin 6

#endif

#ifdef BOARD_STM32

//NRF24L01
#define CE PB1
#define CSN PB0

//QMC I2C and MPU6050
//#define SCL PB8
//#define SDA PB9
#define INTERRUPT_PIN PA2 // use pin 2 on Arduino Uno & most boards not known for stm32

//general
#define details PA5
#define button_pin PB10
#define modePin PB11
#define BoardLED PC13

//Servo pwm driving pin
#define servoThetaPin PA3
#define servoPhiPin PA4

#endif

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>
//#include <Servo.h> //add '<' and '>' before and after servo.h
#include <PWMServo.h> //add '<' and '>' before and after servo.h
#include <QMC5883LCompass.h>
//#include <MPU6050.h>

RF24 radio(CE, CSN);
//Servo servoPhi, servoTheta;
PWMServo servoPhi, servoTheta;

//compass for phi
QMC5883LCompass compass;
int16_t PHI = 1000;
int16_t QMC[4];
int16_t *p2 = QMC;

int16_t readQMC(const int printStatus, int16_t *QMC)
{
  compass.read();
  QMC[0] = compass.getX();
  QMC[1] = compass.getY();
  QMC[2] = compass.getZ();
  QMC[3] = compass.getAzimuth();
  if (printStatus == LOW)
  {
    Serial.println("QMC5883L");
    Serial.print("X: ");
    Serial.print(QMC[0]);
    Serial.print(" Y: ");
    Serial.print(QMC[1]);
    Serial.print(" Z: ");
    Serial.print(QMC[2]);
    Serial.print(" A: ");
    Serial.print(QMC[3]);
    Serial.println();
  }
  return QMC[3];
}

//gyro mpu6050 for theta
int16_t THETA = 1000;

//MPU6050 mpu;

int16_t FLAG = 0;

//const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//int address = 0x99;
int DestinationADDRESS = 0x99; //Rx
int ListeningADDRESS = 0xFF;   //Tx or BS
const uint16_t CODE = 0x7F0F;
uint16_t B[10] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
uint16_t(*ptrB) = B;
int sizeOfB = sizeof(B);
uint16_t C[10];
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

//B[4] = CODE;

/* in new code
const int DestinationADDRESS = 0x99;
const int ListeningADDRESS = 0xFF;
int CODE = 0x7F0F;
*/

void fakeSerialClear()
{
  Serial.print("\t \n \t \n \t \n \t \n \t \n "); // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n "); // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n "); // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n "); // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n "); // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n "); // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n "); // clear 5 lines
}

//void printHello();

double mod(double x, double y)
{
  return x - (int)(x / y) * y;
}

bool button_state = 0;
int printDetails;
void setup()
{

  Serial.begin(9600);
  pinMode(BoardLED, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(details, INPUT_PULLUP);
  pinMode(modePin, INPUT_PULLUP);

  //radio.begin();                  //Starting the Wireless communication
  //radio.openWritingPipe(address); //Setting the address where we will send the data
  //radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  //radio.stopListening();          //This sets the module as transmitter

  radio.begin();                              //Starting the Wireless communication
  radio.openWritingPipe(DestinationADDRESS);  //Setting the address where we will send the data
  radio.openReadingPipe(0, ListeningADDRESS); //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);              //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();                      //This sets the module as transmitter
  radio.printDetails();

  bool check = radio.isChipConnected();
  Serial.print("NRF24L01 Chip Connected?:");
  Serial.println(check);
  delay(100);

  //const char text[] = "11001100";

  servoPhi.attach(servoPhiPin);
  servoTheta.attach(servoThetaPin);
  servoPhi.write(90);
  servoTheta.write(90);

  compass.init();
}

void transmitBeacon(int printDetails, uint16_t *B)
{

  if (printDetails == LOW)
  {
    Serial.print("\nTransmitting Beacon :");
    for (int i = 0; i < 10; i++)
    {
      Serial.print(B[i]);
      Serial.print(" ");
    }
    //Serial.print("\n size of beacon : ");
    //Serial.println(sizeOfB);
  }
  radio.stopListening();      //this is not working here in function
  radio.write(&B, sizeof(B)); //this is not working here in function
}

void printBeacon(uint16_t *B)
{

  Serial.print("\nTransmitting Beacon :");
  for (int i = 0; i < 10; i++)
  {
    Serial.print(B[i]);
    Serial.print(" ");
  }
  Serial.print("\n size of beacon : ");
  Serial.println(sizeOfB);
  //radio.stopListening();
  //radio.write(&B, sizeOfB);
}

void loop()
{
  printDetails = digitalRead(details);
  radio.stopListening();
  //PHI read from QMC
  PHI = readQMC(printDetails, p2);

  //convert
  PHI = mod((PHI + 360 - 90.0), 360.0); //90* shift, Y points to DA

  //BEACON ENCODING
  B[0] = 0x70FF;

  B[3] = DestinationADDRESS;
  B[4] = CODE;
  B[5] = THETA;
  B[6] = PHI;

  B[9] = FLAG;

  //BEACON TRANSMISSION
  B[4] = CODE;
  /*
  //radio.write(&B, sizeof(B)); //Transmit Beacon
  Serial.print("\n Transmit Blank Beacon");
  Serial.print("\nMathing CODE=");
  Serial.print(CODE);
  Serial.print("\n");

*/

  digitalWrite(BoardLED, LOW);
  delay(5);
  digitalWrite(BoardLED, HIGH);
  delay(45);
  /*
  button_state = digitalRead(button_pin);
  Serial.print(button_state);
  if (button_state == HIGH)
  {
    Serial.println(" HIGH");
    //const char text[] = "Your Button State is HIGH";
   // const char text[] = "11001100";

   // radio.write(&text, sizeof(text)); //Sending the message to receiver
  }
  else
  {

    Serial.println("LOW");
    //const char text[] = "Your Button State is LOW";
    //const char text[] = "11001100";
    //radio.write(&text, sizeof(text)); //Sending the message to receiver
  }
  //radio.write(&button_state, sizeof(button_state)); //Sending the message to receiver
  */
  delay(50);

  //SERVO CONTROL
  if (true)
  {
    //Serial.print("\n Controling Servo \n");
    bool mode = digitalRead(modePin);
    Serial.print("\n MODE \t");
    Serial.print(mode);
    Serial.print("\n");
    if (mode == LOW)
    {
      Serial.print("\n Rotation On \n");
      servoPhi.write(90); //stop
      //while (abs(PHI - phi0) < dp)
      {
        // read PHI
        //PHI = readQMC(0, p2);
        //PHI = mod((PHI + 360 - 90.0), 360.0); //90* shift, Y points to DA
        servoPhi.write(180); //move 45in 50ms
        servoTheta.write(180); //move 45in 50ms
        delay(100);
        //servoPhi.write(90);

        // servoPhi.write(135);
        //delay(50);
        //B[6] = PHI; //update beacon and retransmit
        //TRANSMIT BEACON B
        radio.stopListening();
        radio.write(&B, sizeof(B));
        printBeacon(ptrB);
      }
      servoPhi.write(90); //stop
      servoTheta.write(90);
    }
  }

  //Print Detailed Info of sensor readings and beacon
  if (true)
  {
    if (printDetails == LOW)
    {
      Serial.print("\n BEACON TxD: \t");
      for (int i = 0; i < 10; i++)
      {
        if (i == 5 || i == 6)
          Serial.print(B[i], DEC);
        else
          Serial.print(B[i], HEX);
        Serial.print(" ");
      }
      Serial.print("\n Theta: ");
      Serial.print(THETA);
      Serial.print("; Phi: ");
      Serial.print(PHI);
      Serial.print("\n");
    }
  }
  //RECEIVER MODE
  if (true)
  {
    // Serial.print("Listening to Node\n");
    radio.startListening();

    //====
    //CHECK AND READ RADIO
    if (radio.available()) //Looking for the data.
    {
      radio.read(&C, sizeof(C));
      float rho, x, y;
      rho = C[4];
      x = C[5];
      y = C[6];

      Serial.print("\n ===================================");
      //printf("\n PHI, BW \t%d \t DIR. %d",C[0],C[1]);
      //printf("\n THETA, BW \t%d \t DIR. %d",C[2],C[3]);

      Serial.println("");
      Serial.print("PHI  ");
      Serial.print(", DIR ");
      Serial.print(C[0]);
      Serial.print(", BW ");
      Serial.println(C[1]);

      //Serial.println("");
      Serial.print("THETA");
      Serial.print(", DIR ");
      Serial.print(C[2]);
      Serial.print(", BW ");
      Serial.println(C[3]);

      //Serial.println("");
      Serial.print("RHO: ");
      Serial.print(rho);
      Serial.print(", X: ");
      Serial.print(x);
      Serial.print(", Y: ");
      Serial.println(y);
      Serial.print("\n");
    }
  }

  //TRANSMIT BEACON B each iteration
  radio.stopListening();
  radio.write(&B, sizeof(B));
  printBeacon(ptrB);
  delay(50);
  //Serial.print("\nLoop End\n");
}
