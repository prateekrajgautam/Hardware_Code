//BS_NODE_9.ino
#include <Arduino.h>
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

#define led_pin 6
#define button_pin 7
#define modePin 8
#define BoardLED 13

#endif

#ifdef BOARD_STM32

//NRF24L01
#define CE PB1
#define CSN PB0

//QMC I2C and MPU6050
//#define SCL PB8
//#define SDA PB9
#define INTERRUPT_PIN 2 // use pin 2 on Arduino Uno & most boards not known for stm32

//general
#define button_pin PB10
#define modePin PB11
#define BoardLED PC13

#endif

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(CE, CSN);

//MPU inlcude
//+++#include "I2Cdev.h"

#define pi float(22 / 7)
//#define M_PI float(22/7)
#define d2r float(2 * pi / 360)
#define r2d float(360 / (2 * pi))

#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL

#define dt 1
#define dp 1
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;        // [w, x, y, z]         quaternion container
VectorInt16 aa;      // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;  // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity; // [x, y, z]            gravity vector
float euler[3];      // [psi, theta, phi]    Euler angle container
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

//           X Accel  Y Accel  Z Accel   X Gyro   Y Gyro   Z Gyro
//OFFSETS       62,     429,    3170,     126,       0,     -45

volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high
void dmpDataReady()
{
  mpuInterrupt = true;
}

//+++

//SCL->A5
//SDA->A4

#include <SPI.h>
#include "Wire.h"
#include <math.h>
#include <Arduino.h>
#include <Servo.h> //add '<' and '>' before and after servo.h

int servoThetaPin = 3, servoPhiPin = 4;

Servo servoPhi, servoTheta;

int B[10] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
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
float C[10];
int details = 5;

////MPU6050 mpu (Wire);
//SCL->A5
//SDA->A4

int FLAG = 0;
struct QMC5883L_CONFIG
{
  int SCL; //A5
  int SDA; //A4
};

QMC5883L_CONFIG QMC5883L_PIN = {SCL, SDA};
#include <QMC5883LCompass.h>
QMC5883LCompass compass;
int16_t QMC[4];
int16_t *p2 = QMC;

int16_t readQMC(const int printStatus, int16_t *QMC)
{
  compass.read();
  QMC[0] = compass.getX();
  QMC[1] = compass.getY();
  QMC[2] = compass.getZ();
  QMC[3] = compass.getAzimuth();
  if (printStatus == 1)
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

int16_t MCU[7];
//int16_t readMCU(int printStatus,int16_t &MCU);{  }

#include <nRF24L01.h>
#include <RF24.h>

#include <printf.h>
/*
//cofig for NRF24
int i;
struct NFR24L01_CONFIG
{
  int CE;//9
  int CSN;//10
  int MO;//13
  int MI;//11
  int SCK;//12
  int DestinationADDRESS;
  int ListeningADDRESS;
  int CODE;
};
NFR24L01_CONFIG NFR24L01_PIN = {9, 10, 13, 11, 12, 0x99, 0xFF, 0x7F0F};

struct ADXL335_CONFIG
{
  int x;
  int y;
  int z;
};
ADXL335_CONFIG ADXL335_PIN = {A0, A1, A2};
*/
//
/*
  struct HMC5883L_CONFIG
  {
  int SCL;
  int SCA;
  int DRDY;
  };

  HMC5883L_CONFIG HMC5883L_PIN = {SCL, SDA, A3};
*/

const int MPU_ADDR = 0x68;
int16_t GY_521[7];
char tmp_str[7]; // temporary variable used in convert function

char *convert_int16_to_str(int16_t i)
{ // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

//const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//const uint16_t address[sizeof(NFR24L01_PIN.DestinationADDRESS)] = {NFR24L01_PIN.DestinationADDRESS}; //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//const int DestinationADDRESS = {NFR24L01_PIN.DestinationADDRESS}; //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//const int ListeningADDRESS = {NFR24L01_PIN.ListeningADDRESS};
//int CODE = NFR24L01_PIN.CODE;

const int DestinationADDRESS = 0x99;
const int ListeningADDRESS = 0xFF;
const int CODE = 0x7F0F;

bool button_state = 0;

// config gy51 adxl
const int x = A0;
const int y = A1;
const int z = A2;
// initialize minimum and maximum Raw Ranges for each axis
//int ADXL_RAW_Min[3] = {0,0,0};
//int ADXL_RAW_Max[3] = {1023,1023,1023};
int ADXL_RAW_Min[3] = {296, 297, 296};
int ADXL_RAW_Max[3] = {430, 436, 436};
//update these from serial monotor
//ADXL_RAW_Min[0] = 330;
//ADXL_RAW_Max[0] = 430;
//ADXL_RAW_Min[1] = 340;
//ADXL_RAW_Max[1] = 436;
//ADXL_RAW_Min[2] = 314;
//ADXL_RAW_Max[2] = 436;
// Take multiple samples to reduce noise
const int sampleSize = 10;
//int theta=1;

void getADLX(const struct ADXL335_CONFIG *ADXL335_PIN, const int sampleSize, const int printStatus, int *theta);
//int getADLX(ADXL335_CONFIG ADXL335_PIN, int &sampleSize, int &printStatus, int &theta);
void TransmitBeacon(const struct NFR24L01_CONFIG *NRF24L01_PIN, const int CODE);

void read_GY521(const int MPU_ADDR, const int printStatus, int16_t *GY_521, int16_t *roll, int16_t *pitch)
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);                        // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false);             // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  int16_t temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[0] = temp;                              // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);

  temp = Wire.read() << 8 | Wire.read();      // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[1] = Wire.read() << 8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[2] = temp;                      // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[3] = temp;                      // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[4] = temp;                      // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[5] = temp;                      // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[6] = temp;                      // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L);

  *roll = atan2(GY_521[1], GY_521[2]) * 57.3;
  *pitch = atan2((-GY_521[0]), sqrt(GY_521[1] * GY_521[1] + GY_521[2] * GY_521[2])) * 57.3;
  // print out data
  if (printStatus == 1)
  {
    Serial.print("Ax, Ay, Az");
    Serial.print(convert_int16_to_str(GY_521[0]));
    Serial.print(convert_int16_to_str(GY_521[1]));
    Serial.print(convert_int16_to_str(GY_521[2]));

    Serial.print(" | tmp = ");
    Serial.print(convert_int16_to_str(GY_521[3]));

    Serial.print(" | gX, gY, gZ = ");
    Serial.print(convert_int16_to_str(GY_521[4]));
    Serial.print(convert_int16_to_str(GY_521[5]));
    Serial.print(" | Roll = ");
    Serial.print(*roll);
    Serial.print(" | Pitch = ");
    Serial.print(*pitch);
    Serial.println();
  }
}

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

//=============
// SETUP Starts
//=============

void setup()
{
  printf_begin();
  Serial.begin(9600); //to display on serial monitor
  servoPhi.attach(servoPhiPin);
  servoTheta.attach(servoThetaPin);
  servoPhi.write(90);
  servoTheta.write(90);

  //QMC
  compass.init();
  //CALIBRATE OMG from calibration

  //compass.setCalibration(-1700, 1128, -311, 1545, -1230, 1247);//for closed arduino
  //compass.setCalibration(-1753, 926, -1803, 592, -1547, 1540);//for open arduino
  compass.setCalibration(-1653, 840, -1695, 997, -1517, 1187); //FOR BS

  compass.setSmoothing(10, true);

  //Serial.println("setUp");
  //ADXL335
  //analogReference(EXTERNAL);//Not working for me
  //analogReference(DEFAULT);

  //NRF24L01
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(details, INPUT_PULLUP);
  pinMode(modePin, INPUT_PULLUP);
  radio.begin();                              //Starting the Wireless communication
  radio.openWritingPipe(DestinationADDRESS);  //Setting the address where we will send the data
  radio.openReadingPipe(0, ListeningADDRESS); //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);              //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();                      //This sets the module as transmitter
  radio.printDetails();

  bool check = radio.isChipConnected();
  Serial.print("Chip Connected?:");
  Serial.println(check);
  delay(100);
  //const char text[] = "11001100";

  //GY_521 MPU6050
  //  Wire.begin();
  // Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  //Wire.write(0x6B); // PWR_MGMT_1 register
  //Wire.write(0); // set to zero (wakes up the MPU-6050)
  //Wire.endTransmission(true);

  //MPU with DMP
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  //Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(9600);

  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  //mpu.setXGyroOffset(220);
  //mpu.setYGyroOffset(76);
  //mpu.setZGyroOffset(-85);
  //mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  //MCU
  //mpu.Initialize();
  //mpu.Calibrate();
  // supply your own gyro offsets here, scaled for min sensitivity from IMU_zero
  //           X Accel  Y Accel  Z Accel   X Gyro   Y Gyro   Z Gyro
  //OFFSETS       62,     429,    3170,     126,       0,     -45
  mpu.setXGyroOffset(126);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(-45);
  //mpu.setXAccelOffset(62);
  // mpu.setYAccelOffset(429);
  mpu.setZAccelOffset(3170); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0)
  {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  else
  {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

  //  int printStatus = 1;
}
//=============
// Setup END
//=============




//==========
//LOOP Start
//==========
void loop()
{
  int printDetails = digitalRead(details);
  //fakeSerialClear();

  //TRANSMITTER MODE
  radio.stopListening();
  for (int i = 0; i < 10; i++)
  {
    B[i] = 0;
    //  C[i]=0;
  }
  FLAG = 0;

  button_state = digitalRead(button_pin);
  if (button_state == LOW)
  {
    FLAG = 255;
  }
  //int16_t roll, pitch; not used 
  //int printHello();
  //int printDetails = 1;
  //int theta = 0;

#ifndef M_PI
  float M_PI = 22 / 7;
#endif
  //read from MPU+DMP THETA
  // if programming failed, don't try to do anything
  if (!dmpReady)
    return;
  // read a packet from FIFO
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
  { // Get the Latest packet
    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    if (printDetails == LOW)
    {
      Serial.print("\nYPR: \t");
      Serial.print(mod(((ypr[0] * 180 / M_PI) + 360), 360.0));

      Serial.print("\t");
      Serial.print(mod(((ypr[1] * 180 / M_PI) + 360 + 90), 360.0));

      Serial.print("\t");
      Serial.print(mod(((ypr[2] * 180 / M_PI) + 360 + 90), 360.0));

      Serial.print("\t");
    }
  }
  int16_t THETA;
  int16_t PHI;

  // read THETA
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  THETA = (ypr[1] * r2d);
  //read from QMC PHI
  PHI = readQMC(0, p2);
  //convert
  PHI = mod((PHI + 360 - 90.0), 360.0); //90* shift, Y points to DA
  THETA = mod(THETA, 360.0);

  //BEACON ENCODING
  B[0] = 0x70FF;

  B[3] = DestinationADDRESS;
  B[4] = CODE;
  B[5] = THETA;
  B[6] = PHI;

  B[9] = FLAG;
  //TRANSMIT BEACON B
  radio.write(&B, sizeof(B));

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
  delay(200);

  //RECEIVER MODE
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

  //delay(800);
  delay(200);

  // read THETA
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  THETA = (ypr[1] * r2d);
  THETA = mod(THETA, 360.0);

  // read PHI
  PHI = readQMC(0, p2);
  PHI = mod((PHI + 360 - 90.0), 360.0); //90* shift, Y points to DA

  int phi0 = PHI;
  //int theta0 = THETA;

  Serial.print("\n MODE \t");
  bool mode = digitalRead(modePin);
  Serial.print("\n MODE \t");
  Serial.print(mode);
  Serial.print("\n");
  if (mode == LOW)
  {
    Serial.print("\n Rotation On \n");
    servoPhi.write(90); //stop
    while (abs(PHI - phi0) < dp)
    {
      // read PHI
      PHI = readQMC(0, p2);
      PHI = mod((PHI + 360 - 90.0), 360.0); //90* shift, Y points to DA
      servoPhi.write(135);                  //move 45in 50ms
      delay(50);
      // servoPhi.write(135);
      //delay(50);
      B[6] = PHI; //update beacon and retransmit
      //TRANSMIT BEACON B
      radio.write(&B, sizeof(B));
    }
    servoPhi.write(90); //stop
  }

  radio.write(&B, sizeof(B));
  Serial.print("\n tx B\n");
  delay(50);
}
//==========
//LOOP Ends
//==========
