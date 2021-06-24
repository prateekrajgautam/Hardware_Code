https://www.esp32.com/viewtopic.php?t=13889

> Espressif ESP32 Official Forum

# ESP-NOW protocol measure receiver power strength of signal strength
We also use ESP-NOW protocol and read the signal strength from each received packet. To do so, you have to configure a "promiscuous mode" callback function that will be called when each packet is received. For instance:

Code: [Select all](https://www.esp32.com/viewtopic.php?t=13889#)


            esp_wifi_set_promiscuous(true);
            esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);


In the callback function, you can read the RSSI value from the packet header

​		Code: [Select all](https://www.esp32.com/viewtopic.php?t=13889#)	

```
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
```


Phil.
