#include <Arduino.h>
#include <ESP8266WiFi.h>      // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h> // Include the Wi-Fi-Multi library

//#include <SPI.h>
//#include <WiFi.h>


//LED pin

#define LED 2 
float max_rssi = -1000;
float min_rssi = -0;
//SSID of your network

const char *ssid = "ESP-01_BS";
const char *password = "password";

const char *ssid2 = "ESP-8266_BS";
const char *password2 = "password";

ESP8266WiFiMulti wifiMulti; // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP(ssid, password);
  //wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2); // add Wi-Fi networks you want to connect to
  //wifiMulti.addAP(ssid3, password3);   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");
  //int i = 0;
  while (wifiMulti.run() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    Serial.print("Connecting");
    delay(1000);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID()); // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
  
}

void loop()
{
  if (wifiMulti.run() != WL_CONNECTED)
  {

    Serial.print("\nDisconnected\n\n");
    while (wifiMulti.run() != WL_CONNECTED)
    { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
      Serial.print("\nRonnecting");
      delay(1000);
      Serial.print('.');
    }
  }
  Serial.print("\n");
  Serial.print(WiFi.SSID());
  Serial.print(" @ RSSI: ");
  float rssi = WiFi.RSSI();
  Serial.print(rssi);
  if(rssi>max_rssi){
    max_rssi=rssi;
  }
  if(rssi<min_rssi){
    min_rssi=rssi;
  }
  Serial.print("  Max: ");
  Serial.print(max_rssi);
  Serial.print("  min: ");
  Serial.println(min_rssi);
  if(max_rssi-rssi<=10 ){
    digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
    delay(50); // Wait for a second
    digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
    //delay(500); // Wait for two seconds
  }
  digitalWrite(LED, HIGH);
}