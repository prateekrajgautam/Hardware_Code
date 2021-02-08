#include <Arduino.h>

#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<printf.h>
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);



#define pi float(22/7)
#define d2r float(2*pi/360)
#define r2d float(360/(2*pi))

RF24 radio(9, 10); // CE, CSN
//SCL->A5
//SCA->A4
int noSignal = 0;
int phiMat[2] = {0, 0}, thetaMat[2] = {0, 0};
int ListeningADDRESS = 0x99;
int DestinationADDRESS=0xFF;
int CODE = 0x7F0F;

//int THETA = NULL, PHI = NULL, B[10], FLAG = 0;
int THETA = 0, PHI = 0, B[10], FLAG = 0;
float C[10];
int reset = 7;
int Detail = 5;






//const byte address[6] = "00001";
//const int address = ListeningADDRESS;
//boolean button_state = 0;
int led_pin = 6;
//int led_pinGnd = 7;
//int CODE_KEY=11001100;
int CODE_KEY = CODE;

unsigned int phiPresent[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int thetaPresent[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//int *p, *t;

unsigned int (*pAddress)[14] = &phiPresent;
unsigned int (*tAddress)[14]= &thetaPresent;

int phiBW=500, thetaBW=500, phiMean=500, thetaMean=500;

  int*phiBWAddress = &phiBW;
  int*thetaBWAddress = &thetaBW;
  int*phiMeanAddress = &phiMean;
  int*thetaMeanAddress = &thetaMean;




void SetBit(unsigned int * A, int k)
{
  k %= 360;
  A[(k / 32)] |= (1 << (k % 32));
}

void ClearBit(unsigned int * A, int k)
{
  k %= 360;
  A[k / 32] &= ~(1 << (k % 32));
}

int GetBit(unsigned int * A, int k)
{
  k %= 360;
  return ((A[k / 32] & (1 << (k % 32))) != 0);
}

void ClearAll(unsigned int * A)
{
  int k;
  for (k = 0; k <= 360; k++)
  {
    A[k / 32] &= ~(1 << (k % 32));
  }
  //Serial.print("\t Clearing matrix\t ");
  //delay(5);
}

void SetAll(unsigned int * A)
{
  int k;
  for (k = 0; k < 360; k++)
  {
    A[(k / 32)] |= (1 << (k % 32));
  }
}




void saveUnique(unsigned int * Present,  int rxVal)
{
  SetBit(Present, rxVal);
}



//min max BW direction
//void calculateDirectionAndBW(unsigned int * T, unsigned int * P, int * thetaBW, int * phiBW, int *meanTheta, int * meanPhi){
//}

void calculateDirectionAndBW(unsigned int * Address,  int * BWAddress, int * meanAddress) {
  int A, Amin = 1000, Amax = -1000;
  int Amean;
  double x=0,y=0,count=0;
  for (int i = 0; i < 360; i++) {
    A = GetBit(Address, i);
    if (A == 1) {
      A = i;
      A = ((A + 360 * 2 - 180) % 360) + 180;
      ( Amin > A ) && ( Amin = A ); // : ();
      ( Amax < A ) && ( Amax = A ); // : () ;
      
      // accumulate x and y components of angle
      x=x+cos(d2r*i);
      y=y+sin(d2r*i);
      count++;
      }
  }
  //calculate average;
  if(count>1){
    x=x/count;
    y=y/count;
    // calculate direction
    Amean = int( r2d * (atan2(y,x)));
  }
  if (count<2){
    Amean=A;
    }
  
   int Adiff = Amax - Amin;
   *meanAddress = Amean;
   *BWAddress = Adiff;
  //*mean = ((((Amax + 360 * 2 - 90) % 360)) + (((Amin + 360 * 2 - 90) % 360))) / 2 + 45;
  //*mean= *mean%360;
  int printDetails = digitalRead(5);
  if (printDetails == LOW) {
    printf("\ncalcFn Amin:%d Amax:%d BW:%d and Mean:%d \n", Amin, Amax, Adiff, Amean);
  }
}




void printDetailedStatus(unsigned int * present) {
  int i;

  for (i = 0; i < 360; i++) {
    if (GetBit(present, i) == 1) {
      Serial.print(i);
      Serial.print(" ");
    }
  }
}







void fakeSerialClear(){
  Serial.print("\t \n \t \n \t \n \t \n \t \n ");    // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n ");    // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n ");    // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n ");    // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n ");    // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n ");    // clear 5 lines
  Serial.print("\t \n \t \n \t \n \t \n \t \n ");    // clear 5 lines
  }





void setup() {

  pinMode(led_pin, OUTPUT);
  pinMode(reset, INPUT_PULLUP);
  pinMode(Detail, INPUT_PULLUP); // details print ar rx




//LCD
lcd.begin(16,2);
lcd.backlight();
//lcd.noBacklight();
//lcd.autoscroll();
lcd.clear();
//lcd.setCursor(c,r);
lcd.setCursor(0,0);
lcd.print("Hi Prateek");
   
  printf_begin();  
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(DestinationADDRESS); //Setting the address where we will send the data
  radio.openReadingPipe(0, ListeningADDRESS);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
  ClearAll(*pAddress); 
  ClearAll(*tAddress);
}












void loop()
{

  //fakeSerialClear();
  //RECEIVER MODE
  radio.startListening();    
        
  /*
    Serial.print("Lisining Address: ");
    Serial.print(address, HEX);
    Serial.print("; ");
    Serial.print(" Matching Code: ");
    Serial.print(CODE, HEX);

    Serial.print("; Status : ");
  */
  int ResetButton = digitalRead(reset);
  int printDetails = digitalRead(Detail);
  //Serial.print("\nResetButtonState :");
  //Serial.println(ResetButton);
  if (ResetButton == 0) {
    Serial.print("\n=====================================\nClean via Reset button\n\n");
    ClearAll(*pAddress); 
    ClearAll(*tAddress);
    delay(10);
  }









  //CHECK AND READ RADIO
  if (radio.available())              //Looking for the data.
  {
      fakeSerialClear();

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
    if (printDetails == LOW) {
      Serial.print("\n BEACON RxD: \t");
      for (int i = 0; i < 10; i++) {
        if (i == 5 || i == 6)
          Serial.print(B[i], DEC);
        else
          Serial.print(B[i], HEX);
        Serial.print(" ");
      }
      Serial.print("\nFLAG: ");
      Serial.print(FLAG);
    }
    //BEACON ENCODE at TX
    /*
        B[0]=0xFFFF;
        B[1]=0x0000;
        B[2]=0xFF00;

        B[3]=CODE;

        B[5]=THETA;
        B[6]=PHI;


        B[9]=FLAG;
    */


    /*
      radio.read(&CODE, sizeof(CODE));
      radio.read(&THETA, sizeof(THETA));
      radio.read(&PHI, sizeof(PHI));
      radio.read(&B, sizeof(B));
      radio.read(&FLAG, sizeof(FLAG));
    */
    THETA = (THETA + 360) % 360;
    PHI = (PHI + 360) % 360;









    if (FLAG == 255) {
      Serial.print("\n=====================================\nClean via FLAG from Tx\n\n");
      ClearAll(*pAddress);
      ClearAll(*tAddress);
      Serial.print("\n=================\n");
      Serial.print("phiPresent\t");
      for(int i=0;i<14;i++){ Serial.print(phiPresent[i]);}
      Serial.print("\nthetaPresent\t");
      for(int i=0;i<14;i++){ Serial.print(thetaPresent[i]);}
      Serial.print("\n");
    
      
      FLAG = 0;
    }
    //saveUniqueThetaPhi(pAddress, tAddress, THETA, PHI);

    saveUnique(*pAddress, PHI);
    saveUnique(*tAddress, THETA);

    calculateDirectionAndBW(*pAddress, phiBWAddress, phiMeanAddress);
    calculateDirectionAndBW(*tAddress, thetaBWAddress, thetaMeanAddress);
    //printf("\n BEAMWIDTH: phiBW:%d \t thetaBW:%d \t",phiBW,thetaBW);
    //printf("\n Direction: phiMean:%d \t thetaMean:%d \t",phiMean,thetaMean);
    for(int i=0;i<10;i++){
      C[i]=0;
    }
    C[0]=phiBW;
    C[1]=phiMean;
    C[2]=thetaBW;
    C[3]=thetaMean;
    float rho = 1/tan(d2r* thetaMean);
    float x = rho*cos(d2r* phiMean);
    float y = rho*sin(d2r* phiMean);
    C[4]=rho;
    C[5]=x;
    C[6]=y;
    
    Serial.print("\n ===================================");
    printf("\n PHI, BW \t%d \t DIR. %d",phiBW,phiMean);
    printf("\n THETA, BW \t%d \t DIR. %d",thetaBW,thetaMean);
    Serial.println("");
    Serial.print("RHO: ");
    Serial.print(rho);
    Serial.print(", X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.println(y);
    
    if (printDetails == LOW) {
      //PRINT THETA RECEIVED
      Serial.print("\n Theta: \t");
      printDetailedStatus(*tAddress);
      //PRINT PHI RECEIVED
      Serial.print("\n Phi: \t\t");
      printDetailedStatus(*pAddress);
    }

    if (CODE == CODE_KEY) {
      //Serial.println("CODE Match");
      digitalWrite(led_pin, HIGH);
      delay(50);
      digitalWrite(led_pin, LOW);
    }
    else if (CODE != CODE_KEY)
    {
      //Serial.println("CODE missMatch");
    }
    noSignal = 0;


char lcdstr[5];
//dtostrf(inputFloat, 6, 4, lcdstr);
lcd.print(lcdstr);
    lcd.clear();

lcd.setCursor(0,0);
lcd.print("T");

lcd.setCursor(1,0);
lcd.print("D");


lcd.setCursor(2,0);
//lcd.print(thetaMean);
dtostrf(thetaMean, 6, 4, lcdstr);
lcd.print(lcdstr);


lcd.setCursor(6,0);
lcd.print("B");

lcd.setCursor(7,0);
//lcd.print(thetaBW);
dtostrf(thetaBW, 6, 4, lcdstr);
lcd.print(lcdstr);

lcd.setCursor(11,0);
lcd.print("R");
lcd.setCursor(12,0);
//lcd.print(rho);
dtostrf(rho, 6, 4, lcdstr);
lcd.print(lcdstr);

lcd.setCursor(14,0);
lcd.print(noSignal);


lcd.setCursor(0,1);
lcd.print("P");

lcd.setCursor(1,1);
lcd.print("D");

lcd.setCursor(2,1);
//lcd.print(phiMean);
dtostrf(phiMean, 6, 4, lcdstr);
lcd.print(lcdstr);


lcd.setCursor(6,1);
lcd.print("B");

lcd.setCursor(7,1);
//lcd.print(phiBW);
dtostrf(phiBW, 6, 4, lcdstr);
lcd.print(lcdstr);
lcd.setCursor(12,1);
//lcd.print(x);
dtostrf(x, 6, 4, lcdstr);
lcd.print(lcdstr);

lcd.setCursor(14,1);
//lcd.print(y);
dtostrf(y, 6, 4, lcdstr);
lcd.print(lcdstr);
  }
  else
  {

    if (noSignal == 0) {
      Serial.print("\n No Signal \t");
      digitalWrite(led_pin, LOW);
      //lcd.clear();
      lcd.setCursor(14,0);
      //lcd.print(noSignal);
      lcd.print("C");
    }
    if (printDetails == LOW) {

      Serial.print(" ");
      Serial.print(noSignal);
      //lcd.clear();
      lcd.setCursor(14,0);
      lcd.print(noSignal);
      noSignal = +noSignal + 1;
      int tempTest = noSignal % 10;
      if (tempTest == 0)
      {
        Serial.print("\n \t");
      }
    }
    delay(5);
  }

  delay(100);
  
  
  if (printDetails == LOW) {  //debug
    Serial.print("\n=================\n");
    Serial.print("phiPresent\t");
    for(int i=0;i<14;i++){ Serial.print(phiPresent[i]);}
    Serial.print("\nthetaPresent\t");
    for(int i=0;i<14;i++){ Serial.print(thetaPresent[i]);}
    Serial.print("\n");
    delay(100);
  }


  //TRANSMITTER MODE
    radio.stopListening();  
    radio.write(C,sizeof(C));
    delay(100);




}