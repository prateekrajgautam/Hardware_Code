
//SCL->A5
//SCA->A4
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>
#include <SPI.h>
#include "Wire.h"
#include <math.h>
#include <Arduino.h>
#include <TinyMPU6050.h>

MPU6050 mpu (Wire);
//SCL->A5
//SCA->A4

int FLAG = 0;
struct QMC5883L_CONFIG
{
  int SCL;//A5
  int SCA;//A4
};
QMC5883L_CONFIG QMC5883L_PIN = {A4, A5};
#include <QMC5883LCompass.h>
QMC5883LCompass compass;
int16_t QMC[4];

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










//cofig for NRF24
int i;
struct NFR24L01_CONFIG
{
  int CE;//9
  int CSN;//10
  int MO;//13
  int MI;//11
  int SCK;//12
  uint16_t ADDRESS;
  uint16_t CODE;
};
NFR24L01_CONFIG NFR24L01_PIN = {9, 10, 13, 11, 12, 0x99, 0xF0F0};

struct ADXL335_CONFIG
{
  int x;
  int y;
  int z;
};
ADXL335_CONFIG ADXL335_PIN = {A0, A1, A2};

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

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}





RF24 radio(NFR24L01_PIN.CE, NFR24L01_PIN.CSN); // CE, CSN
//const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//const uint16_t address[sizeof(NFR24L01_PIN.ADDRESS)] = {NFR24L01_PIN.ADDRESS}; //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
const uint16_t address = {NFR24L01_PIN.ADDRESS}; //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.


int button_pin = 6;
uint16_t CODE = NFR24L01_PIN.CODE;
boolean button_state = 0;

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






void read_GY521(const int MPU_ADDR, const int printStatus, int16_t *GY_521, int16_t *roll, int16_t *pitch) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  int16_t temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[0] = temp; // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[1] = Wire.read() << 8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[2] = temp; // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[3] = temp; // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[4] = temp;// reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[5] = temp;// reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L);

  temp = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L);
  GY_521[6] = temp; // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L);

  *roll = atan2(GY_521[1] , GY_521[2]) * 57.3;
  *pitch = atan2((- GY_521[0]) , sqrt(GY_521[1] * GY_521[1] + GY_521[2] * GY_521[2])) * 57.3;
  // print out data
  if (printStatus == 1) {
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





//void printHello();

void setup()
{
  //printf_begin();
  Serial.begin(9600); //to display on serial monitor

  //QMC
  compass.init();
  //compass.setCalibration(-1700, 1128, -311, 1545, -1230, 1247);//for closed arduino
  compass.setCalibration(-1753, 926, -1803, 592, -1547, 1540);//for open arduino

  compass.setSmoothing(10, true);

  //MCU
  mpu.Initialize();
  mpu.Calibrate();







  Serial.println("setUp");
  //ADXL335
  //analogReference(EXTERNAL);//Not working for me
  analogReference(DEFAULT);



  //NRF24L01
  pinMode(button_pin, INPUT_PULLUP);
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
  radio.printDetails();
  bool check = radio.isChipConnected();
  Serial.print("Chip Connected?:");
  Serial.println(check);
  delay(100);
  //const char text[] = "11001100";
  //GY_521 MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  int printStatus = 1;
}











void loop()
{
  Serial.print("LOOP\n");
  FLAG = 0;
  button_state = digitalRead(button_pin);
  if (button_state == LOW) {
    FLAG = 255;


  }
  int16_t roll, pitch;
  //int printHello();
  int printStatus = 1;
  int theta = 0;
  int16_t p1 = &GY_521;
  read_GY521(MPU_ADDR, 0, p1, &roll, &pitch);
  /*Serial.print("\n\tRoll = ");
    Serial.println(roll);
    Serial.print("\n\tPitch = ");
    Serial.println(pitch);
  */
  theta = roll;
  if (roll < pitch) {
    theta = pitch;
  }
  //Serial.print("\n\tTHETA = ");
  //Serial.print(theta);


  mpu.Execute();
  //Serial.print("AngX = ");
  //Serial.print(mpu.GetAngX());
  //Serial.print("  /  AngY = ");
  //Serial.print(mpu.GetAngY());
  //Serial.print("  /  AngZ = ");
  //Serial.println(mpu.GetAngZ());
  int16_t THETA = mpu.GetAngX();


  int16_t p2 = &QMC;
  int16_t PHI = readQMC(0, p2);
  //Serial.print("\tPHI = ");
  //Serial.println(PHI);
  //int16_t PHI = QMC[3];
  //getADLX(&ADXL335_PIN, sampleSize, printStatus, &theta);

  //Serial.println(theta);

  //TransmitBeacon(&NFR24L01_PIN, CODE);
  //NRF and Beacon
  //  CODE = NFR24L01_PIN->CODE;
  //int address =  NFR24L01_PIN->ADDRESS;


  /*
    Serial.print("Reciever Address: ");
    Serial.print(address, HEX);
    Serial.print("; ");
    Serial.print(" Matching Code: ");
    Serial.print(CODE, HEX);
    radio.write(&CODE, sizeof(CODE));
  */

  //send beacon
  //int16_t B[] = {THETA, PHI};







  /*
    //send button state
    button_state = digitalRead(button_pin);
    //Serial.print(button_state);
    Serial.print(" Button Status: ");
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
      const char text[] = "11001100";
      radio.write(&text, sizeof(text)); //Sending the message to receiver
    }
    radio.write(&button_state, sizeof(button_state)); //Sending the message to receiver }
  */


  Serial.print("\t Theta ");
  Serial.print(THETA);
  Serial.print(" Phi ");
  Serial.print(PHI);
  Serial.print("\n");

  radio.write(&CODE, sizeof(CODE));
  radio.write(&THETA, sizeof(THETA));
  radio.write(&PHI, sizeof(PHI));
  radio.write(&FLAG, sizeof(FLAG));
  Serial.print("button and Flag status: ");
  Serial.print(button_state);
  Serial.print("Flag: ");
  Serial.println(FLAG);
  //radio.write(&button_state, sizeof(button_state));
  delay(500);
}








/*


  void TransmitBeacon(struct NRF24L01_CONFIG *NFR24L01_PIN, int CODE)
  {

  //NRF and Beacon
  //  CODE = NFR24L01_PIN->CODE;
  //int address =  NFR24L01_PIN->ADDRESS;

  radio.write(&CODE, sizeof(CODE));
  button_state = digitalRead(button_pin);
  //Serial.print(button_state);
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
    const char text[] = "11001100";
    radio.write(&text, sizeof(text)); //Sending the message to receiver
  }
  radio.write(&button_state, sizeof(button_state)); //Sending the message to receiver }
  }

  void getADLX(const struct ADXL335_CONFIG *ADXL335_PIN, const int sampleSize, const int printStatus, int *theta)
  {
  //Serial.print(printStatus);
  // Read ADXL and convert to phi;
  //Read raw values
  float ADXL_RAW[3] = {0, 0, 0};
  for (i = 0; i < sampleSize; i++)
  {
    ADXL_RAW[0] += analogRead(ADXL335_PIN->x);
    ADXL_RAW[1] += analogRead(ADXL335_PIN->y);
    ADXL_RAW[2] += analogRead(ADXL335_PIN->z);
  }
  float ADXL_RAW_AVG[3];
  for (int i = 0; i < 3; i++)
  {
    ADXL_RAW_AVG[i] = ADXL_RAW[i] / sampleSize;
  }

  // Convert raw values to 'milli-Gs"
  float ADXL_SCALED[3];
  for (int i = 0; i < 3; i++)
  {
    ADXL_SCALED[i] = map(ADXL_RAW_AVG[i], ADXL_RAW_Min[i], ADXL_RAW_Max[i], -1000.0, 1000.0);
    //ADXL_SCALED[i] = map(ADXL_RAW_AVG[i], 294, 438, -1000, 1000);
  }

  // re-scale to fractional Gs
  float ADXL_G[3];
  for (i = 0; i < 3; i++)
  {
    ADXL_G[i] = ADXL_SCALED[i] / 1000.0;
  }

  //Print on serial monitor
  if (printStatus == 1)
  {
    Serial.print("X Y Z : ");
    for (i = 0; i < 3; i++)
    {
      Serial.print(ADXL_RAW_AVG[i]);
      Serial.print(" ");
    }
    Serial.print(" ;  Gx Gy Gz : ");
    for (i = 0; i < 3; i++)
    {
      Serial.print(ADXL_G[i]);
      Serial.print(" ");
    }
    Serial.println("");
  }
  theta = 011;
  //calculate theta from ADXL_G
  delay(50);
  //return theta;
  }


*/
