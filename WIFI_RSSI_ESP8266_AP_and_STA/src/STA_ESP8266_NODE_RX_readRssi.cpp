#include <Arduino.h>
#include <ESP8266WiFi.h>      // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h> // Include the Wi-Fi-Multi library

//#include <SPI.h>
//#include <WiFi.h>

//SSID of your network

const char *ssid = "ESP-01_BS";
const char *password = "password";

const char *ssid2 = "ESP-8266_BS";
const char *password2 = "password";

ESP8266WiFiMulti wifiMulti; // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

void setup()
{
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
  Serial.println(WiFi.RSSI());
}