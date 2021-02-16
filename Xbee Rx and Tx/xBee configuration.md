## AT mode

### 



### Node/Router/Slave/Rx





### BS/Coordinator/Master/Tx

### Node/Router/Slave/Rx

---

# Firmware: Zigbee suitable for arduino

#### BS/Coordinator/Master/Tx

1. update firmware to Zigbee

2. change these setting

   | keyword | Value              |
   | ------- | ------------------ |
   | ID      | 1234               |
   | CE      | 1(Enable)          |
   | JV      | 0 (Disabled)       |
   | NI      | COORDINATOR (name) |
   | AP      | 0 (Transparent)    |



#### Node/Router/Slave/Rx

1. update firmware to Zigbee

2. change these setting

   | keyword | Value                        |
   | ------- | ---------------------------- |
   | ID      | 1234                         |
   | JV      | 1 (Enable)                   |
   | CE      | 0 (Disabled)                 |
   | NI      | NODE/ROUTER  (name)          |
   | AP      | 0 (Transparent) for API mode |

---



# Firmware: Zigbee in API mode

#### BS/Coordinator/Master/Tx

1. update firmware to Zigbee

2. change these setting

   | keyword | Value                        |
   | ------- | ---------------------------- |
   | ID      | 1234                         |
   | CE      | 1(Enable)                    |
   | JV      | 0 (Disabled)                 |
   | NI      | COORDINATOR (name)           |
   | AP      | 1 (API Enabled) for API mode |



#### Node/Router/Slave/Rx

1. update firmware to Zigbee

2. change these setting

   | keyword | Value                        |
   | ------- | ---------------------------- |
   | ID      | 1234                         |
   | JV      | 1 (Enable)                   |
   | CE      | 0 (Disabled)                 |
   | NI      | NODE/ROUTER  (name)          |
   | AP      | 1 (API Enabled) for API mode |



1. Now in xctu connect and create zigbee packet for receiver mak address and check reception at coordinator 







---





# Firmware: 802.15.4 for p2p

#### BS/Coordinator/Master/Tx

1. update firmware to 802.15.4

2. change these setting

   | keyword | Value                        |
   | ------- | ---------------------------- |
   | ID      | 1234                         |
   | DL      | 0                            |
   | DF      | FFFF (for BROADCAST)         |
   | CE      | 1(Enable)                    |
   | JV      | 0 (Disabled)                 |
   | NI      | COORDINATOR (name)           |
   | AP      | 0 (Transparent) for API mode |



#### Node/Router/Slave/Rx

1. update firmware to Zigbee

2. change these setting

   | keyword | Value                        |
   | ------- | ---------------------------- |
   | ID      | 1234                         |
   | DL      | 0                            |
   | DF      | FFFF (for BROADCAST)         |
   | JV      | 1 (Enable)                   |
   | CE      | 0 (Disabled)                 |
   | NI      | NODE/ROUTER  (name)          |
   | AP      | 0 (Transparent) for API mode |





# Firmware: 802.15.4 to control from Arduino use AP=2

#### BS/Coordinator/Master/Tx

1. update firmware to 802.15.4

2. change these setting

   | keyword | Value                        |
   | ------- | ---------------------------- |
   | ID      | 1234                         |
   | DL      | 0                            |
   | DF      | FFFF (for BROADCAST)         |
   | CE      | 1(Enable)                    |
   | JV      | 0 (Disabled)                 |
   | NI      | COORDINATOR (name)           |
   | AP      | 0 (Transparent) for API mode |



#### Node/Router/Slave/Rx

1. update firmware to Zigbee

2. change these setting

   | keyword | Value                        |
   | ------- | ---------------------------- |
   | ID      | 1234                         |
   | DL      | 0                            |
   | DF      | FFFF (for BROADCAST)         |
   | JV      | 1 (Enable)                   |
   | CE      | 0 (Disabled)                 |
   | NI      | NODE/ROUTER  (name)          |
   | AP      | 0 (Transparent) for API mode |

