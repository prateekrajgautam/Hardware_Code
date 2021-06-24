#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#include "esp_system.h"
 
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
 
typedef struct test_struct {
  int x;
  int y;
} test_struct;
 



void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
    // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
    if (type != WIFI_PKT_MGMT)
        return;
    static const uint8_t ACTION_SUBTYPE = 0xd0;
    static const uint8_t ESPRESSIF_OUI[] = {0x18, 0xfe, 0x34};
    const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
    const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
    const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;
    // Only continue processing if this is an action frame containing the Espressif OUI.
    if ((ACTION_SUBTYPE == (hdr->frame_ctrl & 0xFF)) &&
        (memcmp(hdr->oui, ESPRESSIF_OUI, 3) == 0)) {
         int rssi = ppkt->rx_ctrl.rssi;
     }
}





void setup() {
 
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);
  //For Rssi
        esp_wifi_set_promiscuous(true);
        //esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
   
  // register peer
  esp_now_peer_info_t peerInfo;
    
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
          
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
 
  test_struct test;
  test.x = 10;
  test.y = 20;
 
  esp_err_t result = esp_now_send(
    broadcastAddress, 
    (uint8_t *) &test,
    sizeof(test_struct));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
 
}
 
void loop() {}