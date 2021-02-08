#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>


//#define NULL 0

//RF24 radio(9, 10); // CE, CSN
//SCL->A5
//SCA->A4

RF24 radio(PB12, PB13); // CE, CSN
int Detail = PA5;
int led_pin = PA6;
int reset = PB0;


int noSignal = 0;
int phiMat[2] = {0, 0}, thetaMat[2] = {0, 0};
int ADDRESS = 0x99;
int CODE = 0x7F0F;

//int THETA = NULL, PHI = NULL, B[10], FLAG = 0;
int THETA = 0, PHI = 0, B[10], FLAG = 0;
//const byte address[6] = "00001";
const int address = ADDRESS;
//boolean button_state = 0;

//int CODE_KEY=11001100;
int CODE_KEY = CODE;

unsigned int phiPresent[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int thetaPresent[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//int *p, *t;
unsigned int *pAddress = phiPresent;
unsigned int *tAddress = thetaPresent;
int phiBW, thetaBW, phiMean, thetaMean;

void SetBit(unsigned int &addA, int k)
{
  unsigned int *A = &addA;
  k %= 360;
  A[(k / 32)] |= (1 << (k % 32));
}

void ClearBit(unsigned int &addA, int k)
{
  unsigned int *A = &addA;
  k %= 360;
  A[k / 32] &= ~(1 << (k % 32));
}

int GetBit(unsigned int &addA, int k)
{
  unsigned int *A = &addA;
  k %= 360;
  return ((A[k / 32] & (1 << (k % 32))) != 0);
}

void ClearAll(unsigned int &addA)
{
  unsigned int *A=&addA;
  int k;
  for (k = 0; k <= 360; k++)
  {
    A[k / 32] &= ~(1 << (k % 32));
  }
  //Serial.print("\t Clearing matrix\t ");
  //delay(5);
}

void SetAll(unsigned int *A)
{
  int k;
  for (k = 0; k < 360; k++)
  {
    A[(k / 32)] |= (1 << (k % 32));
  }
}



void saveUnique(unsigned int *Present, int rxVal)
{
  //int *Present = &addPresent;
  SetBit(*Present, rxVal);
}

//min max BW direction
//void calculateDirectionAndBW(unsigned int * T, unsigned int * P, int * thetaBW, int * phiBW, int *meanTheta, int * meanPhi){
//}

void calculateDirectionAndBW(unsigned int &add, int *BW, int *mean)
{
  unsigned int *Address = &add;

  int A, Amin = 1000, Amax = -1000;

  for (int i = 0; i < 360; i++)
  {
    A = GetBit(*Address, i);
    if (A == 1)
    {
      A = i;
      A = ((A + 360 * 2 - 90) % 360) + 90;
      (Amin > A) && (Amin = A); // : ();
      (Amax < A) && (Amax = A); // : () ;
    }
  }
  *BW = Amax - Amin;
  *mean = ((((Amax + 360 * 2 - 90) % 360)) + (((Amin + 360 * 2 - 90) % 360))) / 2 + 45;
  int printDetails = digitalRead(5);
  if (printDetails == LOW)
  {
    printf("\n Amin:%d Amax:%d BW:%d and Mean:%d \n", Amin, Amax, *BW, *mean);
  }
}

void printDetailedStatus(unsigned int &addpresent)
{
  unsigned int *present = &addpresent;
  int i;
  for (i = 0; i < 360; i++)
  {
    if (GetBit(*present, i) == 1)
    {
      Serial.print(i);
      Serial.print(" ");
    }
  }
}


void blink(int pin,int time)
{
pinMode(pin, OUTPUT);
digitalWrite(pin, HIGH);
delay(time);
digitalWrite(pin, LOW);
delay(time);
}






















void setup()
{

  pinMode(led_pin, OUTPUT);
  pinMode(reset, INPUT_PULLUP);
  pinMode(Detail, INPUT_PULLUP); // details print ar rx
  printf_begin();
  Serial.begin(38400);
  radio.begin();
  radio.openReadingPipe(0, address); //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);     //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();            //This sets the module as receiver
  ClearAll(*pAddress);
  ClearAll(*tAddress);
}

void loop()
{

  int ResetButton = digitalRead(reset);
  int printDetails = digitalRead(Detail);
  if (ResetButton == 0)
  {
    Serial.print("\n=====================================\nClean via Reset button\n\n");
    ClearAll(*pAddress);
    ClearAll(*tAddress);
    delay(10);
  }

  //CHECK AND READ RADIO
  if (radio.available()) //Looking for the data.
  {
    //bool RPDstatus = radio.testRPD();
    /*
      Serial.print("RPD: ");
      Serial.print(RPDstatus);
      Serial.print("; ");
    */

    CODE = 00000000;
    radio.read(&B, sizeof(B));

    CODE = B[4];
    THETA = B[5];
    PHI = B[6];
    FLAG = B[9];
    if (printDetails == LOW)
    {
      Serial.print("\n BEACON RxD: \t");
      for (int i = 0; i < 10; i++)
      {
        if (i == 5 || i == 6)
          Serial.print(B[i], DEC);
        else
          Serial.print(B[i], HEX);
        Serial.print(" ");
      }
      Serial.print("\nFLAG: ");
      Serial.print(FLAG);
    }
    THETA = (THETA + 360) % 360;
    PHI = (PHI + 360) % 360;

    if (FLAG == 255)
    {
      Serial.print("\n=====================================\nClean via FLAG from Tx\n\n");
      ClearAll(*pAddress);
      ClearAll(*tAddress);
      FLAG = 0;
    }
    //saveUniqueThetaPhi(*pAddress, *tAddress, THETA, PHI);

    //saveUnique(*pAddress, PHI);
    //saveUnique(*tAddress, THETA);

    SetBit(*pAddress, PHI);
    SetBit(*tAddress, THETA);

    calculateDirectionAndBW(*pAddress, &phiBW, &phiMean);
    calculateDirectionAndBW(*tAddress, &thetaBW, &thetaMean);
    //printf("\n BEAMWIDTH: phiBW:%d \t thetaBW:%d \t",phiBW,thetaBW);
    //printf("\n Direction: phiMean:%d \t thetaMean:%d \t",phiMean,thetaMean);
    Serial.print("\n ===================================");
    printf("\n PHI, BW \t%d \t DIR. %d", phiBW, phiMean);
    printf("\n THETA, BW \t%d \t DIR. %d", thetaBW, thetaMean);
    Serial.println();

    if (printDetails == LOW)
    {
      //PRINT THETA RECEIVED
      Serial.print("\n Theta: \t");
      printDetailedStatus(*tAddress);
      //PRINT PHI RECEIVED
      Serial.print("\n Phi: \t\t");
      printDetailedStatus(*pAddress);
    }

    if (CODE == CODE_KEY)
    {
      //Serial.println("CODE Match");
      blink(PC13,50);
      digitalWrite(led_pin, HIGH);
      delay(5);
      //digitalWrite(led_pin, LOW);
    }
    else if (CODE != CODE_KEY)
    {
      //Serial.println("CODE missMatch");
    }
    noSignal = 0;
  }
  else
  {

    if (noSignal == 0)
    {
      Serial.print("\n No Signal \t");
      digitalWrite(led_pin, LOW);
    }
    if (printDetails == LOW)
    {

      Serial.print(" ");
      Serial.print(noSignal);
      noSignal = +noSignal + 1;
      int tempTest = noSignal % 10;
      if (tempTest == 0)
      {
        Serial.print("\n \t");
      }
    }
    delay(5);
  }

  delay(400);
  //blink(PC13,500);
}