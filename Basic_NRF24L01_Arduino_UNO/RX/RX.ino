#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//void SetBit(int * A,k){     ( A[(k/32)] |= (1 << (k%32)) )  }
//#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )  

RF24 radio(9, 10); // CE, CSN
//SCL->A5
//SCA->A4
int noSignal=0;
int phiMat[2] = {0,0},thetaMat[2] = {0,0};
uint16_t ADDRESS = 0x99;
uint16_t CODE = 0xF0F0;

int THETA = 100, PHI = 400, B[] = {90,359},FLAG[] = {0};;
int reset = 7;


    //int ,*t;


    

//const byte address[6] = "00001";
const uint16_t address = ADDRESS;
//boolean button_state = 0;
int led_pin = 6;
int led_pinGnd = 7;
//int CODE_KEY=11001100;
uint16_t CODE_KEY = CODE;

      int phiPresent[12]; 
    int thetaPresent[12]; 
   //int *p, *t;
    int pAddress=&phiPresent;
    int tAddress=&thetaPresent;



/*

void saveMinMax(int PHI,int THETA,int *phiMat,int *thetaMat)
  {
    if(PHI<phiMat[0]) {    phiMat[0]=PHI; }
    if(PHI>phiMat[1]) {    phiMat[1]=PHI; }
    if(THETA<thetaMat[0]) {    thetaMat[0]=THETA; }
    if(THETA>thetaMat[1]) {    thetaMat[1]=THETA; }
    
    PHI=((PHI+180)%360)-180;
    //THETA=((THETA+180)%360)-180;
    if(PHI<phiMat[0]) {    phiMat[0]=PHI; }
    if(PHI>phiMat[1]) {    phiMat[1]=PHI; }
    //if(THETA<thetaMat[2]) {    thetaMat[2]=THETA; }
    //if(THETA>thetaMat[3]) {    thetaMat[3]=THETA; }
    
    Serial.println();
    Serial.print("\t PhiMat");
    for(int i=0;i<2;i++){
      Serial.print("\t");
      Serial.print(phiMat[i]);
    }
    Serial.print("\n");
    Serial.print("\t ThetaMat");
    for(int i=0;i<2;i++){
      Serial.print("\t");
      Serial.print(thetaMat[i]);
    }
    Serial.print("\n");
  }

*/




void SetBit(int *A,int k)  { 
  k %= 360;    
  A[(k/32)] |= (1 << (k%32)) ;
 }
  
void ClearBit(int *A,int k) {     
  k %= 360;
  A[k/32] &= ~(1 << (k%32));
} 

int GetBit( int  *A,  int k )   {
    k %= 360;
  return ( (A[k/32] & (1 << (k%32) )) != 0 ) ;     
}
   
void ClearAll(int *A){     
  int k;
  for(k=0;k<360;k++){
    A[k/32] &= ~(1 << (k%32));
  }
  Serial.print("\t Clearing matrix\t ");
  delay(5);
}
  
void SetAll(int *A){    
  int k; 
  for(k=0;k<360;k++){
    A[(k/32)] |= (1 << (k%32)) ;
  }
}





void saveUniqueThetaPhi(int * phiPresent, int * thetaPresent, int t, int p)
{
  Serial.println("saving");
  //printf("\nreceived variables\n \t %d\t%d\t%d\t%d\t",p,t,phiPresent,thetaPresent);
  int i;
  //p=(p+360)%360;
  //t=(t+360)%360;
  //*(phiPresent + p) =1;
  //*(thetaPresent + t) =1; 
  SetBit(phiPresent,p);
  SetBit(thetaPresent,t);
  
  Serial.print("\n Theta: \t");
  for(i=0;i<360;i++)
  { 
    if(GetBit(thetaPresent,i) == 1){ Serial.print(i);Serial.print(" ");    }
  }
  Serial.print("\n Phi: \t");
  for(i=0;i<360;i++)
  { 
    if(GetBit(phiPresent,i) == 1){ Serial.print(i);Serial.print(" ");    }
  }
}
/*
void resetThetaPhi(int * phiPresent, int * thetaPresent)
{
  int i;
  for(i=0;i<360;i++){ 
    phiPresent[i] = 0;
    thetaPresent[i] = 0;  
   }  
  Serial.print("RESET matrix");
  delay(5);
   
}
*/



   










void setup() {

  pinMode(led_pin, OUTPUT);
  pinMode(reset,INPUT);
  pinMode(led_pinGnd, OUTPUT);
  digitalWrite(led_pinGnd, LOW);
      ClearAll(pAddress);
      ClearAll(tAddress);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
}












void loop()
{
  digitalRead(reset);
  if(digitalRead(reset) == HIGH){
    //resetThetaPhi(p,t);
    ClearAll(pAddress);
    ClearAll(tAddress);
  }
  /*
  Serial.print("Lisining Address: ");
  Serial.print(address, HEX);
  Serial.print("; ");
  Serial.print(" Matching Code: ");
  Serial.print(CODE, HEX);

  Serial.print("; Status : ");
*/








  
  if (radio.available())              //Looking for the data.
  {
    bool RPDstatus = radio.testRPD();
   /* 
    Serial.print("RPD: ");
    Serial.print(RPDstatus);
    Serial.print("; ");
    */
    
    
    CODE = 00000000;
    radio.read(&CODE, sizeof(CODE));
    //radio.read(&text, sizeof(text));    //Reading the data
    radio.read(&THETA, sizeof(THETA)); 
    radio.read(&PHI, sizeof(PHI)); 
    radio.read(&FLAG, sizeof(FLAG));
    Serial.print("\tFLAG: ") ;
    Serial.println(FLAG[0]) ;
    
    //radio.read(&button_state, sizeof(button_state)); 
    //Serial.println(button_state);
    if(FLAG[0] > 0){
      ClearAll(pAddress);
      ClearAll(tAddress);
      FLAG[0] = 0;
    }
    saveUniqueThetaPhi(pAddress, tAddress, THETA, PHI);
    /*
    Serial.print("\n========================================\n");    
    Serial.print("Beacon received: ");
    
    Serial.print("Theta: ");
    Serial.print(THETA);
    
    Serial.print("\t");
    
    Serial.print("Phi: ");
    Serial.print(PHI);
    Serial.println("");
    */
//int *pmat,*tmat;
//*pmat=&phiMat;
//*tmat=&thetaMat;
   //saveMinMax(PHI,THETA,pmat,tmat);
   
    
  //radio.write(&B, sizeof(B));
    //radio.read(&button_state, sizeof(button_state));    //Reading the data

    //Serial.print("Beacon Received");
    if(CODE == CODE_KEY) {
      //Serial.println("CODE Match");
      digitalWrite(led_pin, HIGH);
      delay(5);
      //digitalWrite(led_pin, LOW);
    }
    else if(CODE != CODE_KEY)
    {
      //Serial.println("CODE missMatch");
    }
    noSignal=0;
  }
  else
  {
    
    if(noSignal==0){
      Serial.print("\nNo Signal \t");
      digitalWrite(led_pin, LOW);
    }
    
      Serial.print(" ");
      Serial.print(noSignal);
      noSignal=+noSignal+1;
      int tempTest=noSignal%10;
      if(tempTest==0)
      {
        Serial.print("\n \t");
        }
    
    delay(5);
  }
  delay(100);
}
