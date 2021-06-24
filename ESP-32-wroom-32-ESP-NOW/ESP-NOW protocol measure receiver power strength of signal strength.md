https://www.esp32.com/viewtopic.php?t=13889

> Espressif ESP32 Official Forum

# ESP-NOW protocol measure receiver power strength of signal strength
We also use ESP-NOW protocol and read the signal strength from each received packet. To do so, you have to configure a "promiscuous mode" callback function that will be called when each packet is received. For instance:

Code: [Select all](https://www.esp32.com/viewtopic.php?t=13889#)


            esp_wifi_set_promiscuous(true);
            esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);


In the callback function, you can read the RSSI value from the packet header

Code: [Select all](https://www.esp32.com/viewtopic.php?t=13889#)

    void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) {	
    		if (type != WIFI_PKT_MGMT)
                return;            
            if ((ACTION_SUBTYPE == (hdr->frame_ctrl & 0xFF)) &&
                (memcmp(hdr->oui, ESPRESSIF_OUI, 3) == 0)) {
                        int rssi = ppkt->rx_ctrl.rssi;
         }
    }
        




Phil.

